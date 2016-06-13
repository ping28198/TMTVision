
// UserRegisterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserRegister.h"
#include "UserRegisterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CUserRegisterDlg �Ի���



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


// CUserRegisterDlg ��Ϣ�������

BOOL CUserRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUserRegisterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUserRegisterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUserRegisterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_UserName.IsEmpty())
	{
		MessageBox(L"�û���Ϊ�գ�");
		return;
	}
	if (m_PassWord.IsEmpty())
	{
		MessageBox(L"���벻��Ϊ�գ�");
		return;
	}
	if (m_PassWord!=m_PassWordConfirm)
	{
		MessageBox(L"���벻һ�£�");
		return;
	}
	if (m_AuthorizedCode.IsEmpty())
	{
		MessageBox(L"��Ȩ�벻��Ϊ�գ�");
		return;
	}
	if (m_AuthorizedCode!=L"tmta7e36fw9mh5c")
	{
		MessageBox(L"��Ȩ�����");
		return;
	}
	if (m_UserName.GetLength()>18 || m_PassWord.GetLength()>18)
	{
		MessageBox(L"�û��������������");
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
			MessageBox(L"�û����Ѵ��ڣ������Ѹ��£�");
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
	MessageBox(L"��ӳɹ���");
}
