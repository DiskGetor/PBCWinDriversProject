#include "precomp.h"

#pragma pack(1)
typedef struct ServiceDescriptorEntry {
    unsigned int *ServiceTableBase;
    unsigned int *ServiceCounterTableBase; //Used only in checked build
    unsigned int NumberOfServices;
    unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

__declspec(dllimport)  ServiceDescriptorTableEntry_t KeServiceDescriptorTable;
#define SYSTEMSERVICE(_function) KeServiceDescriptorTable.ServiceTableBase[ *(PULONG)((PUCHAR)_function+1)]
#define SDT     SYSTEMSERVICE
#define KSDT KeServiceDescriptorTable

void StartHook(void);
void RemoveHook(void);


#define SystemLoadAndCallImage 38
#define SystemLoadImage        26

typedef struct _SYSTEM_LOAD_AND_CALL_IMAGE 
{ 
	UNICODE_STRING ModuleName;    //ģ������
} SYSTEM_LOAD_AND_CALL_IMAGE, *PSYSTEM_LOAD_AND_CALL_IMAGE; 


//LPC��Ϣ����
typedef enum _LPC_MSG_TYPE

{
	LPC_NEW_MSG,
	LPC_REQUEST,     //���ͻ���ʹ�� NtRequestWaitReplyPort() ������������ʱ������˽��մ���Ϣ
	LPC_REPLY,       //���������ظ�������ʱ���ͻ��� NtRequestWaitReplyPort() �������մ�����Ϣ
	LPC_DATAGRAM,
	LPC_LOST_REPLY,
	LPC_PORT_CLOSED, //���ͻ��˹رն˿ھ��ʱ������˽��մ���Ϣ
	LPC_CLIENT_DIED, //���ͻ����˳�ʱ������˽��ܴ˳���
	LPC_EXCEPTION,
	LPC_DEBUG_EVENT,
	LPC_ERROR_EVENT,
	LPC_CONNECTION_REQUEST, //���ͻ��˵���NtConnectPort����NtSecureConnectPort���Ӷ˿�ʱ����Ӧ�ķ���˽��մ���Ϣ
} LPC_MSG_TYPE;


//LPC��Ϣ�ṹ��
typedef struct LpcMessage 
{
	WORD         ActualMessageLength;
	WORD         TotalMessageLength;
	DWORD        MessageType;
	DWORD        ClientProcessId;
	DWORD        ClientThreadId;
	DWORD        MessageId;
	DWORD        SharedSectionSize;
	BYTE         MessageData[1];      //�䳤�ṹ��
} LPCMESSAGE, *PLPCMESSAGE;

typedef struct _LPCP_NONPAGED_PORT_QUEUE
{
	KSEMAPHORE Semaphore;
	PVOID BackPointer;
} LPCP_NONPAGED_PORT_QUEUE, *PLPCP_NONPAGED_PORT_QUEUE;

typedef struct _LPCP_PORT_QUEUE
{
	PLPCP_NONPAGED_PORT_QUEUE NonPagedPortQueue;
	PKSEMAPHORE Semaphore;
	LIST_ENTRY ReceiveHead;
} LPCP_PORT_QUEUE, *PLPCP_PORT_QUEUE;

typedef struct _LPCP_PORT_OBJECT
{
	struct _LPCP_PORT_OBJECT* ConnectionPort;
	struct _LPCP_PORT_OBJECT* ConnectedPort;
	LPCP_PORT_QUEUE MsgQueue;
	CLIENT_ID Creator;
	PVOID ClientSectionBase;
	PVOID ServerSectionBase;
	PVOID PortContext;
	PETHREAD ClientThread;
	SECURITY_QUALITY_OF_SERVICE SecurityQos;
	SECURITY_CLIENT_CONTEXT StaticSecurity;
	LIST_ENTRY LpcReplyChainHead;
	LIST_ENTRY LpcDataInfoChainHead;
	union
	{
		PEPROCESS ServerProcess;
		PEPROCESS MappingProcess;
	};
	WORD MaxMessageLength;
	WORD MaxConnectionInfoLength;
	ULONG Flags;
	KEVENT WaitEvent;
} LPCP_PORT_OBJECT, *PLPCP_PORT_OBJECT;




//////////////////////////////////////////////////////////////HOOK
//ԭ������ֱ�������ˣ���ΪWIN7 Xp��SSDT����ȫһ�������Ժ�����ַ��̬��ȡ
/*
NTKERNELAPI NTSTATUS ZwLoadDriver(
  IN PUNICODE_STRING DriverServiceName );


//����ZW������ȡNT��ַ
NTSYSAPI NTSTATUS NTAPI ZwSetSystemInformation(
	IN ULONG SystemInformationClass,
	IN PVOID SystemInformation,
	IN SIZE_T SystemInformationLength
	);

NTSYSAPI NTSTATUS NTAPI ZwRequestWaitReplyPort(
	IN HANDLE PortHandle,
	OUT PLPCMESSAGE LpcRequest,
	IN PLPCMESSAGE LpcReply
	);


NTSTATUS NTAPI ZwAlpcSendWaitReceivePort(
	HANDLE PortHandle,
	DWORD SendFlags,
	PLPCMESSAGE SendMessage ,
	PVOID InMessageBuffer ,
	PLPCMESSAGE ReceiveBuffer ,
	PULONG ReceiveBufferSize ,
	PVOID OutMessageBuffer ,
	PLARGE_INTEGER Timeout);
*/

//HOOK����
NTSTATUS Hook_ZwLoadDriver(
  IN PUNICODE_STRING DriverServiceName
  );

NTSTATUS NTAPI HOOK_NtSetSystemInformation
	(
	IN ULONG SystemInformationClass,
	IN OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength
	);

NTSTATUS NTAPI HOOK_NtRequestWaitReplyPort(
	IN HANDLE PortHandle,
	OUT PLPCMESSAGE LpcReply,
	IN PLPCMESSAGE LpcRequest
	);

NTSTATUS NTAPI HOOK_NTAlpcSendWaitReceivePort(
	HANDLE PortHandle,
	DWORD SendFlags,
	PLPCMESSAGE SendMessage ,
	PVOID InMessageBuffer ,
	PLPCMESSAGE ReceiveBuffer ,
	PULONG ReceiveBufferSize ,
	PVOID OutMessageBuffer ,
	PLARGE_INTEGER Timeout);

//////////////////////////////////////////////////////////////HOOK

//����ָ��
typedef NTSTATUS (*ZWLOADDRIVER)(
  IN PUNICODE_STRING DriverServiceName );

typedef NTSTATUS (*NtSETSYSTEMINFORMATION)
	(
	IN ULONG SystemInformationClass,
	IN OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength
	);

typedef NTSTATUS (*NtREQUESTWAITREPLYPORT)(
	IN HANDLE PortHandle,
	OUT PLPCMESSAGE LpcRequest,
	IN PLPCMESSAGE LpcReply
	);

typedef NTSTATUS  (*NTALPCSENDWAITRECEIVEPORT)(
	HANDLE PortHandle,
	DWORD SendFlags,
	PLPCMESSAGE SendMessage ,
	PVOID InMessageBuffer ,
	PLPCMESSAGE ReceiveBuffer ,
	PULONG ReceiveBufferSize ,
	PVOID OutMessageBuffer ,
	PLARGE_INTEGER Timeout);

//ZW������ַ��������ȡSSDT����NT������ַ
static ZWLOADDRIVER              g_pZwLoadDriver = NULL;
static NtSETSYSTEMINFORMATION    g_pZwSetSystemInformation = NULL;
static NtREQUESTWAITREPLYPORT    g_pZwRequestWaitReplyPort = NULL;
static NTALPCSENDWAITRECEIVEPORT g_pZwAlpcSendWaitReceivePort = NULL;

//NT����ԭ��ַ
static ZWLOADDRIVER              g_pOldZwLoadDriver = NULL;
static NtSETSYSTEMINFORMATION    g_pOldNtSetSystemInformation = NULL;
static NtREQUESTWAITREPLYPORT    g_pOldNtRequestWaitReplyPort = NULL;
static NTALPCSENDWAITRECEIVEPORT g_pOldNTAlpcSendWaitReceivePort = NULL;


static HANDLE g_Pid = 0;    //���������Ľ���PID


//����ƥ��
BOOLEAN IsPatternMatch(PUNICODE_STRING Expression, PUNICODE_STRING Name, BOOLEAN IgnoreCase)
{
	return FsRtlIsNameInExpression(
		Expression,
		Name,
		IgnoreCase,//�����������ΪTRUE,��ôExpression�����Ǵ�д��
		NULL
		); 
}

NTSTATUS Hook_ZwLoadDriver(IN PUNICODE_STRING DriverServiceName )
{
	UNICODE_STRING			uPath						= {0};
	NTSTATUS				status						= STATUS_SUCCESS;
	BOOL					skipOriginal				= FALSE;
	WCHAR					szTargetDriver[MAX_PATH]	= {0};
	WCHAR					szTarget[MAX_PATH]			= {0};
	R3_RESULT				CallBackResult				= R3Result_Pass;
	WCHAR					wszPath[MAX_PATH]			= {0};
	UNICODE_STRING  usDestPath = {0};
	WCHAR				wszProcessPath[MAX_PATH] = {0};
	ULONG_PTR ulPtr = 0;
	UNICODE_STRING uExpression;
	DECLARE_UNICODE_STRING_SIZE(StrProcessName, 260);
	__try
	{
		UNICODE_STRING CapturedName;
		
		if((ExGetPreviousMode() == KernelMode) || 
			(DriverServiceName == NULL))
		{
			skipOriginal = TRUE;
			status = g_pOldZwLoadDriver(DriverServiceName);
			return status;
		}
		
		uPath.Length = 0;
		uPath.MaximumLength = MAX_PATH * sizeof(WCHAR);
		uPath.Buffer = wszPath;
		
		
		CapturedName = ProbeAndReadUnicodeString(DriverServiceName);
		
		//�ڴ�����Ӧ�ò㣬���Եò����Ƿ�ɶ�
		ProbeForRead(CapturedName.Buffer, 
			CapturedName.Length,
			sizeof(WCHAR));
		
		RtlCopyUnicodeString(&uPath, &CapturedName);
		
		if(ntGetDriverImagePath(&uPath, szTargetDriver))
		{
			
// 			if(ntIsDosDeviceName(szTargetDriver))
// 			{
// 				if( ntGetNtDeviceName(szTargetDriver, 
// 					szTarget))
// 				{
// 					RtlStringCbCopyW(szTargetDriver, 
// 						sizeof(szTargetDriver), 
// 						szTarget);
// 				}
// 			}
			
			DbgPrint("Driver:%ws will be loaded\n", szTargetDriver);
			//�޸ĵ�������������Ŀ��·��
			usDestPath.Buffer = szTargetDriver;
			usDestPath.Length = wcslen(szTargetDriver)*sizeof(WCHAR);
			usDestPath.MaximumLength = sizeof(szTargetDriver)*sizeof(WCHAR);
			RtlInitUnicodeString(&uExpression, L"*SERVICES.EXE");
			ulPtr = (ULONG_PTR)PsGetCurrentProcessId();
			GetProcessFullNameByPid((HANDLE)ulPtr, &StrProcessName);
			//�򵥵ľ�����������ƥ��Ŷ
			if(IsPatternMatch(&uExpression, &StrProcessName, TRUE))
			{
				ulPtr=(ULONG_PTR)g_Pid;
			}
			CallBackResult = GetResultFromUser((HANDLE)ulPtr,&usDestPath);
			if (CallBackResult == R3Result_Block)
			{
				return STATUS_ACCESS_DENIED;
			}
			
			skipOriginal = TRUE;
			status = g_pOldZwLoadDriver(DriverServiceName);
			return status;
		}	
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		
	}
	
	if(skipOriginal)
		return status;
	
	return g_pOldZwLoadDriver(DriverServiceName);
}


NTSTATUS NTAPI HOOK_NtSetSystemInformation
	(
	IN ULONG SystemInformationClass,
	IN OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength
	)
{

	R3_RESULT CallBackResult = R3Result_Pass;
	PSYSTEM_LOAD_AND_CALL_IMAGE pLoadImg = NULL;
	ULONG_PTR ulPtr = 0;
	UNICODE_STRING uExpression;
	DECLARE_UNICODE_STRING_SIZE(StrProcessName, 260);
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,"HOOK_NtSetSystemInformation  enter Class=%d\n", SystemInformationClass));
	if (SystemInformationClass == SystemLoadAndCallImage ||
		SystemInformationClass == SystemLoadImage)
	{
		pLoadImg = (PSYSTEM_LOAD_AND_CALL_IMAGE)SystemInformation;
		KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,"HOOK_NtSetSystemInformation  loadDriver Class=%d img=%wZ\n", &(pLoadImg->ModuleName)));
		//Ч�鵱ǰ���ؽ���ID
		RtlInitUnicodeString(&uExpression, L"*SERVICES.EXE");
		ulPtr = (ULONG_PTR)PsGetCurrentProcessId();
		GetProcessFullNameByPid((HANDLE)ulPtr, &StrProcessName);
		if(IsPatternMatch(&uExpression, &StrProcessName, TRUE))
		{
			ulPtr=(ULONG_PTR)g_Pid;
		}
		//����
		CallBackResult = GetResultFromUser((HANDLE)ulPtr,NULL);
		if (CallBackResult == R3Result_Block)
		{
			return STATUS_ACCESS_DENIED;
		}
	}
	if (g_pOldNtSetSystemInformation != NULL)
	{
		return g_pOldNtSetSystemInformation(SystemInformationClass,
			                       SystemInformation,
								   SystemInformationLength);
	}
	return STATUS_FAILED_DRIVER_ENTRY;
}


