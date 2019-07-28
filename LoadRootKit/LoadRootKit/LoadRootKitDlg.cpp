
// LoadRootKitDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LoadRootKit.h"
#include "LoadRootKitDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef HRESULT(__stdcall *DLLGEATCLASSOBJECT)(const IID * const rclsid
	, const IID *const riid, LPVOID *ppv);

DLLGEATCLASSOBJECT RootDllGetClassObject;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoadRootKitDlg �Ի���



CLoadRootKitDlg::CLoadRootKitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOADROOTKIT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoadRootKitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CLoadRootKitDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLoadRootKitDlg::OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CLoadRootKitDlg::OnNMRClickList1)
	ON_COMMAND(ID__CALL, &CLoadRootKitDlg::IDCallRootFunc)
END_MESSAGE_MAP()


// CLoadRootKitDlg ��Ϣ�������

BOOL CLoadRootKitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertColumn(0, _T("�������"), LVCFMT_LEFT, 50);
	m_ListCtrl.InsertColumn(1, _T("������"), LVCFMT_LEFT, 300);
	m_ListCtrl.InsertColumn(2, _T("������ַ"), LVCFMT_LEFT, 120);
	//m_ListCtrl.InsertColumn(2, _T("ssdt"), LVCFMT_LEFT, 60);
	int ilineCount = m_ListCtrl.GetItemCount();

	m_ListCtrl.InsertItem(ilineCount, _T("1"));
	m_ListCtrl.SetItemText(ilineCount, 1, _T("DllCanUnloadNow"));
	m_ListCtrl.SetItemText(ilineCount, 2, _T("00000000"));
	
	m_ListCtrl.InsertItem(ilineCount, _T("2"));
	m_ListCtrl.SetItemText(ilineCount, 1, _T("DllGetClassObject"));
	m_ListCtrl.SetItemText(ilineCount, 2, _T("00000000"));

	m_ListCtrl.InsertItem(ilineCount, _T("3"));
	m_ListCtrl.SetItemText(ilineCount, 1, _T("DllRegisterServer"));
	m_ListCtrl.SetItemText(ilineCount, 2, _T("00000000"));

	m_ListCtrl.InsertItem(ilineCount, _T("4"));
	m_ListCtrl.SetItemText(ilineCount, 1, _T("DllUnregisterServer"));
	m_ListCtrl.SetItemText(ilineCount, 2, _T("00000000"));



	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CLoadRootKitDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLoadRootKitDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLoadRootKitDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLoadRootKitDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HMODULE hRootKit = NULL;
	WCHAR *lpBuffer = NULL;
	ULONG dwCanUnloadNow = 0;
	ULONG dwGetClassObjectAddress = 0;
	ULONG dwRegisterServer = 0;
	ULONG dwUnregisterServer = 0;
	
	hRootKit = LoadLibrary(_T("a.dll"));
	if (!hRootKit)
	{
		MessageBox(L"����RootKitʧ��", L"message", 0);
		goto ret;
	}


	lpBuffer = (WCHAR *)malloc(sizeof(WCHAR) * MAX_PATH);
	wnsprintf(lpBuffer,sizeof(WCHAR) * MAX_PATH,_T("����RootKit�ɹ�,Module is 0x%x"),(ULONG)hRootKit);
	MessageBox(lpBuffer, _T("message"), 0);

	memset(lpBuffer, 0x00, sizeof(WCHAR) * MAX_PATH);

	dwCanUnloadNow = (ULONG)GetProcAddress(hRootKit, "DllCanUnloadNow");
	if (!dwCanUnloadNow)
	{
		MessageBox(_T("��ȡDllCanUnloadNow������ַʧ��"),_T("message"),0);
		goto ret;
	}

	wnsprintf(lpBuffer,sizeof(WCHAR) * MAX_PATH,_T("0x%x"),dwCanUnloadNow);
	int dwListItemCount = m_ListCtrl.GetItemCount();
	m_ListCtrl.SetItemText(dwListItemCount - 1, 2, lpBuffer);
	UpdateData(FALSE);

	dwGetClassObjectAddress = (ULONG)GetProcAddress(hRootKit, (LPCSTR)"DllGetClassObject");
	if (!dwGetClassObjectAddress)
	{
		wnsprintf(lpBuffer,sizeof(WCHAR) * MAX_PATH,_T("Get GetClassObject Address faild,ErrorCode is %d!\n"),GetLastError);
		MessageBox(lpBuffer,_T("message"),0);
		goto ret;
	}

	memset(lpBuffer, 0x00, sizeof(WCHAR) * MAX_PATH);

	wnsprintf(lpBuffer,sizeof(WCHAR) * MAX_PATH,_T("0x%x"),dwGetClassObjectAddress);
	m_ListCtrl.SetItemText(dwListItemCount - 2, 2, lpBuffer);
	UpdateData(FALSE);
	//test
	RootDllGetClassObject = (DLLGEATCLASSOBJECT)dwGetClassObjectAddress;	
	RootDllGetClassObject((const IID *const)NULL,(const IID *const)NULL,NULL);

	dwRegisterServer = (ULONG)GetProcAddress(hRootKit, "DllRegisterServer");
	if (!dwRegisterServer)
	{
		MessageBox(_T("Get DllRegisterServer Address faild!"),_T("message"),0);
		goto ret;
	}

	memset(lpBuffer, 0x00, sizeof(WCHAR) * MAX_PATH);
	wnsprintf(lpBuffer, sizeof(WCHAR) * MAX_PATH,_T("0x%x"),dwRegisterServer);
	m_ListCtrl.SetItemText(dwListItemCount - 3, 2, lpBuffer);
	UpdateData(FALSE);

	dwUnregisterServer = (ULONG)GetProcAddress(hRootKit, "DllUnregisterServer");
	if (!dwUnregisterServer)
	{
		MessageBox(_T("GZet DllUnregisterServer Address Faild!"),_T("message"),0);
		goto ret;
	}

	memset(lpBuffer,0x00,sizeof(WCHAR) * MAX_PATH);
	wnsprintf(lpBuffer,sizeof(WCHAR) * MAX_PATH,_T("0x%x"),dwUnregisterServer);
	m_ListCtrl.SetItemText(dwListItemCount - 4, 2, lpBuffer);
	UpdateData(FALSE);



ret:
	{
		if (lpBuffer)
		{
			free(lpBuffer);
			lpBuffer = NULL;
		}
	}
	

	//CDialogEx::OnOK();
}


void CLoadRootKitDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//����̲߳���
		VERIFY(menu.LoadMenu(IDR_MENU1));			//������������1�ж����MENU���ļ�����
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	}
	

}


void CLoadRootKitDlg::IDCallRootFunc()
{
	// TODO: �ڴ���������������
	MessageBox(_T("call!"), _T("message"), 0);

}
