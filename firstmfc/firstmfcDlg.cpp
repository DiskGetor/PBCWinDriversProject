// firstmfcDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "firstmfc.h"
#include "firstmfcDlg.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD CfirstmfcDlg::m_SortColum = 0;
BOOL CfirstmfcDlg::m_bAs = TRUE;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CfirstmfcDlg �Ի���




CfirstmfcDlg::CfirstmfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CfirstmfcDlg::IDD, pParent)
	, m_strNameInput(_T(""))
	, m_strNameOutput(_T(""))
	, m_bIter(FALSE)
	, m_bDoctor(FALSE)
	, m_bLawyer(FALSE)
	, m_iGender(0)
	, m_strFilePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfirstmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strNameInput);
	DDV_MaxChars(pDX, m_strNameInput, 64);
	DDX_Text(pDX, IDC_EDIT2, m_strNameOutput);
	DDX_Check(pDX, IDC_CHECK_IT, m_bIter);
	DDX_Check(pDX, IDC_CHECK_DOCTOR, m_bDoctor);
	DDX_Check(pDX, IDC_CHECK_LAWYER, m_bLawyer);
	DDX_Radio(pDX, IDC_RADIO_MALE, m_iGender);
	DDX_Control(pDX, IDC_LIST_PRICE, m_listCtrl);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabCtrl);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFilePath);
}

BEGIN_MESSAGE_MAP(CfirstmfcDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CfirstmfcDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CfirstmfcDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDCANCEL, &CfirstmfcDlg::OnBnClickedCancel)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PRICE, &CfirstmfcDlg::OnLvnColumnclickListPrice)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRICE, &CfirstmfcDlg::OnNMClickListPrice)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PRICE, &CfirstmfcDlg::OnNMRClickListPrice)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CfirstmfcDlg::OnTcnSelchangeTabMain)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CfirstmfcDlg::OnBnClickedButtonBrowse)
	ON_WM_DROPFILES()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CfirstmfcDlg ��Ϣ�������

