
// USBCameraDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "USBCameraApp.h"
#include "USBCameraDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_SHOWFROMTRAY (WM_USER+10)

// CUSBCameraDlg 对话框


CUSBCameraDlg::CUSBCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_USBCAMERA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUSBCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAM_STC, m_CamStc);
	DDX_Control(pDX, IDC_CAM_COB, m_CamCob);
	DDX_Control(pDX, IDC_CONTROL_STC, m_ControlStc);
	DDX_Control(pDX, IDC_STARTCAM_BTN, m_StartCamBtn);
	DDX_Control(pDX, IDC_STOPCAM_BTN, m_StopCamBtn);
	DDX_Control(pDX, IDC_UPLEVEL_BTN, m_UplevelBtn);
	DDX_Control(pDX, IDC_DOWNLEVEL_BTN, m_DownLevelBtn);
	DDX_Control(pDX, IDC_CAMPATH_STC, m_CamPathStc);
	DDX_Control(pDX, IDC_CAMPATH_EDT, m_CamPathEdt);
	DDX_Control(pDX, IDC_SETCAMPATH_BTN, m_SetCamPathBtn);
	DDX_Control(pDX, IDC_CAPTIME_STC, m_CapTimeStc);
	DDX_Control(pDX, IDC_CAPTIME_EDT, m_CapTimeEdt);
	DDX_Control(pDX, IDC_CAPNUM_STC, m_CapNumStc);
	DDX_Control(pDX, IDC_CAPNUM_EDT, m_CapNumEdt);
	DDX_Control(pDX, IDC_SHOWIMG_BTN, m_ShowImgBtn);
	DDX_Control(pDX, IDC_IMAGE_PAN, m_ImagePan);
}

BEGIN_MESSAGE_MAP(CUSBCameraDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SHOWFROMTRAY, OnShowFromTray)
	ON_CBN_SELCHANGE(IDC_CAM_COB, &CUSBCameraDlg::OnCbnSelchangeCamCob)
	ON_BN_CLICKED(IDC_STARTCAM_BTN, &CUSBCameraDlg::OnBnClickedStartcamBtn)
	ON_BN_CLICKED(IDC_STOPCAM_BTN, &CUSBCameraDlg::OnBnClickedStopcamBtn)
	ON_BN_CLICKED(IDC_UPLEVEL_BTN, &CUSBCameraDlg::OnBnClickedUplevelBtn)
	ON_BN_CLICKED(IDC_DOWNLEVEL_BTN, &CUSBCameraDlg::OnBnClickedDownlevelBtn)
	ON_BN_CLICKED(IDC_SETCAMPATH_BTN, &CUSBCameraDlg::OnBnClickedSetcampathBtn)
	ON_BN_CLICKED(IDC_SHOWIMG_BTN, &CUSBCameraDlg::OnBnClickedShowimgBtn)
END_MESSAGE_MAP()


// CUSBCameraDlg 消息处理程序

