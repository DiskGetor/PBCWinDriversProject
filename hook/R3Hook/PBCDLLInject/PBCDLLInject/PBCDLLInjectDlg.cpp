
// PBCDLLInjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PBCDLLInject.h"
#include "PBCDLLInjectDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
//#include <memoryapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL AddDebugPrivilege(VOID);

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPBCDLLInjectDlg �Ի���



CPBCDLLInjectDlg::CPBCDLLInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PBCDLLINJECT_DIALOG, pParent)
	, m_strProcId(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPBCDLLInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PROCID, m_strProcId);
}

BEGIN_MESSAGE_MAP(CPBCDLLInjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDCANCEL, &CPBCDLLInjectDlg::OnCancel)
	ON_BN_CLICKED(ID_NtCreateThread_Button, &CPBCDLLInjectDlg::OnBnClickedNtcreatethreadButton)
	ON_BN_CLICKED(ID_CreateRemoteThreadInject, &CPBCDLLInjectDlg::OnBnClickedCreateremotethreadinject)
	ON_BN_CLICKED(ID_QueueApc_Btn, &CPBCDLLInjectDlg::OnBnClickedQueueapcBtn)
	ON_BN_CLICKED(IDC_SetThreadContext_Btn, &CPBCDLLInjectDlg::OnBnClickedSetthreadcontextBtn)
	ON_BN_CLICKED(IDC_SetWindowsHookEx_Btn, &CPBCDLLInjectDlg::OnBnClickedSetwindowshookexBtn)
END_MESSAGE_MAP()


// CPBCDLLInjectDlg ��Ϣ�������

BOOL CPBCDLLInjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��������ģʽ
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPBCDLLInjectDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CPBCDLLInjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPBCDLLInjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPBCDLLInjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

ULONG __atoi(const char*_source,size_t len)
{
	if (!_source)
		return -1;
	ULONG all = 0;
	char *p = (char *)_source;
	while (*p != '\0')
	{
		if(*p > '9' || *p <'0')
			continue;
		ULONG tmp = *p - '0';
		all = all * 10 + tmp;
		++p;
	}

	return all;

}

BOOL CPBCDLLInjectDlg::CheckInput(void)
{
	UpdateData(TRUE);
	if (m_strProcId.GetLength() == 0)
	{
		MessageBoxA((LPCSTR)_T("������Ŀ�����ID!"), (LPCSTR)_T("message"),0);
		return FALSE;
	}
	return TRUE;
}

//Ϊ��ǰ�������debugȨ��
BOOL AddDebugPrivilege(VOID)
{
	TOKEN_PRIVILEGES tp;
	LUID lUid;
	HANDLE hToken;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &lUid))
	{
		MessageBoxA(0, (LPCSTR)_T("LookupPrivilegeValue faild!"), (LPCSTR)_T("message"), 0);
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = lUid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		MessageBoxA(0,(LPCSTR)_T("OpenProcessToken faild"), (LPCSTR)_T("message"), 0);
		return FALSE;
	}
	
	if (!AdjustTokenPrivileges(hToken
		, FALSE
		, &tp
		, sizeof(TOKEN_PRIVILEGES)
		, (PTOKEN_PRIVILEGES)NULL
		, (PDWORD)NULL))
	{
		MessageBoxA(0, (LPCSTR)_T("AdjustTokenPrivileges faild!"), (LPCSTR)_T("message"), 0);
		return FALSE;
	}

	return TRUE;

}

