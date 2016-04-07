#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "CommonInclude.h"

// CAskHistDlg 对话框
class CVisionClientDlg;
class CAskHistDlg : public CDialog
{
	DECLARE_DYNAMIC(CAskHistDlg)

public:
	CAskHistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAskHistDlg();
	CVisionClientDlg* pParent;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASKHIST_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox mComBoxCtrl;
	CMonthCalCtrl mCalCtrl;
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedOk();
};