NTSTATUS NTAPI HOOK_NtRequestWaitReplyPort(
	IN HANDLE PortHandle,
	OUT PLPCMESSAGE LpcRequest,
	IN PLPCMESSAGE LpcReply
	)
{
	
	ULONG *ptr = NULL;
	ULONG i = 0, uactLength = 0;
	PLPCP_PORT_OBJECT  LPCProt = NULL;
	DECLARE_UNICODE_STRING_SIZE (uRealName, 256);
	DECLARE_UNICODE_STRING_SIZE (uLpcName, 256);
	RtlInitUnicodeString(&uRealName,L"\\RPC Control\\ntsvcs");
	ObReferenceObjectByHandle(PortHandle,(ACCESS_MASK)PROCESS_ALL_ACCESS,NULL,KernelMode,(PVOID *)&LPCProt,NULL);//��ȡ����
	ObQueryNameString(LPCProt->ConnectionPort,(POBJECT_NAME_INFORMATION)&uLpcName,256,&uactLength);
	if(!(RtlCompareUnicodeString(&uLpcName,&uRealName,TRUE)))  //�������豸�Ǽ���������
	{
		ptr=(ULONG *)(LpcRequest->MessageData);
		/*
		for (i=0; i<LpcReply->ActualMessageLength/sizeof(ULONG); i++)
		{
			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,"%x ", ptr[i])); //�������
		}
		*/
		KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,"LPCNAME:%wZ\n",&uLpcName));
		//if(ptr[0]==0x01&&ptr[1]==0x1f0241)//�е�����
		if(ptr[1]==0x1f0241)
		{
			//������ؽ�����Ϣ��PID��
			g_Pid=PsGetCurrentProcessId();
			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,"DriverLoad PID=%d\n",g_Pid));
		}
	}
	else
	{
		KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,"LPCNAME:%wZ\n",&uLpcName));
	}
	ObDereferenceObject(LPCProt);
	return g_pOldNtRequestWaitReplyPort(PortHandle,LpcReply,LpcRequest);
}

