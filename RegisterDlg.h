#pragma once


// CRegisterDlg �Ի���

class CRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisterDlg();

//������ݿ�֧��
	CRect m_rect;
	_RecordsetPtr	m_pRecordset;
	_ConnectionPtr	m_pConnection;
	POINT Old;//��ŶԻ���Ŀ�͸ߡ�

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER_DIALOG1 };
#endif

protected:
	HICON m_hIcon;             // ��ʼ�Ի����б���

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();            //��ʼ���Ի���

	DECLARE_MESSAGE_MAP()
public:
	CString m_Rname;
	CString m_Rpawc;
	CString m_Rpaw;
	afx_msg void OnBnClickedRackButton1();
	afx_msg void OnBnClickedRcancelButton2();
};
