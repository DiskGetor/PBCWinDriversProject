// PBCDLLInject.h : PBCDLLInject DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPBCDLLInjectApp
// �йش���ʵ�ֵ���Ϣ������� PBCDLLInject.cpp
//

class CPBCDLLInjectApp : public CWinApp
{
public:
	CPBCDLLInjectApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
