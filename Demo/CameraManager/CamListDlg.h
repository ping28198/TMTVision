#pragma once
#include "afxcmn.h"


// CCamListDlg 对话框
class CCameraManagerDlg;
class CCamListDlg : public CDialog
{
	DECLARE_DYNAMIC(CCamListDlg)

public:
	CCamListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCamListDlg();

public:
	CCameraManagerDlg* pParent;











// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERASINFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddcameraBtn();
	afx_msg void OnNMCustomdrawCamList(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl m_CamList;
	virtual BOOL OnInitDialog();
};
