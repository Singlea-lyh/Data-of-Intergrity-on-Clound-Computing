// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IFServerUI.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REGISTER_DIALOG1, pParent)
	, m_Rname(_T(""))
	, m_Rpawc(_T(""))
	, m_Rpaw(_T(""))
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RNAME_EDIT1, m_Rname);
	DDX_Text(pDX, IDC_RPAWC_EDIT3, m_Rpawc);
	DDX_Text(pDX, IDC_RPAW_EDIT2, m_Rpaw);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RACK_BUTTON1, &CRegisterDlg::OnBnClickedRackButton1)
	ON_BN_CLICKED(IDC_RCANCEL_BUTTON2, &CRegisterDlg::OnBnClickedRcancelButton2)
END_MESSAGE_MAP()


// CRegisterDlg 消息处理程序


//*************************************************************初始化时打开数据库************************************************************
BOOL CRegisterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadStringW(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


									// TODO: Add extra initialization here
	GetClientRect(&m_rect);

	m_pConnection.CreateInstance(__uuidof(Connection));
	try
	{
		// 连接本地数据库
		m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=db.mdb", "", "", adModeUnknown);

	}
	catch (_com_error e)
	{
		AfxMessageBox(_T("数据库连接失败!!!"));
		//		return FALSE;
	}
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	try
	{

		m_pRecordset->Open("SELECT username,password FROM test",                // 查询Demo表中所有字段
			_variant_t((IDispatch*)m_pConnection, true),	            // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}



	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CRegisterDlg::OnBnClickedRackButton1()
{
	// TODO: 在此添加控件通知处理程序代码


	_variant_t vName, vAge;
	CString rname, rpaw;


	try
	{
		UpdateData(TRUE);
		//m_pRecordset->MoveFirst();
		if (!m_pRecordset->adoEOF)
		{
			UpdateData(TRUE);
			m_pRecordset->MoveFirst();
			while (!m_pRecordset->adoEOF)
			{
				UpdateData(TRUE);
				vName = m_pRecordset->GetCollect("username");
				vAge = m_pRecordset->GetCollect("password");

				rname = (LPCTSTR)(_bstr_t)vName;
				rpaw = (LPCTSTR)(_bstr_t)vAge;
				if (0 == rname.Compare(m_Rname) && 0 == rpaw.Compare(m_Rpaw))
				{
					MessageBox(_T("此用户已经注册，请更换注册名"));
					return;
				}

				m_pRecordset->MoveNext();
			}
			//MessageBox(_T("请输入正确的用户名和密码"));
		}

	}

	catch (_com_error &e)
	{
		//AfxMessageBox(e.Description());
	}


	UpdateData(TRUE);

	if ((m_Rname == "") || (m_Rpaw == ""))
	{
		AfxMessageBox(_T("姓名和密码不能为空"));
		return;
	}
	else
	{
		try
		{
			// 写入各字段值
			m_pRecordset->AddNew();
			m_pRecordset->PutCollect("username", _variant_t(m_Rname));
			m_pRecordset->PutCollect("password", _variant_t(m_Rpaw));

			m_pRecordset->Update();

			AfxMessageBox(_T("注册成功!"));
			m_Rname = "";
			m_Rpaw = "";
			UpdateData(FALSE);

		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
	}
	CDialogEx::OnOK();
}


void CRegisterDlg::OnBnClickedRcancelButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
