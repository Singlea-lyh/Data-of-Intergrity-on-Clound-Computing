#pragma once


// CDealDlg �Ի���

class CDealDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDealDlg)

public:
	CDealDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDealDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
