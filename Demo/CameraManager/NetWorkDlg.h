#pragma once


// NetWorkDlg �Ի���
class CCameraManagerDlg;
class NetWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NetWorkDlg)

public:
	NetWorkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NetWorkDlg();
public:
	CCameraManagerDlg* pParent;









// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESSAGEINFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
