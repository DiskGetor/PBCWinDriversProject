
// PBCSpy.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPBCSpyApp: 
// �йش����ʵ�֣������ PBCSpy.cpp
//

class CPBCSpyApp : public CWinApp
{
public:
	CPBCSpyApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPBCSpyApp theApp;