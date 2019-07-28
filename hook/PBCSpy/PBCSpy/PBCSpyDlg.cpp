
// PBCSpyDlg.cpp : ʵ���ļ�
//

#include <winuser.h>
#include "stdafx.h"
#include "PBCSpy.h"
#include "PBCSpyDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HMODULE g_hMyDll = NULL;


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

BOOL CALLBACK MainDlgProc(HWND hDlg,	// handle to dialog box
	UINT uMsg,      // message
	WPARAM wParam,  // first message parameter
	LPARAM lParam);

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPBCSpyDlg �Ի���



CPBCSpyDlg::CPBCSpyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PBCSPY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPBCSpyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPBCSpyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_StartDlgButton, &CPBCSpyDlg::OnBnClickedStartdlgbutton)
END_MESSAGE_MAP()


// CPBCSpyDlg ��Ϣ�������

BOOL CPBCSpyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

void CPBCSpyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint t_point;
	DWORD ldwProId;
	HANDLE hProcHandle;
	PROCESSENTRY32 proInfo;
	BOOL more;
	char *buffer = NULL;
	GetCursorPos(&t_point);
	CWnd *windows = WindowFromPoint(t_point);
	GetWindowThreadProcessId(*windows, &ldwProId);
	if (ldwProId == 0)
	{
		MessageBoxA(0, "GetWindowsThreadProcessId error!",0);
		goto ret;
	}

	hProcHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProcHandle == (HANDLE)-1)
	{
		MessageBoxA(0, "CreateToolhelp32Snapshot error!", 0);
		goto ret;
	}

	more = ::Process32First(hProcHandle,&proInfo);
	buffer = (char *)malloc(sizeof(char) * MAX_PATH);
	while (more)
	{
		if (proInfo.th32ProcessID == ldwProId)
		{
			sprintf_s(buffer, sizeof(char) * MAX_PATH
				,"find process name is %s", proInfo.szExeFile);
			//MessageBoxA(0, buffer,0);
		}
		more = ::Process32Next(hProcHandle,&proInfo);
	}





ret:
	if (buffer)
	{
		free(buffer);
		buffer = NULL;
	}
	CDialogEx::OnMouseMove(nFlags,point);
}

void CPBCSpyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPBCSpyDlg::OnPaint()
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
HCURSOR CPBCSpyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//����Ҫ�������Լ���dllע�뵽Ŀ����̣��ú���������Ŀ�꺯���ľ��
void InJectDll(HANDLE hProcess)
{
	HANDLE hRemoteThread;
	char *lpLibPath;
	HMODULE hKernel;
	char *lpRemoteMemory;
	DWORD dwModule = 0;

	lpLibPath = (char *)malloc(MAX_PATH * sizeof(char));
	if (!lpLibPath)
		return;
	hKernel = ::GetModuleHandle(_T("Kernel32"));
	if (hKernel == INVALID_HANDLE_VALUE)
	{
		goto ret;
	}
	
	GetModuleFileName(g_hMyDll,lpLibPath,sizeof(char) * MAX_PATH);
	char *p = strstr(lpLibPath, _T(".exe"));
	if (!p)
	{
		goto ret;
	}
	strcpy_s(lpLibPath,sizeof(char)*MAX_PATH, _T(".dll"));
	lpRemoteMemory = (char *)::VirtualAllocEx(hProcess
		, NULL
		, sizeof(char) * MAX_PATH
		, MEM_COMMIT
		, PAGE_READWRITE);
	if (!lpRemoteMemory)
	{
		goto ret;
	}

	::WriteProcessMemory(hProcess
	,lpRemoteMemory
	,lpLibPath
	,sizeof(char) * MAX_PATH
	,NULL);

	//create remote thread
	hRemoteThread = ::CreateRemoteThread(hProcess
		, NULL
		, 0
		, (LPTHREAD_START_ROUTINE)::GetProcAddress(hKernel, _T("LoadLibraryA"))
		, lpLibPath
		, 0
		, NULL);

	WaitForSingleObject(hRemoteThread, INFINITE);

	::GetExitCodeThread(hRemoteThread, &dwModule);
	::CloseHandle(hRemoteThread);

	if (!dwModule)
	{
		printf("GetExitCodeProcess faild!\n");
		goto ret;
	}

	::VirtualFreeEx(hProcess, lpRemoteMemory, sizeof(char)*MAX_PATH, MEM_RELEASE);

	hRemoteThread = ::CreateRemoteThread(hProcess
		, NULL
		, 0
		,(LPTHREAD_START_ROUTINE)::GetProcAddress(hKernel, _T("FreeLibrary"))
			, (void *)&dwModule
			, 0
			, NULL);

	if (!hRemoteThread)
	{
		printf("FreeLibrary faild!\n");
		goto ret;
	}

	::WaitForSingleObject(hRemoteThread, INFINITE);
	::GetExitCodeThread(hRemoteThread, &dwModule);
	::CloseHandle(hRemoteThread);

ret:
	if (lpLibPath)
	{
		free(lpLibPath);
		lpLibPath = NULL;
	}
}

BOOL CALLBACK MainDlgProc(HWND hDlg,	// handle to dialog box
	UINT uMsg,      // message
	WPARAM wParam,  // first message parameter
	LPARAM lParam) // second message parameter
{
	POINT pt;
	RECT rc;
	static bool bCapture = false;

	static HBITMAP		hBmpCross;
	static HBITMAP		hBmpBlank;


	switch (uMsg) {

	case WM_INITDIALOG:
		
		return true;

	case WM_LBUTTONDOWN:
		pt.x = MAKEPOINTS(lParam).x;
		pt.y = MAKEPOINTS(lParam).y;
		::ClientToScreen(hDlg, &pt);

		
		break;

	case WM_LBUTTONUP:
	case WM_KILLFOCUS:
		if (bCapture)
		{
			
			::ReleaseCapture();
			bCapture = false;
		}
		break;

	case WM_MOUSEMOVE:
		
		break;

	case WM_CTLCOLOREDIT:
		
		break;

	case WM_CLOSE:
		
		return true;
	}

	return false;
}

void CPBCSpyDlg::OnBnClickedStartdlgbutton()
{
	int result = 1;
	char *errorMsg = NULL;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HINSTANCE hInstance = AfxGetInstanceHandle();
	result = ::DialogBoxParam(hInstance
		, MAKEINTRESOURCE(129)
		, NULL
		, MainDlgProc
		, NULL);
	if (result == 0 || result == -1)
	{
		errorMsg = (char *)malloc(sizeof(char) * 0x400);
		int lastError = GetLastError();
		sprintf_s(errorMsg, sizeof(char) * 0x400, "::DialogBoxParam faild:0x%x!", lastError);
		MessageBoxExA(0,errorMsg,"message",0,0);
	}

	if (errorMsg)
	{
		free(errorMsg);
		errorMsg = NULL;
	}
}
