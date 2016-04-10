#pragma once


// CDealDlg 对话框

class CDealDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDealDlg)

public:
	CDealDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDealDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
