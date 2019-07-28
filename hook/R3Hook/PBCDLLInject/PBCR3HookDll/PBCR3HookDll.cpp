// PBCR3HookDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "PBCR3HookDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//����NtHookEngine�еĺ���������������ָ��������
BOOL(__cdecl *HookFunction)(ULONG_PTR OriginalFunction, ULONG_PTR NewFunction);
VOID(__cdecl *UnHookFunction)(ULONG_PTR Function);
ULONG_PTR(__cdecl *GetOriginalFunction)(ULONG_PTR HookFunction);

//Hook��ĺ���,ǩ��Ҫ��ԭ��������һ��
int WINAPI PBC_MessageBoxA(__in HWND hWnd, __in LPCSTR lpText, __in LPCSTR lpCaption, __in unsigned int uType);

//���屸�ݺ���
int (WINAPI *OldMessageBoxA)(__in HWND hWnd, __in LPCSTR lpText, __in LPCSTR lpCaption, __in unsigned int uType);

VOID PBC_StartHook(VOID);
VOID PBX_UnHook(VOID);

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CPBCR3HookDllApp

BEGIN_MESSAGE_MAP(CPBCR3HookDllApp, CWinApp)
END_MESSAGE_MAP()


// CPBCR3HookDllApp ����

CPBCR3HookDllApp::CPBCR3HookDllApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPBCR3HookDllApp ����

CPBCR3HookDllApp theApp;


// CPBCR3HookDllApp ��ʼ��

BOOL CPBCR3HookDllApp::InitInstance()
{
	CWinApp::InitInstance();

	PBC_StartHook();

	return TRUE;
}

extern "C" __declspec(dllexport)int KeyBoardHook()
{
	static int i = 0;
	if(!i)
		MessageBox(0,"���Ѿ���Hook","message",0);
	++i;
	return 1;
}

VOID PBC_StartHook(VOID)
{
	HMODULE hNtHookEngine;
	ULONG_PTR MessageBoxAAddress;
	hNtHookEngine =  LoadLibrary(_T("NtHookEngine.dll"));
	if (!hNtHookEngine)
	{
		MessageBoxA(0,_T("û���ҵ�NtHookEngine.dll"),_T("message"),0);
		return;
	}
	
	HookFunction = (BOOL (__cdecl *)(ULONG_PTR,ULONG_PTR))
		GetProcAddress(hNtHookEngine, (LPCSTR)_T("HookFunction"));

	UnHookFunction = (VOID(__cdecl *)(ULONG_PTR))
		GetProcAddress(hNtHookEngine, (LPCSTR)_T("UnhookFunction"));

	GetOriginalFunction = (ULONG_PTR(__cdecl *)(ULONG_PTR))
		GetProcAddress(hNtHookEngine, (LPCSTR)_T("GetOriginalFunction"));
	
	if (!HookFunction || !UnHookFunction || !GetOriginalFunction)
	{
		MessageBoxA(0,_T("��ȡNtHookEngine.dll����ʧ��!"), _T("message"), 0);
		return;
	}
	
	//��ʼHook
	MessageBoxAAddress = (ULONG_PTR)GetProcAddress(LoadLibraryA(_T("User32.dll")), _T("MessageBoxA"));
	HookFunction((ULONG_PTR)MessageBoxAAddress, (ULONG_PTR)&PBC_MessageBoxA);
	OldMessageBoxA = (int ( WINAPI *)(HWND,LPCSTR,LPCSTR,UINT))GetOriginalFunction((ULONG_PTR)PBC_MessageBoxA);

}

int WINAPI PBC_MessageBoxA(__in HWND hWnd, __in LPCSTR lpText, __in LPCSTR lpCaption, __in unsigned int uType)
{
	char Buffer[0x200] = {0x00};
	
	sprintf_s(Buffer, 0x200,"PBC:���Ѿ���Hook!%s",lpText);
	return OldMessageBoxA(hWnd, lpText, Buffer, uType);
}

VOID PBC_UnHook(VOID)
{
	ULONG_PTR Original = (ULONG_PTR)GetProcAddress(GetModuleHandle(_T("User32.dll")),(LPCSTR)_T("MessageBoxA"));
	UnHookFunction(Original);
}