BOOL CfirstmfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_strNameInput=_T("������");
	m_bIter = TRUE;
	m_iGender = 2;

	m_listCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT);
	m_listCtrl.InsertColumn( 0, _T("��վ��Դ"),LVCFMT_LEFT,85);
	m_listCtrl.InsertColumn( 1, _T("��Ʒ"),LVCFMT_LEFT,110);
	m_listCtrl.InsertColumn( 2, _T("�۸�"),LVCFMT_LEFT,80);

	int iLine = m_listCtrl.GetItemCount();
	m_listCtrl.InsertItem( iLine,_T("SOHU"));
	m_listCtrl.SetItemText( iLine, 1,_T("IPHONE") );
	m_listCtrl.SetItemText( iLine, 2,_T("5000") );

	iLine = m_listCtrl.GetItemCount();
	m_listCtrl.InsertItem( iLine,_T("TAOBAO"));
	m_listCtrl.SetItemText( iLine, 1,_T("����") );
	m_listCtrl.SetItemText( iLine, 2,_T("500000") );

	iLine = m_listCtrl.GetItemCount();
	m_listCtrl.InsertItem( iLine,_T("GOOGLE"));
	m_listCtrl.SetItemText( iLine, 1,_T("����") );
	m_listCtrl.SetItemText( iLine, 2,_T("600000") );


	m_tabCtrl.InsertItem(0, _T("����"));
	m_tabCtrl.InsertItem(1, _T("����"));
	m_tabCtrl.InsertItem(2, _T("�ļ�"));

	m_tabCtrl.SetPadding(19);
	m_tabCtrl.SetMinTabWidth(50);
	m_tabCtrl.SetItemSize(CSize(50,25));


	m_pageFile.Create(IDD_DIALOG_PAGE_FILE, &m_tabCtrl);
	m_pageProc.Create(IDD_DIALOG_PAGE_PROC, &m_tabCtrl);
	m_pageNet.Create(IDD_DIALOG_PAGE_NET, &m_tabCtrl);

	CRect rc;
	m_tabCtrl.GetClientRect(rc);
	rc.top += 25;

	m_pageNet.MoveWindow(&rc);
	m_pageFile.MoveWindow(&rc);
	m_pageProc.MoveWindow(&rc);

	m_pDialog[0]=&m_pageProc;
	m_pDialog[1]=&m_pageNet;
	m_pDialog[2]=&m_pageFile;

	//��ʾ��ʼҳ��
	m_pDialog[0]->ShowWindow(SW_SHOW);
	m_pDialog[1]->ShowWindow(SW_HIDE);
	m_pDialog[2]->ShowWindow(SW_HIDE);

	m_CurSelTab = 0;

	UpdateData(FALSE);

	::BringWindowToTop(m_hWnd);
	::SetWindowPos(
		m_hWnd, 
		HWND_TOPMOST, 
		0, 0, 0, 0, 
		SWP_NOMOVE | SWP_NOSIZE);

	// ��������С
	CRect rectWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rectWorkArea,SPIF_SENDCHANGE);   

	// ��öԻ����С
	CRect rectDlg;

	//::AnimateWindow(m_showMsgDlg.m_hWnd,1000,AW_VER_NEGATIVE|AW_SLIDE); 
	//m_showMsgDlg.ShowWindow(SW_SHOW);

	// ��������ҲŲ�����½�

	GetWindowRect(&rectDlg);
	int nW = rectDlg.Width();
	int nH = rectDlg.Height();
	::SetWindowPos(m_hWnd, HWND_BOTTOM,
		rectWorkArea.right-nW-15,rectWorkArea.bottom-nH,nW,nH,SWP_NOZORDER);

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CfirstmfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CfirstmfcDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CfirstmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CfirstmfcDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//���½���ؼ��ϵ�ֵ����Ա������
	MessageBox(m_strNameInput.GetBuffer(0),_T("HI ȷ��"),MB_OK);

	CString strCareer;

	if(m_bDoctor)
	{
		strCareer +=_T("ҽ�� ");
	}
	if(m_bIter)
	{
		strCareer += _T("��ũ ");
	}

	if(m_bLawyer)
	{
		strCareer += _T("��ʦ");
	}

	CString strGender;

	switch(m_iGender)
	{
	case 0:
		strGender = _T("��");
		break;
	case 1:
		strGender = _T("Ů");
		break;
	default:
		strGender = _T("����");
	}
	MessageBox(strCareer.GetBuffer(0),strGender.GetBuffer(0),MB_OK);
	m_strNameOutput = m_strNameInput;

	INT iLine = m_listCtrl.GetItemCount();
	m_listCtrl.InsertItem( iLine,_T("TMALL"));
	m_listCtrl.SetItemText( iLine, 1,_T("����") );
	m_listCtrl.SetItemText( iLine, 2,_T("6000000") );
	UpdateData(FALSE);//���³�Ա������ֵ������ؼ���
	//OnOK();
}

void CfirstmfcDlg::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here
	CLoginDlg loginDlg;

	if(loginDlg.DoModal()==IDOK)
	{
		MessageBox(loginDlg.m_strUser.GetBuffer(0),_T("Hi"),MB_OK);
	}
	
	/*m_loginDlg.Create(IDD_DIALOG_LOGIN, NULL);
	m_loginDlg.ShowWindow(SW_SHOW);*/

	m_listCtrl.DeleteItem(0);
	//MessageBox(_T("Ӧ�ð�ť������"),_T("HI Ӧ��"),MB_OK);
}

void CfirstmfcDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	MessageBox(_T("ȡ����ť������"),_T("HI ȡ��"),MB_OK);
	OnCancel();
}


