
// PBCDLLInjectDlg.h : ͷ�ļ�
//

#pragma once

typedef ULONG(WINAPI *NTCREATETHREADEX)(PHANDLE pThreadHandle, ACCESS_MASK DesiredAccess, LPVOID ObjectAttributes, HANDLE ProcessHandle, LPTHREAD_START_ROUTINE pThreadStartRoutine, PVOID lpParameter, BOOL CreateSuspended, ULONG StackZeroBits, ULONG SizeOfStackCommit, ULONG SizeOfStackReserve, LPVOID lpBytesBuffer);


// CPBCDLLInjectDlg �Ի���
class CPBCDLLInjectDlg : public CDialogEx
{
// ����
public:
	CPBCDLLInjectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PBCDLLINJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	BOOL CPBCDLLInjectDlg::CheckInput(void);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnCancel();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strProcId;
	afx_msg void OnBnClickedCreateremotethreadinject();
	afx_msg void OnBnClickedNtcreatethreadButton();
	afx_msg void OnBnClickedQueueapcBtn();
	afx_msg void OnBnClickedSetthreadcontextBtn();
	static UINT32 WINAPI SetThreadContextRoutine(PVOID pContext);
	afx_msg void OnBnClickedSetwindowshookexBtn();
};
