
// CameraManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#include "CamListDlg.h"
#include "NetWorkDlg.h"
#include "AddCamDlg.h"
#include "CommonInclude.h"
#include "UpdateDataThread.h"
#include "CameraManager.h"

// CCameraManagerDlg �Ի���
class CCameraManagerDlg : public CDialog
{
// ����
public:
	CCameraManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERAMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	bool AddCam(Tmtv_CameraInfo* pCamInfo);
	int CheckCam(Tmtv_CameraInfo* pCamInfo);


public:
	CTabCtrl m_tab;
	int m_CurSelTab;
	CameraManager* pCamManager;
	CUpdateDataThread* pUpdateTread;
	CCamListDlg* pCamListDlg;
	NetWorkDlg* pNetWorkDlg;
	CAddCamDlg* pAddCamDlg;
	CDialog* pDialog[2];










// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam); 
	afx_msg void OnTcnSelchangeFunctionTab(NMHDR *pNMHDR, LRESULT *pResult);


	void HideToTray();
	afx_msg LRESULT OnShowFromTray(WPARAM   wParam, LPARAM   lParam);
};
