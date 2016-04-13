#pragma once
#include "afxcmn.h"


// CCamListDlg �Ի���
class CCameraManagerDlg;
class CCamListDlg : public CDialog
{
	DECLARE_DYNAMIC(CCamListDlg)

public:
	CCamListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCamListDlg();

public:
	CCameraManagerDlg* pParent;











// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERASINFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddcameraBtn();
	afx_msg void OnNMCustomdrawCamList(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl m_CamList;
	virtual BOOL OnInitDialog();
};
