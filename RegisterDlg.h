#pragma once


// CRegisterDlg 对话框

class CRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisterDlg();

//添加数据库支持
	CRect m_rect;
	_RecordsetPtr	m_pRecordset;
	_ConnectionPtr	m_pConnection;
	POINT Old;//存放对话框的宽和高。

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER_DIALOG1 };
#endif

protected:
	HICON m_hIcon;             // 初始对话框中变量

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();            //初始化对话框

	DECLARE_MESSAGE_MAP()
public:
	CString m_Rname;
	CString m_Rpawc;
	CString m_Rpaw;
	afx_msg void OnBnClickedRackButton1();
	afx_msg void OnBnClickedRcancelButton2();
};
