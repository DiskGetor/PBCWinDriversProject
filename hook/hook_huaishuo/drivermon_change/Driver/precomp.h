#include <ntifs.h>        //NT�ļ��ļ�ϵͳͷ�ļ�
#include <ntddk.h>        //NT�豸����ͷ�ļ�
#include <ntstrsafe.h>    //�ַ�����ȫ����
#include <windef.h>       //����windows����
#include <ntimage.h>      //�ļ���ʽ
#include "Ioctlcmd.h"
#include "main.h"
#include "hook.h"
#include "misc.h"

NTSTATUS
NTAPI
ZwQueryInformationProcess(
						  __in HANDLE ProcessHandle,
						  __in PROCESSINFOCLASS ProcessInformationClass,
						  __out_bcount(ProcessInformationLength) PVOID ProcessInformation,
						  __in ULONG ProcessInformationLength,
						  __out_opt PULONG ReturnLength
    );