void CPBCDLLInjectDlg::OnBnClickedNtcreatethreadButton()
{
	typedef struct _NtCreateThreadBuffer
	{
		ULONG Size;
		ULONG Unknown1;
		ULONG Unknown2;
		PULONG Unknown3;
		ULONG Unknown4;
		ULONG Unknown5;
		ULONG Unknown6;
		PULONG Unknown7;
		ULONG Unknown8;
	}NtCreateThreadBuffer;

	HANDLE hProcess = 0;
	ULONG dwProcessId;
	PVOID pVirtualMemory = NULL;
	char *DllPath = "PBCR3HookDll.dll";
	BOOL bResult = FALSE;
	HANDLE hThread = 0;
	PTHREAD_START_ROUTINE pLoadLibraryFunc = NULL;
	LARGE_INTEGER l1 = { 0x00 };
	LARGE_INTEGER l2 = { 0x00 };

	NtCreateThreadBuffer ntBuffer = { 0 };
	memset(&ntBuffer, 0, sizeof(NtCreateThreadBuffer));

	while (CheckInput())
	{
		if (!AddDebugPrivilege())
			break;

		dwProcessId = __atoi(m_strProcId.GetBuffer(), m_strProcId.GetLength());

		hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, dwProcessId);
		if (!hProcess)
		{
			MessageBox("OpenProcess fail!", "message", 0);
			break;
		}

		pVirtualMemory = VirtualAllocEx(hProcess, NULL, strlen(DllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
		if (!pVirtualMemory)
		{
			MessageBox("Alloc Virtual Memory faild!\n", "Message", 0);
			break;
		}

		bResult = ::WriteProcessMemory(hProcess, pVirtualMemory, DllPath, strlen(DllPath), NULL);
		if (!bResult)
		{
			MessageBox("WriteProcessMemory fail!\n");
			break;
		}
		//������߼���������
		NTCREATETHREADEX fNtCreateThreadEx = (NTCREATETHREADEX)GetProcAddress((HMODULE)GetModuleHandle("ntdll.dll"), "NtCreateThreadEx");
		if (!fNtCreateThreadEx)
		{
			MessageBox("Get NtCreateThreadEx Function Address fail!\n");
			break;
		}

		pLoadLibraryFunc = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32.dll"), "LoadLibraryA");

		ntBuffer = { sizeof(NtCreateThreadBuffer), 0x10003, 0x08, (DWORD *)&l2, 0, 0x10004, 4, (DWORD *)&l1, 0 };

		bResult = fNtCreateThreadEx(&hThread, 0x1fffff, NULL, hProcess, pLoadLibraryFunc, pVirtualMemory, FALSE, 0, 0, 0, NULL);
		if (bResult != ERROR_SUCCESS)
		{
			ULONG dwErrorCode = GetLastError();
			break;
		}

		WaitForSingleObject(hThread, INFINITE);

		break;
	}



	if (pVirtualMemory)
		VirtualFreeEx(hProcess, pVirtualMemory, 0, MEM_RELEASE);
	if (hProcess)
		CloseHandle(hProcess);

	if (hThread)
		CloseHandle(hThread);

}


void CPBCDLLInjectDlg::OnBnClickedCreateremotethreadinject()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwTarProcessId;
	HANDLE hTargetProcess;
	HMODULE hKernel32Module;
	ULONG_PTR uLoadLibraryFuncAddress = 0;
	WCHAR lpInjectDllPath[0x20] = L"PBCR3HookDll.dll";
	void *lpRemoteBuffer = NULL;
	HANDLE hRemoteHandle;
	DWORD dwExitCode;
	DWORD dwLastError;
	//����ִ��dllע��

	if (!CheckInput())
		goto finish;

	if (!AddDebugPrivilege())
	{
		goto finish;
	}


	dwTarProcessId = (DWORD)__atoi((const char *)m_strProcId.GetBuffer(), (size_t)m_strProcId.GetLength());
	if (dwTarProcessId == 0)
	{
		MessageBoxA((LPCSTR)_T("__atoi faild!"), (LPCSTR)_T("message"), 0);
		goto finish;
	}

	hTargetProcess = OpenProcess(PROCESS_CREATE_THREAD
		| PROCESS_QUERY_INFORMATION
		| PROCESS_VM_OPERATION
		| PROCESS_VM_READ
		| PROCESS_VM_WRITE
		, FALSE, dwTarProcessId);
	if (hTargetProcess == NULL)
	{
		MessageBoxA((LPCSTR)_T("OpenProcess faild!"), (LPCSTR)_T("message"), 0);
		goto finish;
	}

	hKernel32Module = GetModuleHandle(_T("Kernel32"));
	if (!hKernel32Module)
	{
		MessageBoxA((LPCSTR)_T("û���ҵ�Kernel32.dll!"), _T("message"), 0);
		goto finish;
	}

	uLoadLibraryFuncAddress = (ULONG_PTR)GetProcAddress(hKernel32Module, _T("LoadLibraryW"));
	if (!uLoadLibraryFuncAddress)
	{
		MessageBoxA(_T("��ȡKernel32->LoadLibraryW����ʧ��!"), _T("message"), 0);
		goto finish;
	}

	lpRemoteBuffer = ::VirtualAllocEx(hTargetProcess, NULL, sizeof(lpInjectDllPath), MEM_COMMIT, PAGE_READWRITE);
	dwLastError = GetLastError();
	if (!lpRemoteBuffer)
		goto finish;

	//����Ŀ����̷�����ڴ�д�����ݣ�����Ҫע��dll��·������Ϊ����ִ��Զ���߳�ִ�к����Ĳ���
    if (!::WriteProcessMemory(hTargetProcess, lpRemoteBuffer, lpInjectDllPath, sizeof(lpInjectDllPath), NULL))
    {
        MessageBoxA("д���������ʧ��", "message", 0);
        goto finish;
    }

	hRemoteHandle = CreateRemoteThread(hTargetProcess
		, NULL
		, 0
		, (LPTHREAD_START_ROUTINE)uLoadLibraryFuncAddress
		, lpRemoteBuffer
		, 0
		, NULL);
	if (!hRemoteHandle)
	{
		MessageBoxA(_T("����Զ���߳�ʧ��!"), _T("message"), 0);
		goto finish;
	}

	::WaitForSingleObject(hRemoteHandle, INFINITE);

	::GetExitCodeThread(hTargetProcess, &dwExitCode);

	::CloseHandle(hRemoteHandle);

finish:
	{
		if (lpRemoteBuffer)
		{
			::VirtualFree(lpRemoteBuffer, sizeof(lpInjectDllPath), MEM_RELEASE);
		}
	}
	return;
}

