
// DatabaseManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DatabaseManagerApp.h"
#include "DatabaseManagerDlg.h"
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


// CDatabaseManagerDlg �Ի���



CDatabaseManagerDlg::CDatabaseManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DATABASEMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDatabaseManagerDlg::UpdateDbData()
{
	vector<Tmtv_CameraInfo> mCamVec;
	vector<Tmt_ClientInfo> mClientVec;
	vector<Tmtv_CameraInfo>::iterator itcam;
	vector<Tmt_ClientInfo>::iterator itclt;
	m_pDbmanager->GetAllCamInfoFrmDb(mCamVec);
	m_pDbmanager->GetAllClientInfoFrmDb(mClientVec);
	int nRow = 0;
	int mindex = 0;
	CListCtrl* pList = &m_CamStatusList;
	pList->DeleteAllItems();
	PATHWSTR wstr;
	for (itcam = mCamVec.begin(); itcam != mCamVec.end();itcam++)
	{
		swprintf_s(wstr, L"%d", itcam->Indexnum);
		nRow = pList->InsertItem(mindex, wstr);
		CCommonFunc::AnsiToUnicode(itcam->CameraName, wstr, TMTV_PATHSTRLEN);
		pList->SetItemText(nRow, 1, wstr);
		CCommonFunc::AnsiToUnicode(itcam->CameraPath, wstr, TMTV_PATHSTRLEN);
		pList->SetItemText(nRow, 2, wstr);
		CCommonFunc::AnsiToUnicode(itcam->CameraHost, wstr, TMTV_PATHSTRLEN);
		pList->SetItemText(nRow, 3, wstr);
		swprintf_s(wstr, L"%f",itcam->CameraPos[0]);
		pList->SetItemText(nRow, 4, wstr);
		swprintf_s(wstr, L"%f", itcam->CameraPos[1]);
		pList->SetItemText(nRow, 5, wstr);
		swprintf_s(wstr, L"%d", itcam->AlgorithmInfo.WarnningLevel);
		pList->SetItemText(nRow, 6, wstr);
		switch (itcam->Status)
		{
		case Tmtv_CameraInfo::TMTV_RUNNINGCAM:
			swprintf_s(wstr, L"����");
			break;
		case Tmtv_CameraInfo::TMTV_STOPEDCAM:
			swprintf_s(wstr, L"ֹͣ");
			break;
		default:
			swprintf_s(wstr, L"δ֪");
			break;
		}
		pList->SetItemText(nRow, 7, wstr);
		CCommonFunc::AnsiToUnicode(itcam->AlgorithmInfo.MaskImgPath, wstr, TMTV_PATHSTRLEN);
		pList->SetItemText(nRow, 8, wstr);
		switch (itcam->AlgorithmInfo.mAlgoStatus)
		{
		case Tmtv_AlgorithmInfo::TMTV_STARTWARN:
			swprintf_s(wstr, L"����");
			break;
		case Tmtv_AlgorithmInfo::TMTV_NOWARN:
			swprintf_s(wstr, L"ֹͣ");
			break;
		case Tmtv_AlgorithmInfo::TMTV_PREWARN:
			swprintf_s(wstr, L"������");
			break;
		default:
			swprintf_s(wstr, L"δ֪");
			break;
		}
		pList->SetItemText(nRow, 9, wstr);
		mindex++;
	}
	nRow = 0;
	mindex = 0;
	pList = &m_ClientList;
	pList->DeleteAllItems();
	for (itclt = mClientVec.begin(); itclt != mClientVec.end(); itclt++)
	{
		swprintf_s(wstr, L"%d", itclt->ClientID);
		nRow = pList->InsertItem(mindex, wstr);
		CCommonFunc::AnsiToUnicode(itclt->mIpAddr, wstr, TMTV_IPSTRLEN);
		pList->SetItemText(nRow, 1, wstr);
		swprintf_s(wstr, L"%d", itclt->mport);
		pList->SetItemText(nRow, 2, wstr);
		switch (itclt->status)
		{
		case Tmt_ClientInfo::TMT_CLIENT_RUNNING:
			swprintf_s(wstr, L"����");
			break;
		case Tmt_ClientInfo::TMT_CLIENT_CLOSE:
			swprintf_s(wstr, L"����");
			break;
		default:
			swprintf_s(wstr, L"����");
			break;
		}
		pList->SetItemText(nRow, 3, wstr);
	}
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


// CDatabaseManagerDlg ��Ϣ�������

BOOL CDatabaseManagerDlg::OnInitDialog()
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
	DbServerLogger::mLogger.TraceKeyInfo("start------------------------------------");
	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	LONG mStyle;
	mStyle = GetWindowLong(m_CamStatusList.m_hWnd, GWL_STYLE);//��ȡ��ǰ���ڷ�
	mStyle &= ~LVS_TYPEMASK;
	mStyle |= LVS_REPORT;
	SetWindowLong(m_CamStatusList.m_hWnd, GWL_STYLE, mStyle);
	DWORD dwStyle = m_CamStatusList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ�����뱨�����listctrl��
	m_CamStatusList.SetExtendedStyle(dwStyle); //������չ��ʽ

	m_CamStatusList.InsertColumn(0, _T("���ID"), LVCFMT_CENTER, 50);        //����б��� 
	m_CamStatusList.InsertColumn(1, _T("�������"), LVCFMT_CENTER, 70);
	m_CamStatusList.InsertColumn(2, _T("���·��"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(3, _T("������ַ"), LVCFMT_CENTER, 80);
	m_CamStatusList.InsertColumn(4, _T("γ������"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(5, _T("��������"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(6, _T("����ȼ�"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(7, _T("���״̬"), LVCFMT_CENTER, 60);
	m_CamStatusList.InsertColumn(8, _T("����ͼ��λ��"), LVCFMT_CENTER, 70);
	m_CamStatusList.InsertColumn(9, _T("�㷨״̬"), LVCFMT_CENTER, 60);


	mStyle = GetWindowLong(m_ClientList.m_hWnd, GWL_STYLE);//��ȡ��ǰ���ڷ�
	mStyle &= ~LVS_TYPEMASK;
	mStyle |= LVS_REPORT;
	SetWindowLong(m_ClientList.m_hWnd, GWL_STYLE, mStyle);
	dwStyle = m_ClientList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ�����뱨�����listctrl��
	m_ClientList.SetExtendedStyle(dwStyle); //������չ��ʽ

	m_ClientList.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 50);
	m_ClientList.InsertColumn(1, _T("������ַ"), LVCFMT_CENTER, 90);
	m_ClientList.InsertColumn(2, _T("�˿�"), LVCFMT_CENTER, 50);
	m_ClientList.InsertColumn(3, _T("״̬"), LVCFMT_CENTER, 50);






	m_pDbmanager = new CDatabaseManager(this);
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


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDatabaseManagerDlg::OnPaint()
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
HCURSOR CDatabaseManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDatabaseManagerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}
