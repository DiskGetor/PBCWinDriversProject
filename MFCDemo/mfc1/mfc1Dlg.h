// mfc1Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// Cmfc1Dlg �Ի���
class Cmfc1Dlg : public CDialog
{
// ����
public:
	Cmfc1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnEnChangeId();
	CString m_strUserId;
	CString m_userIdOutput;
	int m_Gender;
	BOOL m_Doc;
	BOOL m_BSinger;
	BOOL m_BTest;
	BOOL m_BDev;
	static DWORD m_SortColum;
	static BOOL m_bAs;
	CListCtrl m_ListCtrl;
	afx_msg void OnNMClickListtest(NMHDR *pNMHDR, LRESULT *pResult);
	static int CALLBACK MyListCompar(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	afx_msg void OnLvnColumnclickListtest(NMHDR *pNMHDR, LRESULT *pResult);
};
