#pragma once


// CMyPictureControl �Ի���

class CMyPictureControl : public CDialogEx
{
	DECLARE_DYNAMIC(CMyPictureControl)

public:
	CMyPictureControl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyPictureControl();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PBCSPY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