int   CALLBACK CfirstmfcDlg::MyListCompar(LPARAM   lParam1,   LPARAM   lParam2,   LPARAM   lParamSort) 
{ 
	//ͨ�����ݵĲ������õ�CSortList����ָ�룬�Ӷ��õ�����ʽ 
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;

	//ͨ��ItemData��ȷ������ 

	int   iCompRes; 
	CString    szComp1 = pListCtrl->GetItemText(lParam1,m_SortColum);
	CString    szComp2 = pListCtrl->GetItemText(lParam2,m_SortColum);

	switch(m_SortColum) 
	{ 
	case(2): 
		//�Ե�һ��Ϊ��������   ��� 
		iCompRes=atof((char*)szComp1.GetBuffer(0)) <=atof((char*)szComp2.GetBuffer(0))?-1:1; 
		break; 

	default: 
		iCompRes=szComp1.Compare(szComp2); 
		break; 
	} 
	//���ݵ�ǰ������ʽ���е���

	if(m_bAs) 
		return   iCompRes; 
	else 
		return   -iCompRes; 	
} 
void CfirstmfcDlg::OnLvnColumnclickListPrice(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_SortColum = pNMListView->iSubItem;
	m_bAs=!m_bAs;//�����ǽ���


	int count = m_listCtrl.GetItemCount();   //����
	for (int i=0; i<count;  i++)  
	{  
		m_listCtrl.SetItemData(i,i);  
	}
	m_listCtrl.SortItems(MyListCompar, (LPARAM) &m_listCtrl);
	*pResult = 0;
}

void CfirstmfcDlg::OnNMClickListPrice(NMHDR *pNMHDR, LRESULT *pResult)
{
	int istat=m_listCtrl.GetSelectionMark();//��istat��ŵ�ǰѡ�����ǵڼ���
	CString strSite = m_listCtrl.GetItemText(istat,0);//��ȡ��ǰ���е����ݣ�0�����ǵ�0��
	CString strProductName = m_listCtrl.GetItemText(istat,1);
	INT iPrice = atoi(m_listCtrl.GetItemText(istat,2));

	CString strResult;
	strResult.Format("��վ��Դ��%s, ��Ʒ���ƣ�%s, ��Ʒ�۸�%d", 
		strSite, strProductName, iPrice);

	MessageBox(strResult.GetBuffer(0), _T("listCtrl clicked!"), MB_OK);

	*pResult = 0;
}

void CfirstmfcDlg::OnNMRClickListPrice(NMHDR *pNMHDR, LRESULT *pResult)
{
	int istat=m_listCtrl.GetSelectionMark();//��istat��ŵ�ǰѡ�����ǵڼ���
	CString strSite = m_listCtrl.GetItemText(istat,0);//��ȡ��ǰ���е����ݣ�0�����ǵ�0��
	CString strProductName = m_listCtrl.GetItemText(istat,1);
	INT iPrice = atoi(m_listCtrl.GetItemText(istat,2));

	CString strResult;
	strResult.Format("��վ��Դ��%s, ��Ʒ���ƣ�%s, ��Ʒ�۸�%d", 
		strSite, strProductName, iPrice);

	MessageBox(strResult.GetBuffer(0), _T("listCtrl clicked!"), MB_OK);

	*pResult = 0;
}

void CfirstmfcDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	m_CurSelTab = m_tabCtrl.GetCurSel();
	if (m_pDialog[m_CurSelTab])
	{
		m_pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}

void CfirstmfcDlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here

	static TCHAR BASED_CODE szFilter[] = _T("JPG (*.jpg)|*.jpg|")
		_T("BMP�ļ� (*.bmp)|*.bmp|")
		_T("PNG�ļ� (*.png)|*.png|")
		/*_T("All Files (*.*)|*.*||")*/;

	CFileDialog Filedlg(TRUE,_T(""), NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter, NULL);
	if (Filedlg.DoModal() != IDOK)
	{
		return;
	}
	m_strFilePath = Filedlg.GetPathName();
	UpdateData(FALSE);
}

void CfirstmfcDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	UINT count;
	TCHAR filePath[MAX_PATH] = {0};

	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(count == 1)
	{
		DragQueryFile(hDropInfo, 0, filePath, sizeof(filePath));
		m_strFilePath= filePath;
		//CheckNow(m_strPath);
		UpdateData(FALSE);
		DragFinish(hDropInfo);

		CDialog::OnDropFiles(hDropInfo);
		return;

	}
	else
	{   
		m_vectorFile.clear();
		for(UINT i=0; i<count; i++)
		{
			int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
			
			m_vectorFile.push_back(filePath);
			break;
		}
		m_strFilePath = filePath;
		//DoAllCheck();
		//CheckNow(m_strPath);
		UpdateData(FALSE);
		DragFinish(hDropInfo);
	}


	CDialog::OnDropFiles(hDropInfo);
}

void CfirstmfcDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
}



