
// UserRegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UserRegister.h"
#include "UserRegisterDlg.h"
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


// CUserRegisterDlg 对话框



CUserRegisterDlg::CUserRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USERREGISTER_DIALOG, pParent)
	, m_UserName(_T(""))
	, m_PassWord(_T(""))
	, m_PassWordConfirm(_T(""))
	, m_AuthorizedCode(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUserRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_UserName);
	DDX_Text(pDX, IDC_EDIT2, m_PassWord);
	DDX_Text(pDX, IDC_EDIT3, m_PassWordConfirm);
	DDX_Text(pDX, IDC_EDIT4, m_AuthorizedCode);
}

BEGIN_MESSAGE_MAP(CUserRegisterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUserRegisterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserRegisterDlg 消息处理程序

BOOL CUserRegisterDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUserRegisterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUserRegisterDlg::OnPaint()
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
HCURSOR CUserRegisterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUserRegisterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_UserName.IsEmpty())
	{
		MessageBox(L"用户名为空！");
		return;
	}
	if (m_PassWord.IsEmpty())
	{
		MessageBox(L"密码不能为空！");
		return;
	}
	if (m_PassWord!=m_PassWordConfirm)
	{
		MessageBox(L"密码不一致！");
		return;
	}
	if (m_AuthorizedCode.IsEmpty())
	{
		MessageBox(L"授权码不能为空！");
		return;
	}
	if (m_AuthorizedCode!=L"tmta7e36fw9mh5c")
	{
		MessageBox(L"授权码错误！");
		return;
	}
	if (m_UserName.GetLength()>18 || m_PassWord.GetLength()>18)
	{
		MessageBox(L"用户名或密码过长！");
		return;
	}
	LONGSTR username;
	LONGSTR password;
	CCommonFunc::UnicodeToAnsi(m_UserName.GetBuffer(), username, TMTV_LONGSTRLEN);
	CCommonFunc::UnicodeToAnsi(m_PassWord.GetBuffer(), password, TMTV_LONGSTRLEN);
	string usernamestr(username);
	string passwordstr(password);
	string EncrptUserName = mCrypt.AESEncrypt(usernamestr);
	string EncrptPassword = mCrypt.AESEncrypt(passwordstr);
	Tmt_UserInfo mUserInfo;
	strcpy_s(mUserInfo.UserName, TMTV_LONGSTRLEN, EncrptUserName.c_str());
	strcpy_s(mUserInfo.PassWord, TMTV_LONGSTRLEN, EncrptPassword.c_str());
	//int backcode= mCfgOp.WriteConfig("setting\\user.data",EncrptUserName.c_str(),EncrptPassword.c_str());
	vector<Tmt_UserInfo> mUserVec;
	Tmt_UserInfo tmpUserInfo;
	CMemoryFile mMemFile;
	mMemFile.OpenFile_R(L"setting\\user.data");
	bool IsExist=false;
	while (mMemFile.ReadMemoryFromFile((&tmpUserInfo),sizeof(Tmt_UserInfo)))
	{
		if (strcmp(tmpUserInfo.UserName, mUserInfo.UserName)==0)
		{
			MessageBox(L"用户名已存在，密码已更新！");
			mUserVec.push_back(mUserInfo);
			IsExist = true;
			continue;
		}
		mUserVec.push_back(tmpUserInfo);
	}
	mMemFile.CloseFile();
	if (!IsExist)
	{
		mUserVec.push_back(mUserInfo);
	}
	mMemFile.OpenFile_W(L"setting\\user.data");
	vector<Tmt_UserInfo>::iterator it;
	for (it = mUserVec.begin(); it != mUserVec.end();it++)
	{
		mMemFile.WriteMemoryToFile(&(*it), sizeof(Tmt_UserInfo));
	}
	mMemFile.CloseFile();
	MessageBox(L"添加成功！");
}
