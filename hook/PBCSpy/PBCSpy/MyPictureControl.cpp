// MyPictureControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PBCSpy.h"
#include "MyPictureControl.h"
#include "afxdialogex.h"


// CMyPictureControl �Ի���

IMPLEMENT_DYNAMIC(CMyPictureControl, CDialogEx)

CMyPictureControl::CMyPictureControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PBCSPY_DIALOG, pParent)
{

}

CMyPictureControl::~CMyPictureControl()
{
}

void CMyPictureControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyPictureControl, CDialogEx)
END_MESSAGE_MAP()


// CMyPictureControl ��Ϣ�������
