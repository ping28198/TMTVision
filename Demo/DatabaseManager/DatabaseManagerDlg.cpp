
// DatabaseManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DatabaseManagerApp.h"
#include "DatabaseManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDatabaseManagerDlg 对话框



CDatabaseManagerDlg::CDatabaseManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DATABASEMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDatabaseManagerDlg::UpdateDbData()
{




}

void CDatabaseManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMSTATUS_LIST, m_CamStatusList);
	DDX_Control(pDX, IDC_CLIENT_LIST, m_ClientList);
}

BEGIN_MESSAGE_MAP(CDatabaseManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDatabaseManagerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDatabaseManagerDlg 消息处理程序

BOOL CDatabaseManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	DbServerLogger::mLogger.TraceKeyInfo("start------------------------------------");
	// TODO: 在此添加额外的初始化代码


	LONG mStyle;
	mStyle = GetWindowLong(m_CamStatusList.m_hWnd, GWL_STYLE);//获取当前窗口风
	mStyle &= ~LVS_TYPEMASK;
	mStyle |= LVS_REPORT;
	SetWindowLong(m_CamStatusList.m_hWnd, GWL_STYLE, mStyle);
	DWORD dwStyle = m_CamStatusList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）
	m_CamStatusList.SetExtendedStyle(dwStyle); //设置扩展样式

	m_CamStatusList.InsertColumn(0, _T("相机号"), LVCFMT_CENTER, 50);        //添加列标题 
	m_CamStatusList.InsertColumn(1, _T("相机描述"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(2, _T("相机位置"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(3, _T("主机地址"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(4, _T("坐标lat"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(5, _T("坐标lng"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(6, _T("警告等级"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(7, _T("相机状态"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(8, _T("掩码图像位置"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(9, _T("算法状态"), LVCFMT_CENTER, 60);


	mStyle = GetWindowLong(m_ClientList.m_hWnd, GWL_STYLE);//获取当前窗口风
	mStyle &= ~LVS_TYPEMASK;
	mStyle |= LVS_REPORT;
	SetWindowLong(m_ClientList.m_hWnd, GWL_STYLE, mStyle);
	dwStyle = m_ClientList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）
	m_ClientList.SetExtendedStyle(dwStyle); //设置扩展样式

	m_ClientList.InsertColumn(0, _T("主机地址"), LVCFMT_CENTER, 50);
	m_ClientList.InsertColumn(1, _T("端口"), LVCFMT_CENTER, 70);
	m_ClientList.InsertColumn(2, _T("状态"), LVCFMT_CENTER, 50);






	m_pDbmanager = new CDatabaseManager;
	m_pDbmanager->Initial();
	//Tmtv_CameraInfo mCam;
	//mCam.Indexnum = 3;
	//sprintf(mCam.CameraName, "name0");
	//sprintf(mCam.CameraPath, "d:\\aB");
	//sprintf(mCam.CameraHost, "192.168.1.1");
	//mCam.CameraPos[0] = 11;
	//mCam.CameraPos[1] = 22;
	//sprintf(mCam.AlgorithmInfo.MaskImgPath, "d:\\a\\b");
	//bool bb = m_Dbmanager.AddCamToDb(mCam);
	//if (bb)
	//{
	//	DbServerLogger::mLogger.TraceKeyInfo("cam insert ok");
	//}
	//else
	//{
	//	DbServerLogger::mLogger.TraceKeyInfo("cam insert fail");
	//}
	//mCam.Indexnum = 2;
	//bool a = m_Dbmanager.CheckCamInfo(mCam);
	////m_Dbmanager.GetClientInfoFrmDb();
	//Tmt_ClientInfo mclient;
	//sprintf(mclient.mIpAddr, "192.168.0.1");
	//mclient.mport = 5012;
	//mclient.status = 0;
	//m_Dbmanager.UpdateClientStatusDb(mclient);
	//a = m_Dbmanager.UpdateCamInfoDb(mCam);

	//Tmtv_ImageInfo mimg;
	//sprintf(mimg.GrabTime, "2016-04-20 12:23:20");
	//sprintf(mimg.ImagePath, "d:\\a");
	//mimg.mCamId = 2;
	//mimg.mDefectInfo.DefectNum = 1;
	//mimg.mDefectInfo.DefectPos[0][0] = 100;
	//mimg.mDefectInfo.DefectPos[0][1] = 200;
	//mimg.mDefectInfo.DefectPos[0][2] = 300;
	//mimg.mDefectInfo.DefectPos[0][3] = 400;
	//a = m_Dbmanager.InsertImgInfoToDb(mimg);
	//Tmtv_ImageInfo mimg2;
	//mimg2.mCamId = 2;
	//a = m_Dbmanager.GetLastImgInfoFrmDb(mimg2);

	//vector<Tmtv_ImageInfo> mimgvec;
	//m_Dbmanager.GetImginfoByDate(mimgvec, 2, "2016-04-20");
	//m_Dbmanager.DisConnectDb();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDatabaseManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDatabaseManagerDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDatabaseManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDatabaseManagerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
