
// LoadRootKit.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLoadRootKitApp: 
// �йش����ʵ�֣������ LoadRootKit.cpp
//

class CLoadRootKitApp : public CWinApp
{
public:
	CLoadRootKitApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLoadRootKitApp theApp;