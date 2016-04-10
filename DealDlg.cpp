// DealDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IFServerUI.h"
#include "DealDlg.h"
#include "afxdialogex.h"


// CDealDlg 对话框

IMPLEMENT_DYNAMIC(CDealDlg, CDialogEx)

CDealDlg::CDealDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN_DIALOG1, pParent)
{

}

CDealDlg::~CDealDlg()
{
}

void CDealDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDealDlg, CDialogEx)
END_MESSAGE_MAP()


// CDealDlg 消息处理程序
