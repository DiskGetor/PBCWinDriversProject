// mfc1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc1.h"
#include "mfc1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// Cmfc1Dlg �Ի���




Cmfc1Dlg::Cmfc1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cmfc1Dlg::IDD, pParent)
	, m_strUserId(_T(""))
	, m_userIdOutput(_T(""))
	, m_Gender(0)
	, m_Doc(FALSE)
	, m_BSinger(FALSE)
	, m_BTest(FALSE)
	, m_BDev(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmfc1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, EDIT_ID, m_strUserId);
	DDX_Text(pDX, EDIT_ID_OUTPUT, m_userIdOutput);

	DDX_Radio(pDX, IDC_RADIO_WOMAN, m_Gender);
	DDX_Check(pDX, IDC_CHECK_DEC, m_Doc);
	DDX_Check(pDX, IDC_CHECK_SIGNER, m_BSinger);
	DDX_Check(pDX, IDC_CHECK_Test, m_BTest);
	DDX_Check(pDX, IDC_CHECK_DEV, m_BDev);
	DDX_Control(pDX, IDC_LIST_test, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(Cmfc1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cmfc1Dlg::OnBnClickedOk)
	ON_EN_CHANGE(EDIT_ID, &Cmfc1Dlg::OnEnChangeId)
	
ON_NOTIFY(NM_CLICK, IDC_LIST_test, &Cmfc1Dlg::OnNMClickListtest)
ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_test, &Cmfc1Dlg::OnLvnColumnclickListtest)
END_MESSAGE_MAP()


// Cmfc1Dlg ��Ϣ�������

BOOL Cmfc1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0,_T("�ں˹���"),LVCFMT_LEFT,85);
	m_ListCtrl.InsertColumn(1,_T("Ӧ�ò㹳��"),LVCFMT_LEFT,115);
	m_ListCtrl.InsertColumn(2,_T("ssdt"),LVCFMT_LEFT,60);
	int ilineCount = m_ListCtrl.GetItemCount();

	m_ListCtrl.InsertItem(ilineCount,_T("1"));
	m_ListCtrl.SetItemText(ilineCount,1,_T("�ں˹���1"));
	m_ListCtrl.SetItemText(ilineCount,2,_T("Ӧ�ò㹳��1"));

	m_ListCtrl.InsertItem(ilineCount,_T("2"));
	m_ListCtrl.SetItemText(ilineCount,1,_T("�ں˹���2"));
	m_ListCtrl.SetItemText(ilineCount,2,_T("Ӧ�ò㹳��2"));

	m_ListCtrl.InsertItem(ilineCount,_T("3"));
	m_ListCtrl.SetItemText(ilineCount,1,_T("�ں˹���3"));
	m_ListCtrl.SetItemText(ilineCount,2,_T("Ӧ�ò㹳��3"));


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

void Cmfc1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cmfc1Dlg::OnPaint()
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
HCURSOR Cmfc1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cmfc1Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();

	UpdateData(TRUE);

	//m_userIdOutput = m_strUserId;
	CString userId = m_strUserId;
	CString GenderStr = _T("");
	switch(m_Gender)
	{
	case 0:
		GenderStr += _T("Ů");
		break;
	case 1:
		GenderStr += _T("��");
		break;
	case 2:
		GenderStr += _T("����");
		break;
	default:
		GenderStr += _T("δ֪");
		break;
	}


	

	UpdateData(FALSE);

	MessageBox(m_strUserId,_T("p0sixB1ackcat"),MB_OK);
	MessageBox(GenderStr,_T("�Ա�"),MB_OK);

	while(m_ListCtrl.DeleteItem(0))
	{

	}

}


void Cmfc1Dlg::OnEnChangeId()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}



void Cmfc1Dlg::OnNMClickListtest(NMHDR *pNMHDR, LRESULT *pResult)
{
	int istatus = m_ListCtrl.GetSelectionMark();
	CString strNumOne = m_ListCtrl.GetItemText(istatus,0);
	CString strKernelOne = m_ListCtrl.GetItemText(istatus,1);
	CString strKernelTwo = m_ListCtrl.GetItemText(istatus,2);

}

DWORD Cmfc1Dlg::m_SortColum = 0;
BOOL Cmfc1Dlg::m_bAs = TRUE;



int   CALLBACK Cmfc1Dlg::MyListCompar(LPARAM   lParam1,   LPARAM   lParam2,   LPARAM   lParamSort) 
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
		//_ttol 
		iCompRes=_ttol(szComp1) <=_ttol(szComp2)?-1:1; 
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
void Cmfc1Dlg::OnLvnColumnclickListtest(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_SortColum = pNMListView->iSubItem;
	m_bAs=!m_bAs;//�����ǽ���

	int count = m_ListCtrl.GetItemCount();   //����
	for (int i=0; i<count;  i++)  
	{  
		m_ListCtrl.SetItemData(i,i);  
	}
	m_ListCtrl.SortItems(MyListCompar, (LPARAM) &m_ListCtrl);
	*pResult = 0;
}
