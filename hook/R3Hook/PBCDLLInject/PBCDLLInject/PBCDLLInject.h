
// PBCDLLInject.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPBCDLLInjectApp: 
// �йش����ʵ�֣������ PBCDLLInject.cpp
//

class CPBCDLLInjectApp : public CWinApp
{
public:
	CPBCDLLInjectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPBCDLLInjectApp theApp;