BOOL CUSBCameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 初始化控件
	//m_ImagePan.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 20, 20), this, IDC_IMAGE_PAN);
	m_ImagePan.SetMovable(false);
	// 获取窗口外框
	SetWindowPos(NULL, 0, 0, 4 * CONTROLHEIGHT, 4 * CONTROLHEIGHT, SWP_NOMOVE);
	CRect rect;
	GetClientRect(&rect);
	m_DiffWndWidth = 4 * CONTROLHEIGHT - rect.Width();
	m_DiffWndHeight = 4 * CONTROLHEIGHT - rect.Height();
	// 设置控件位置
	SetSize(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUSBCameraDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nID)
	{
	case SC_MINIMIZE:
		HideToTray();
		break;
	case SC_CLOSE:
		if (::MessageBox(NULL, L"确定要退出吗?", L"程序退出", MB_YESNO) == IDYES)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUSBCameraDlg::OnPaint()
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
HCURSOR CUSBCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUSBCameraDlg::HideToTray()
{
	NOTIFYICONDATA   nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWFROMTRAY;//自定义的消息名称 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(nid.szTip, 128, L"计划任务提醒 ");//信息提示条为“计划任务提醒” 
	Shell_NotifyIcon(NIM_ADD, &nid);//在托盘区添加图标 
	ShowWindow(SW_HIDE);//隐藏主窗口 
}

LRESULT CUSBCameraDlg::OnShowFromTray(WPARAM   wParam, LPARAM   lParam)
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
{
	if (wParam != IDR_MAINFRAME)
		return   1;
	CPoint pt;
	CUSBCameraApp* app = (CUSBCameraApp*)AfxGetApp();
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

void CUSBCameraDlg::OnBnClickedStartcamBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUSBCameraDlg::OnBnClickedStopcamBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUSBCameraDlg::OnBnClickedUplevelBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUSBCameraDlg::OnBnClickedDownlevelBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUSBCameraDlg::OnBnClickedSetcampathBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUSBCameraDlg::OnCbnSelchangeCamCob()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUSBCameraDlg::OnBnClickedShowimgBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	SetSize(!m_Maxed);
	Update();
}

void CUSBCameraDlg::SetSize(bool maxed, int imageCtrlWidth, int imageCtrlHeight)
{
	m_Maxed = maxed;
	m_ImageCtrlWidth = max(imageCtrlWidth, CONTROLHEIGHT * 12);
	m_ImageCtrlHeight = max(imageCtrlHeight, CONTROLHEIGHT * 12);

	m_MinRect.top = 0;
	m_MinRect.left = 0;
	m_MinRect.bottom = m_MinRect.top + 4 * BORDERWIDTH + 3 * CONTROLHEIGHT;
	m_MinRect.right = m_MinRect.left + 2 * BORDERWIDTH + m_ImageCtrlWidth;
	m_MaxRect = m_MinRect;
	m_MaxRect.bottom = m_MaxRect.bottom + BORDERWIDTH + m_ImageCtrlHeight;

	CRect m_TmpRect = m_MinRect;
	m_TmpRect.top = m_TmpRect.top + BORDERWIDTH;
	m_TmpRect.bottom = m_TmpRect.top + CONTROLHEIGHT;
	m_TmpRect.left = m_TmpRect.left + BORDERWIDTH;
	m_TmpRect.right = m_TmpRect.left + 2 * CONTROLHEIGHT;
	m_CamStc.MoveWindow(m_TmpRect);

	CRect m_TmpRectR = m_MinRect;
	m_TmpRectR.top = m_TmpRectR.top + BORDERWIDTH;
	m_TmpRectR.bottom = m_TmpRectR.top + CONTROLHEIGHT;
	m_TmpRectR.right = m_TmpRectR.right - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRectR.right - CONTROLHEIGHT;
	m_DownLevelBtn.MoveWindow(m_TmpRectR);

	m_TmpRectR.right = m_TmpRectR.left - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRectR.right - CONTROLHEIGHT;
	m_UplevelBtn.MoveWindow(m_TmpRectR);

	m_TmpRectR.right = m_TmpRectR.left - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRectR.right - CONTROLHEIGHT;
	m_StopCamBtn.MoveWindow(m_TmpRectR);

	m_TmpRectR.right = m_TmpRectR.left - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRectR.right - CONTROLHEIGHT;
	m_StartCamBtn.MoveWindow(m_TmpRectR);

	m_TmpRectR.right = m_TmpRectR.left - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRectR.right - 2 * CONTROLHEIGHT;
	m_ControlStc.MoveWindow(m_TmpRectR);

	m_TmpRectR.right = m_TmpRectR.left - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRect.right + BORDERWIDTH;
	m_CamCob.MoveWindow(m_TmpRectR);


	m_TmpRect = m_MinRect;
	m_TmpRect.top = m_TmpRect.top + 2* BORDERWIDTH + CONTROLHEIGHT;
	m_TmpRect.bottom = m_TmpRect.top + CONTROLHEIGHT;
	m_TmpRect.left = m_TmpRect.left + BORDERWIDTH;
	m_TmpRect.right = m_TmpRect.left + 2 * CONTROLHEIGHT;
	m_CamPathStc.MoveWindow(m_TmpRect);

	m_TmpRectR = m_MinRect;
	m_TmpRectR.top = m_TmpRectR.top + 2 * BORDERWIDTH + CONTROLHEIGHT;
	m_TmpRectR.bottom = m_TmpRectR.top + CONTROLHEIGHT;
	m_TmpRectR.right = m_TmpRectR.right - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRectR.right - 3 * CONTROLHEIGHT;
	m_SetCamPathBtn.MoveWindow(m_TmpRectR);

	m_TmpRectR.right = m_TmpRectR.left - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRect.right + BORDERWIDTH;
	m_CamPathEdt.MoveWindow(m_TmpRectR);


	m_TmpRect = m_MinRect;
	m_TmpRect.top = m_TmpRect.top + 3 * BORDERWIDTH + 2 * CONTROLHEIGHT;
	m_TmpRect.bottom = m_TmpRect.top + CONTROLHEIGHT;
	m_TmpRect.left = m_TmpRect.left + BORDERWIDTH;
	m_TmpRect.right = m_TmpRect.left + 2 * CONTROLHEIGHT;
	m_CapTimeStc.MoveWindow(m_TmpRect);

	m_TmpRectR = m_MinRect;
	m_TmpRectR.top = m_TmpRectR.top + 3 * BORDERWIDTH + 2 * CONTROLHEIGHT;
	m_TmpRectR.bottom = m_TmpRectR.top + CONTROLHEIGHT;
	m_TmpRectR.right = m_TmpRectR.right - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRectR.right - 4 * CONTROLHEIGHT;
	m_ShowImgBtn.MoveWindow(m_TmpRectR);

	m_TmpRectR.right = m_TmpRectR.left - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRectR.right - 1.5 * CONTROLHEIGHT;
	m_CapNumEdt.MoveWindow(m_TmpRectR);

	m_TmpRectR.right = m_TmpRectR.left - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRectR.right - 2.5 * CONTROLHEIGHT;
	m_CapNumStc.MoveWindow(m_TmpRectR);

	m_TmpRectR.right = m_TmpRectR.left - BORDERWIDTH;
	m_TmpRectR.left = m_TmpRect.right + BORDERWIDTH;
	m_CapTimeEdt.MoveWindow(m_TmpRectR);


	//CImageWnd m_ImagePan;
	if (m_Maxed)
	{
		SetWindowPos(NULL, 0, 0, m_MaxRect.Width() + m_DiffWndWidth, m_MaxRect.Height() + m_DiffWndHeight, SWP_NOMOVE);
		m_TmpRect = m_MaxRect;
		m_TmpRect.top = m_MinRect.bottom;
		m_TmpRect.bottom = m_TmpRect.bottom - BORDERWIDTH;
		m_TmpRect.left = m_TmpRect.left + BORDERWIDTH;
		m_TmpRect.right = m_TmpRect.right  - BORDERWIDTH;
		m_ImagePan.MoveWindow(m_TmpRect);
		m_ImagePan.ShowWindow(TRUE);
	}
	else
	{
		SetWindowPos(NULL, 0, 0, m_MinRect.Width() + m_DiffWndWidth, m_MinRect.Height() + m_DiffWndHeight, SWP_NOMOVE);
		m_ImagePan.ShowWindow(FALSE);
	}
}

void CUSBCameraDlg::Update()
{
	//m_ImagePan.LoadImageFromImage(USBCameraState::m_CameraInfo.);
	m_ImagePan.ReDraw();
}