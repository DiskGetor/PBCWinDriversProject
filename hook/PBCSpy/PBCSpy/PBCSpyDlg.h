
// PBCSpyDlg.h : ͷ�ļ�
//
#pragma once
#include "MyPictureControl.h"

// CPBCSpyDlg �Ի���
class CPBCSpyDlg : public CDialogEx
{
// ����
public:
	CPBCSpyDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PBCSPY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	void OnMouseMove(UINT nFlags, CPoint point);
	CMyPictureControl g_mypiccontrol;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartdlgbutton();
};
