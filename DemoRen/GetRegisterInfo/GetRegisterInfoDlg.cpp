
// GetRegisterInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GetRegisterInfo.h"
#include "GetRegisterInfoDlg.h"
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


// CGetRegisterInfoDlg �Ի���



CGetRegisterInfoDlg::CGetRegisterInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GETREGISTERINFO_DIALOG, pParent)
	, mRegisterInfo(_T(""))
	, mRegistCode(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetRegisterInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mRegisterInfo);
	DDX_Text(pDX, IDC_REGISTER_CODE, mRegistCode);
}

BEGIN_MESSAGE_MAP(CGetRegisterInfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGetRegisterInfoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CGetRegisterInfoDlg ��Ϣ�������

BOOL CGetRegisterInfoDlg::OnInitDialog()
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


	string mMac = CipherCode::GetAvailableNetMac();
	LONGSTR mMacstr;
	strcpy_s(mMacstr, TMTV_LONGSTRLEN, mMac.c_str());
	LONGWSTR mwMac;
	CCommonFunc::AnsiToUnicode(mMacstr, mwMac, TMTV_LONGSTRLEN);
	this->mRegisterInfo = mwMac;
	UpdateData(FALSE);




	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGetRegisterInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGetRegisterInfoDlg::OnPaint()
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
HCURSOR CGetRegisterInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGetRegisterInfoDlg::OnBnClickedButton1()
{
	TmtCrypter mCrypt;

	/*wchar_t* wstr1 = mRegisterInfo.GetBuffer(TMTV_LONGSTRLEN);
	char str1[TMTV_LONGSTRLEN] = { 0 };
	CCommonFunc::UnicodeToAnsi(wstr1, str1, TMTV_LONGSTRLEN);
	string mcrp = mCrypt.AESEncrypt(string(str1));

	string mcryphex = CipherCode::Chars2Hex(mcrp);
	string mcrychar = CipherCode::Hex2Chars(mcryphex);
*/










	UpdateData();
	wchar_t* mwStr = mRegistCode.GetBuffer(TMTV_LONGSTRLEN);
	char mstr[TMTV_LONGSTRLEN] = { 0 };
	CCommonFunc::UnicodeToAnsi(mwStr, mstr, TMTV_LONGSTRLEN);
	string mcode = CipherCode::Hex2Chars(string(mstr));
	
	string rigmac = mCrypt.AESDecrypt(mcode);
	if (!CipherCode::CompareNetMac(rigmac))
	{
		MessageBox(L"ע��ʧ�ܣ�");
		return;
	}
	bool b= CMemoryFile::WriteMemoryToFile_W(mcode.c_str(), mcode.length(), L"setting\\registerinfo.data");
	if (b)
	{
		MessageBox(L"ע��ɹ���");
	}
	else
	{
		MessageBox(L"д��ע����Ϣʧ�ܣ�");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
