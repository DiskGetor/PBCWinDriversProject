// firstmfc.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CfirstmfcApp:
// �йش����ʵ�֣������ firstmfc.cpp
//

class CfirstmfcApp : public CWinApp
{
public:
	CfirstmfcApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CfirstmfcApp theApp;