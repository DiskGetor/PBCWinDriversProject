// PBCDLLInject.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "PBCDLLInject.h"
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//share segment with process
#pragma  data_seg(".shared")
HWND hPedit = NULL;
char lpPasswordBuffer[128] = {0x00};
#pragma  data_seg()

#pragma  comment(Linker,"/Section:.shared","RWS")

BEGIN_MESSAGE_MAP(CPBCDLLInjectApp, CWinApp)
END_MESSAGE_MAP()


// CPBCDLLInjectApp ����

CPBCDLLInjectApp::CPBCDLLInjectApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPBCDLLInjectApp ����

CPBCDLLInjectApp theApp;


// CPBCDLLInjectApp ��ʼ��

BOOL CPBCDLLInjectApp::InitInstance()
{
	CWinApp::InitInstance();
	
	return TRUE;
}

BOOL DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{

}