NTSTATUS NTAPI HOOK_NTAlpcSendWaitReceivePort(
	HANDLE PortHandle,
	DWORD SendFlags,
	PLPCMESSAGE SendMessage ,
	PVOID InMessageBuffer ,
	PLPCMESSAGE ReceiveBuffer ,
	PULONG ReceiveBufferSize ,
	PVOID OutMessageBuffer ,
	PLARGE_INTEGER Timeout)
{
	ULONG *ptr = NULL;
	ULONG i = 0, uactLength = 0;
	PLPCP_PORT_OBJECT  LPCProt = NULL;

	//�����˰��죬ObReferenceObjectByHandle ��������ʱ���������Բ�����ֱ���õ�PID�����Ҳ��ȷ

	//DECLARE_UNICODE_STRING_SIZE (uRealName, 256);
	//DECLARE_UNICODE_STRING_SIZE (uLpcName, 256);
	if (SendMessage)
	{
		/*
		RtlInitUnicodeString(&uRealName,L"\\RPC Control\\ntsvcs");
		ObReferenceObjectByHandle(PortHandle,(ACCESS_MASK)PROCESS_ALL_ACCESS,NULL,KernelMode,(PVOID *)&LPCProt,NULL);//��ȡ����
		ObQueryNameString(LPCProt->ConnectionPort,(POBJECT_NAME_INFORMATION)&uLpcName,1024,&uactLength);
		/*
		ptr=(ULONG *)(SendMessage->MessageData);
		if(ptr[1]==0x1f0241 && !(RtlCompareUnicodeString(&uLpcName,&uRealName,TRUE)))  //�������豸�Ǽ���������
		{

			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,"LPCNAME:%wZ\n",&uLpcName));
			//if(ptr[0]==0x01&&ptr[1]==0x1f0241)//�е�����
			//������ؽ�����Ϣ��PID��
			g_Pid=PsGetCurrentProcessId();
			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,"DriverLoad PID=%d\n",g_Pid));
		}
		else
		{
			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,"LPCNAME:%wZ\n",&uLpcName));
		}
		ObDereferenceObject(LPCProt);
		*/
		g_Pid=PsGetCurrentProcessId();
	}

	return g_pOldNTAlpcSendWaitReceivePort(PortHandle,SendFlags,SendMessage,InMessageBuffer, ReceiveBuffer, ReceiveBufferSize, OutMessageBuffer, Timeout);
}