void CPBCDLLInjectDlg::OnBnClickedQueueapcBtn()
{
	ULONG dwProcessId;
	HANDLE hToolSnapShot = NULL;
	HANDLE hProcess = 0;
	HANDLE hThread = 0;
	THREADENTRY32 ThreadEntry;
	BOOL bResult = FALSE;
	ULONG dwErrorCode = 0;
	ULONG dwThreadId = 0;
	PVOID pRemoteBuffer = NULL;
	PVOID pLoadLibraryAddr = NULL;
	char *pstrDllPath = "PBCR3HookDll.dll";

	do
	{
		if (!CheckInput())
			break;
		dwProcessId = __atoi(m_strProcId.GetBuffer(), m_strProcId.GetLength());
		hToolSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		ThreadEntry.dwSize = sizeof(THREADENTRY32);

		if (hToolSnapShot == INVALID_HANDLE_VALUE)
		{
			MessageBox("CreateToolhelp32Snapshot fail!", "message", 0);
			break;
		}

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, dwProcessId);
		if (!hProcess)
		{
			MessageBox("OpenProcess fail!", "message", 0);
			break;
		}

		pRemoteBuffer = VirtualAllocEx(hProcess, NULL, strlen(pstrDllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
		if (!pRemoteBuffer)
		{
			bResult = GetLastError();
			MessageBox("VirtualAllocEx fail!", "message", 0);
			break;
		}

		bResult = WriteProcessMemory(hProcess, pRemoteBuffer, pstrDllPath, strlen(pstrDllPath), NULL);
		if (!bResult)
		{
			MessageBox("WriteProcessMemory fail!", "message", 0);
			break;
		}

		pLoadLibraryAddr = (PVOID)GetProcAddress(GetModuleHandle("Kernel32.dll"),"LoadLibraryA");
		if (!pLoadLibraryAddr)
		{
			MessageBox("GetProcAddress With LoadLibraryA fail!", "message", 0);
			break;
		}

		bResult = Thread32First(hToolSnapShot,&ThreadEntry);
		if (!bResult)
		{
			dwErrorCode = GetLastError();
			MessageBox("Thread32First fail!","message",0);
			break;
		}

		while (bResult)
		{
			bResult = Thread32Next(hToolSnapShot, &ThreadEntry);
			if (!bResult)
			{
				break;
			}

			if (ThreadEntry.th32OwnerProcessID == dwProcessId)
			{
				dwThreadId = ThreadEntry.th32ThreadID;
				hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, dwThreadId);
				if (!hThread)
				{
					MessageBox("OpenThread fail","message",0);
					break;
				}

				bResult = QueueUserAPC((PAPCFUNC)pLoadLibraryAddr, hThread,(ULONG_PTR)pRemoteBuffer);
				if (!bResult)
				{
					MessageBox("QueueUserApc fail!", "message", 0);
					break;
				}
				CloseHandle(hThread);
				hThread = NULL;
			}
		}


		break;
	} while (0);

	if (hProcess)
		CloseHandle(hProcess);
	if (hThread)
		CloseHandle(hThread);
	if (pRemoteBuffer)
		VirtualFree(pRemoteBuffer,strlen(pstrDllPath) + 1,MEM_RELEASE);
	if (hToolSnapShot)
		CloseHandle(hToolSnapShot);

}

