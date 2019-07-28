// firstmfcDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "LoginDlg.h"
#include "PageFile.h"
#include "PageProc.h"
#include "PageNet.h"

#include <vector>
using namespace std;

// CfirstmfcDlg �Ի���
class CfirstmfcDlg : public CDialog
{
// ����
public:
	CfirstmfcDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIRSTMFC_DIALOG };

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
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedCancel();
	CString m_strNameInput;
	CString m_strNameOutput;
	BOOL m_bIter;
	BOOL m_bDoctor;
	BOOL m_bLawyer;
	int m_iGender;
	CListCtrl m_listCtrl;

	static DWORD m_SortColum;
	static BOOL m_bAs;

	CLoginDlg m_loginDlg;

	static int CALLBACK MyListCompar(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	afx_msg void OnLvnColumnclickListPrice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListPrice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListPrice(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tabCtrl;

	CPageFile m_pageFile;
	CPageNet  m_pageNet;
	CPageProc m_pageProc;

#define	MAX_DLG_PAGE 5

	CDialog		 *m_pDialog[MAX_DLG_PAGE];
	int			 m_CurSelTab;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_strFilePath;
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	vector<CString> m_vectorFile;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditFile();
};
