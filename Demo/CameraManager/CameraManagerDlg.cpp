
// CameraManagerDlg.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "CameraManagerApp.h"
#include "CameraManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SHOWFROMTRAY (WM_USER+10)
// CCameraManagerDlg 对话框



CCameraManagerDlg::CCameraManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAMERAMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCameraManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FUNCTIONTAB, m_tab);
}

bool CCameraManagerDlg::AddCam(Tmtv_CameraInfo* pCamInfo)
{
	
	return pCamManager->AddCamera(*pCamInfo);
}

int CCameraManagerDlg::CheckCam(Tmtv_CameraInfo* pCamInfo)
{
	vector<CameraObject*>::iterator it;
	Tmtv_CameraInfo mCam;
	for (it = pCamManager->m_CameraObjectVector.begin(); it != pCamManager->m_CameraObjectVector.end(); it++)
	{
		(*it)->GetCamInfo(&mCam);
		if (mCam.Indexnum == pCamInfo->Indexnum)
		{
			return 1;
		}
		if (strcmp(mCam.CameraName, pCamInfo->CameraName) == 0)
		{
			return 2;
		}
		if (strcmp(mCam.CameraPath, pCamInfo->CameraPath) == 0)
		{
			return 3;
		}
	}
	return 0;
}

BEGIN_MESSAGE_MAP(CCameraManagerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_SHOWFROMTRAY, OnShowFromTray)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FUNCTIONTAB, &CCameraManagerDlg::OnTcnSelchangeFunctionTab)
END_MESSAGE_MAP()


// CCameraManagerDlg 消息处理程序

BOOL CCameraManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	LoggerServer::mLogger.TraceKeyInfo("-----------------------------------程序启动！");
	pCamListDlg = new CCamListDlg(this);
	pNetWorkDlg = new NetWorkDlg(this);
	pAddCamDlg = new CAddCamDlg(this);
	pCamManager = new CameraManager;
	pCamManager->Initial();

	pUpdateTread = new CUpdateDataThread(this);

	

	pAddCamDlg->Create(IDD_ADDCAM_DLG, this);
	m_tab.InsertItem(0, _T("相机"));
	m_tab.InsertItem(1, _T("网络"));
	pCamListDlg->Create(IDD_CAMERASINFO_DLG, &m_tab);
	pNetWorkDlg->Create(IDD_MESSAGEINFO_DLG, &m_tab);
	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	pCamListDlg->MoveWindow(&rc);
	pNetWorkDlg->MoveWindow(&rc);
	pDialog[0] = pCamListDlg;
	pDialog[1] = pNetWorkDlg;
	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	m_CurSelTab = 0;

	pUpdateTread->Initial();












	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCameraManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCameraManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCameraManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nID)
	{
	case SC_MINIMIZE:
		HideToTray();
		break;
	case SC_CLOSE:
		if (::MessageBox(NULL,L"确定要退出吗?", L"程序退出", MB_YESNO) == IDYES)
		{
			//添加卸载程序
			CDialog::OnSysCommand(nID, lParam);
		}
		break;
	default:
		CDialog::OnSysCommand(nID, lParam);
		break;
	}
}

void CCameraManagerDlg::OnTcnSelchangeFunctionTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	//把当前的页面隐藏起来
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//得到新的页面索引
	m_CurSelTab = m_tab.GetCurSel();
	//把新的页面显示出来
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;

}

void CCameraManagerDlg::HideToTray()
{
	NOTIFYICONDATA   nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWFROMTRAY;//自定义的消息名称 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(nid.szTip,128, L"计划任务提醒 ");//信息提示条为“计划任务提醒” 
	Shell_NotifyIcon(NIM_ADD, &nid);//在托盘区添加图标 
	ShowWindow(SW_HIDE);//隐藏主窗口 
}
LRESULT CCameraManagerDlg::OnShowFromTray(WPARAM   wParam, LPARAM   lParam)
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
{
	if (wParam != IDR_MAINFRAME)
		return   1;
	CPoint pt;
    CCameraManagerApp* app = (CCameraManagerApp*)AfxGetApp();
	switch (lParam) {
	case WM_RBUTTONUP:
	{ // Let's track a popup menu
		GetCursorPos(&pt);
		HMENU hmenu = LoadMenu(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_TRAYMENU));
		HMENU hpopup = GetSubMenu(hmenu, 0);
		switch (TrackPopupMenu(hpopup,            // Popup menu to track
			TPM_RETURNCMD |    // Return menu code
			TPM_RIGHTBUTTON,   // Track right mouse button?
			pt.x, pt.y,        // screen coordinates
			0,                 // reserved
			m_hWnd,            // owner
			NULL))             // LPRECT user can click in
							   // without dismissing menu
		{
		case ID_TRYMENU_OPEN: //OpenWindow(hwnd); break;
			ShowWindow(SW_SHOWDEFAULT);
			break;
		case ID_TRYMENU_SET:
			ShowWindow(SW_SHOWDEFAULT);
			break;
		case ID_TRYMENU_EXIT:
			//卸载部分
			if (::MessageBox(NULL, L"确定要退出吗?", L"程序退出", MB_YESNO) == IDYES)
			{
				//添加卸载程序

				CDialog::OnOK();
			}
			break;
		default:
			break;
		}
	}
	case WM_LBUTTONDBLCLK:
		ShowWindow(SW_SHOWDEFAULT);
	}
	return 0;
}