ULONG _getthreadid(ULONG dwProcessId)
{
	ULONG dwThread = 0;
	THREADENTRY32 ThreadEntry;
	HANDLE hToolHelp;
	ULONG dwResult = 0;
	HANDLE hThread = 0;

	hToolHelp = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (!hToolHelp)
		return dwThread;

	ThreadEntry.dwSize = sizeof(THREADENTRY32);
	dwResult = Thread32First(hToolHelp, &ThreadEntry);
	do 
	{
		ThreadEntry.dwSize = sizeof(THREADENTRY32);
		if(!dwResult)
			break;
		if (ThreadEntry.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(ThreadEntry.th32OwnerProcessID))
		{
			if (ThreadEntry.th32OwnerProcessID == dwProcessId)
			{
				hThread = OpenThread(READ_CONTROL, FALSE, ThreadEntry.th32ThreadID);
				if(hThread)
					dwThread = ThreadEntry.th32ThreadID;
				CloseHandle(hThread);
				break;
			}
		}

	} while (Thread32Next(hToolHelp,&ThreadEntry));

	CloseHandle(hToolHelp);

	return dwThread;
}

char shellCode[] =
{ 
#ifndef _WIN64
    0x68,0x90,0x90,0x90,0x90 //push xxxx eipռλ��
,0x9c //pushfd
,0x60//pushad cpu�ֳ�
,0x68,0x90,0x90,0x90,0x90//push xxxx dllpath
,0xb8,0x90,0x90,0x90,0x90//mov eax xxxx LoadLibrary������ַռλ��
,0xff,0xd0 //call eax
,0x61 //popad
,0x9d //popfd
,0xc3 //ret
#else
    0
#endif
};