void StartHook (void)
{
    //��ȡδ�����ķ�����������
	UNICODE_STRING uInterface ={0};
    ULONG  ulByteReaded;
	ULONG majorVersion, minorVersion;
	//��ò���ϵͳ�汾
	PsGetVersion( &majorVersion, &minorVersion, NULL, NULL );
    __asm
    {
        push    eax
        mov        eax, CR0
        and        eax, 0FFFEFFFFh
        mov        CR0, eax
        pop        eax
    }
	//XP SSDT����û�� NtAlpcSendWaitReceivePort  WIN7 ���� NTRequestWaitReplyPort Ҳ�޷���ã�ֻ�ֿܷ�����
	RtlInitUnicodeString (&uInterface, L"ZwLoadDriver"); 
	g_pZwLoadDriver = (ZWLOADDRIVER) MmGetSystemRoutineAddress(&uInterface);
	RtlInitUnicodeString (&uInterface, L"ZwSetSystemInformation"); 
	g_pZwSetSystemInformation = (NtSETSYSTEMINFORMATION) MmGetSystemRoutineAddress(&uInterface);
	if (majorVersion == 5 && minorVersion == 1 )
	{
		KdPrintEx((DPFLTR_IHVDRIVER_ID,DPFLTR_ERROR_LEVEL, "comint32: Running on Windows XP\n"));
		RtlInitUnicodeString (&uInterface, L"ZwRequestWaitReplyPort"); 
		g_pZwRequestWaitReplyPort = (NtREQUESTWAITREPLYPORT) MmGetSystemRoutineAddress(&uInterface);
	}
	else if(majorVersion == 6 && minorVersion == 1)
	{
		KdPrintEx((DPFLTR_IHVDRIVER_ID,DPFLTR_ERROR_LEVEL, "comint32: Running on Windows 7\n"));
		RtlInitUnicodeString (&uInterface, L"ZwAlpcSendWaitReceivePort"); 
		g_pZwAlpcSendWaitReceivePort = (NTALPCSENDWAITRECEIVEPORT) MmGetSystemRoutineAddress(&uInterface);
	}

	if (g_pZwLoadDriver != NULL)
	{
		 g_pOldZwLoadDriver = (ZWLOADDRIVER)InterlockedExchange((PLONG)&SDT(g_pZwLoadDriver),(LONG)Hook_ZwLoadDriver);
	}
	if (g_pZwSetSystemInformation != NULL)
	{
		g_pOldNtSetSystemInformation = (NtSETSYSTEMINFORMATION)InterlockedExchange((PLONG)&SDT(g_pZwSetSystemInformation), (LONG)HOOK_NtSetSystemInformation);
	}
	if (g_pZwRequestWaitReplyPort != NULL)
	{
		g_pOldNtRequestWaitReplyPort = (NtREQUESTWAITREPLYPORT)InterlockedExchange((PLONG)&SDT(g_pZwRequestWaitReplyPort), (LONG)HOOK_NtRequestWaitReplyPort);
	}
	if (g_pZwAlpcSendWaitReceivePort != NULL)
	{
		g_pOldNTAlpcSendWaitReceivePort = (NTALPCSENDWAITRECEIVEPORT)InterlockedExchange((PLONG)&SDT(g_pZwAlpcSendWaitReceivePort), (LONG)HOOK_NTAlpcSendWaitReceivePort);
	}
    //�ر�
    __asm
    {
        push    eax
        mov     eax, CR0
        or      eax, NOT 0FFFEFFFFh
        mov     CR0, eax
        pop     eax
    }
    return ;
}

