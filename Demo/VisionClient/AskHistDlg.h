#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "CommonInclude.h"

// CAskHistDlg �Ի���
class CVisionClientDlg;
class CAskHistDlg : public CDialog
{
	DECLARE_DYNAMIC(CAskHistDlg)

public:
	CAskHistDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAskHistDlg();
	CVisionClientDlg* pParent;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASKHIST_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox mComBoxCtrl;
	CMonthCalCtrl mCalCtrl;
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedOk();
};