/*
UINT32 WINAPI CPBCDLLInjectDlg::SetThreadContextRoutine(PVOID pContext)
{
	PVOID pRemoteBuffer = NULL;
	PVOID pRemoteDllPath = NULL;
	HANDLE hProcess = NULL;
	ULONG dwProcessId = 0;
	ULONG dwThreadId = 0;
	HANDLE hThread = NULL;
	ULONG dwResult = 0;
	char *pstrDllPath = "PBCR3HookDll.dll";
	PTHREAD_START_ROUTINE pLoadLibraryFuncAddr = NULL;
	CONTEXT ctx;
	ULONG oldEip;
	DWORD oldProtect;
	
	//while (CheckInput())
	{
		dwProcessId = *(ULONG *)pContext;
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwProcessId);
		if (!hProcess)
		{
			dwResult = GetLastError();
			::MessageBox(0,"OpenProcess fail!", "message", 0);
			//break;
			return dwResult;
		}

		pRemoteDllPath = VirtualAllocEx(hProcess, NULL, strlen(pstrDllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
		if (!pRemoteDllPath)
		{
			::MessageBox(0,"VirtuaalAllocEx With pRemoteDllPath fail!", "message", 0);
			return dwResult;
		}

		pLoadLibraryFuncAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32.dll"), "LoadLibraryA");
		if (!pLoadLibraryFuncAddr)
		{
			::MessageBox(0,"GetProcAddress LoadLibraryA fail!", "message", 0);
			return dwResult;
		}

		dwThreadId = _getthreadid(dwProcessId);
		hThread = OpenThread(THREAD_SET_CONTEXT | THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME, FALSE, dwThreadId);
		if (!hThread)
		{
			::MessageBox(0,"OpenThread fail!", "message", 0);
			return dwResult;
		}

		pRemoteBuffer = VirtualAllocEx(hProcess, NULL, sizeof(shellCode), MEM_COMMIT, PAGE_EXECUTE_READ);
		if (!pRemoteBuffer)
		{
			::MessageBox(0,"VirtualAllocEx With pRemoteBuffer fail!", "message", 0);
			return dwResult;
		}

		dwResult = WriteProcessMemory(hProcess, pRemoteDllPath, pstrDllPath, strlen(pstrDllPath), NULL);

		if (!dwResult)
		{
			::MessageBox(0,"WriteProcess Memory Fail With Dll Path!", "message", 0);
			return dwResult;
		}

		SuspendThread(hThread);

		ctx.ContextFlags = CONTEXT_CONTROL;
		dwResult = GetThreadContext(hThread, &ctx);
		if (!dwResult)
		{
			::MessageBox(0,"GetThreadContext fail!", "message", 0);
			return dwResult;
		}

		oldEip = ctx.Eip;
		ctx.Eip = (DWORD)pRemoteBuffer;
		ctx.ContextFlags = CONTEXT_CONTROL;
		VirtualProtect(pRemoteBuffer, sizeof(shellCode), PAGE_EXECUTE_READWRITE, &oldProtect);
		CopyMemory(shellCode + 1, &oldEip, 4);
		CopyMemory(shellCode + 8, &pRemoteDllPath, 4);
		CopyMemory(shellCode + 0xd, &pLoadLibraryFuncAddr, 4);

		dwResult = WriteProcessMemory(hProcess, pRemoteBuffer, shellCode, sizeof(shellCode), NULL);
		if (!dwResult)
		{
			::MessageBox(0,"WriteProcessMemory pRemoteBuffer Fail!", "message", 0);
			return dwResult;
		}

		SetThreadContext(hThread, &ctx);
		ResumeThread(hThread);

	}

	//Sleep(8000);

	if (pRemoteDllPath)
		VirtualFree(pRemoteDllPath, strlen(pstrDllPath), 0);
	if (pRemoteBuffer)
		VirtualFree(pRemoteBuffer, sizeof(shellCode), 0);
	if (hProcess)
		CloseHandle(hProcess);
	if (hThread)
		CloseHandle(hThread);
	
	return dwResult;

}
*/

void CPBCDLLInjectDlg::OnBnClickedSetthreadcontextBtn()
{
	HANDLE hThread = NULL;
	ULONG dwProcessId;

	if (!CheckInput())
		return;

	dwProcessId = __atoi(m_strProcId.GetBuffer(),m_strProcId.GetLength());

	//hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SetThreadContextRoutine, &dwProcessId, 0, 0);

	WaitForSingleObject(hThread, INFINITE);

	printf("\n");
}


void CPBCDLLInjectDlg::OnBnClickedSetwindowshookexBtn()
{
	ULONG dwProcessId;
	ULONG dwThreadId;
	HMODULE hDll;
	PVOID fDllAddr;
	HHOOK hh;

	if (!CheckInput())
		return;
	dwProcessId = __atoi(m_strProcId.GetBuffer(),m_strProcId.GetLength());

	dwThreadId = _getthreadid(dwProcessId);

	hDll = LoadLibraryEx("PBCR3HookDll.dll",NULL,DONT_RESOLVE_DLL_REFERENCES);
	if (!hDll)
	{
		ULONG dwErrorCode = GetLastError();
		MessageBox("GetModuleHanle Fail With PBCR3HookDll!","message",0);
		return;
	}
	
	fDllAddr = GetProcAddress(hDll, "KeyBoardHook");
	if (!fDllAddr)
	{
		MessageBox("GetProcAddress Fail!","message",0);
		return;
	}

	hh = SetWindowsHookEx(WH_MOUSE,(HOOKPROC)fDllAddr,hDll,dwThreadId);



}