void RemoveHook (void)
{
    __asm
    {
        push    eax
        mov     eax, CR0
        and     eax, 0FFFEFFFFh
        mov     CR0, eax
        pop     eax
    }
	if (g_pZwLoadDriver != NULL && g_pOldZwLoadDriver != NULL)
	{
		InterlockedExchange( (PLONG) &SDT(g_pZwLoadDriver),  (LONG) g_pOldZwLoadDriver);
	}
	if (g_pZwSetSystemInformation != NULL && g_pOldNtSetSystemInformation != NULL)
	{
		InterlockedExchange( (PLONG) &SDT(g_pZwSetSystemInformation),  (LONG) g_pOldNtSetSystemInformation);
	}
	if (g_pZwRequestWaitReplyPort != NULL && g_pOldNtRequestWaitReplyPort != NULL)
	{
		InterlockedExchange( (PLONG) &SDT(g_pZwRequestWaitReplyPort),  (LONG) g_pOldNtRequestWaitReplyPort);
	}
    if (g_pZwAlpcSendWaitReceivePort != NULL && g_pOldNTAlpcSendWaitReceivePort != NULL)
    {
		InterlockedExchange( (PLONG) &SDT(g_pZwAlpcSendWaitReceivePort),  (LONG) g_pOldNTAlpcSendWaitReceivePort);
    }
    __asm
    {
        push    eax
        mov     eax, CR0
        or      eax, NOT 0FFFEFFFFh
        mov     CR0, eax
        pop     eax
    }
}


