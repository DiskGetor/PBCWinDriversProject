// PBCR3HookDll.h : PBCR3HookDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPBCR3HookDllApp
// �йش���ʵ�ֵ���Ϣ������� PBCR3HookDll.cpp
//

class CPBCR3HookDllApp : public CWinApp
{
public:
	CPBCR3HookDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
