
// DatabaseManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "DatabaseManager.h"
#include "afxcmn.h"

// CDatabaseManagerDlg �Ի���
class CDatabaseManagerDlg : public CDialogEx
{
// ����
public:
	CDatabaseManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATABASEMANAGER_DIALOG };
#endif
public:
	CDatabaseManager *m_pDbmanager;

	void UpdateDbData();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrl m_CamStatusList;
	CListCtrl m_ClientList;
};
