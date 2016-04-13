#pragma once


// NetWorkDlg 对话框
class CCameraManagerDlg;
class NetWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NetWorkDlg)

public:
	NetWorkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NetWorkDlg();
public:
	CCameraManagerDlg* pParent;









// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESSAGEINFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
