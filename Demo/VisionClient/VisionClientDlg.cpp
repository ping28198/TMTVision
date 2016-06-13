
// VisionClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VisionClient.h"
#include "VisionClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

#define DLG_LF_MARGIN 270
#define DLG_TP_MARGIN 108



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


// CVisionClientDlg �Ի���



CVisionClientDlg::CVisionClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VISIONCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pImgBmp = NULL;
}

void CVisionClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CAMERA, mListCamera);
}



BEGIN_MESSAGE_MAP(CVisionClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADDCAM_BT, &CVisionClientDlg::OnBnClickedAddcamBt)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CVisionClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_ISROLLING_BT, &CVisionClientDlg::OnBnClickedIsrollingBt)
	ON_BN_CLICKED(IDC_HISTIMG_BT, &CVisionClientDlg::OnBnClickedHistimgBt)
	ON_BN_CLICKED(IDC_SHNEWIMG_BT, &CVisionClientDlg::OnBnClickedShnewimgBt)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON2, &CVisionClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_USER_LOGIN_BT, &CVisionClientDlg::OnBnClickedUserLoginBt)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON3, &CVisionClientDlg::OnBnClickedButton3)

	ON_MESSAGE(WM_ASK_RESULT, OnAskResult)

END_MESSAGE_MAP()


// CVisionClientDlg ��Ϣ�������

BOOL CVisionClientDlg::OnInitDialog()
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

	ShowWindow(SW_MAXIMIZE);
	//VisionPublicSet::mLogger.TraceInfo("----------------------------------------��ʼ��");

	//if (CipherCode::CheckRegistrInfo(L"setting\\registerinfo.data") != true)
	//{
	//	MessageBox(L"���δע�ᣡ");
	//	CDialog::OnCancel();
	//}
	VisionClientLogger::mLogger.TraceKeyInfo("Start-----------------------------------------------");

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (pImgBmp == NULL) pImgBmp = new CBitmap;
	pBmpDC = new CDC;
	//pNetWorkServer = new CNetWorkServer(this);
	//pRunState = new CVisionRunState(this);
	pVisionRun = new CVisionRun(this);
	pAddCamDlg = NULL;
	m_IsRolling = true;
	pZoomImgDlg = new CZoomImgDlg(this);
	m_IsLogin = false;


	pVisionRun->Initial();


	//CipherCode mCipCode;
	//string mstr = mCipCode.GetAvailableNetMac();
	//string hex = mCipCode.Chars2Hex(mstr);
	//string mstr2 = mCipCode.Hex2Chars(hex);

	//bool m = mCipCode.CompareNetMac(mstr2);

	InitializeCriticalSection(&cs);
	pBmpDC->CreateCompatibleDC(NULL);
	LONG mStyle;
	mStyle = GetWindowLong(mListCamera.m_hWnd, GWL_STYLE);//��ȡ��ǰ���ڷ�
	mStyle &= ~LVS_TYPEMASK;
	mStyle |= LVS_REPORT;
	SetWindowLong(mListCamera.m_hWnd, GWL_STYLE, mStyle);
	DWORD dwStyle = mListCamera.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_CHECKBOXES;
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ�����뱨�����listctrl��
	mListCamera.SetExtendedStyle(dwStyle); //������չ���

	//mListCamera.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);
	mListCamera.InsertColumn(0, _T("�ص�"), LVCFMT_CENTER, 40);        //����б��� 
	mListCamera.InsertColumn(1, _T("�����"), LVCFMT_LEFT, 50);
	mListCamera.InsertColumn(2, _T("�����Ϣ"), LVCFMT_LEFT, 250);

	//mt = imread("D:\\aa.jpg");
	InitConfig();
	//pRunState->Resume();
	//pNetWorkServer->Resume();
	LoadCamInfo();
	ShowCamInfoToList();
	pZoomImgDlg->Create(IDD_ZOOMIMG_DLG, this);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVisionClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVisionClientDlg::OnPaint()
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
		//InitConfig();
		ShowMemBmpToScreen();
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVisionClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVisionClientDlg::DrawImage(Tmtv_ImageInfo& ImgInfo, CRect mRect,CDC* pDC)
{
	CImage img;
	wchar_t imgpath[TMTV_LONGSTRLEN];
	CCommonFunc::AnsiToUnicode(ImgInfo.ImagePath,imgpath,TMTV_LONGSTRLEN);
	//if (!CCommonFunc::FileExist(imgpath)) return;
	Mat mt;
	mt = imread(ImgInfo.ImagePath);
	if (mt.empty()) return;
	cv::Size mSize = cv::Size(mRect.Width(), mRect.Height());
	Mat image2 ;
	resize(mt, image2, mSize);
	//mt.resize(mSize);
	CvImage::Mat2CImage(image2, img);
	if (img.IsNull()) return;
	CDC* pImDC = CDC::FromHandle(img.GetDC());
	CFont font;
	int fontheight = img.GetHeight()*0.05;
	font.CreateFont(
		fontheight,					//height
		0,                            // nWidth
		0,                            // nEscapement
		0,                            // nOrientation
		FW_BOLD,                        // nWeight
		FALSE,                        // bItalic
		FALSE,                        // bUnderline
		0,                            // cStrikeOut
		ANSI_CHARSET,                    // nCharSet
		OUT_DEFAULT_PRECIS,            // nOutPrecision
		CLIP_DEFAULT_PRECIS,            // nClipPrecision
		DEFAULT_QUALITY,                // nQuality
		DEFAULT_PITCH | FF_SWISS,        // nPitchAndFamily
		_T("����")                    // lpszFac
		);
	pImDC->SelectObject(&font);
	pImDC->SetTextColor(RGB(0, 255, 0));
	pImDC->SetBkMode(TRANSPARENT);//д������ͷ�����Ϣ
	CString mstr;
	wchar_t mWstr[256];
	
	mstr.Format(_T("�����:%d"),ImgInfo.mCamId);
	pImDC->TextOut(0,0,mstr);
	Tmtv_CameraInfo mCaminfo;
	mCaminfo.Indexnum = ImgInfo.mCamId;
	GetCam(&mCaminfo);

	CCommonFunc::AnsiToUnicode(mCaminfo.CameraName, mWstr, sizeof(mWstr));
	mstr.Format(_T("%s"), mWstr);
	pImDC->TextOut(0, fontheight, mstr);


	CCommonFunc::AnsiToUnicode(ImgInfo.GrabTime, mWstr, sizeof(mWstr));
	mstr.Format(_T("%s"), mWstr);
	pImDC->TextOut(0, 2*fontheight, mstr);
	int linewidth = img.GetWidth()*0.007;
	if (linewidth < 1) linewidth = 1;
	CPen *pPen=new CPen(PS_SOLID, linewidth,RGB(255,0,0));
	pImDC->SelectObject(pPen);
	//pImDC->Detach();
	//pImDC->StretchBlt()
	//pDC->StretchBlt()
	pImDC->SelectStockObject(NULL_BRUSH);//����һ�������Ļ�ˢ
	if (ImgInfo.mDefectInfo.DefectNum>0)//����ȱ����Ϣ
	{
		for (int i = 0; i < ImgInfo.mDefectInfo.DefectNum;i++)
		{
			CRect mRect;
			mRect.left = (double)ImgInfo.mDefectInfo.DefectPos[i][0] / ImgInfo.mDefectInfo.ImgWidth*img.GetWidth();
			mRect.top = (double)ImgInfo.mDefectInfo.DefectPos[i][1] / ImgInfo.mDefectInfo.ImgHeight*img.GetHeight();
			mRect.right= (double)(ImgInfo.mDefectInfo.DefectPos[i][2]) / ImgInfo.mDefectInfo.ImgWidth*img.GetWidth();
			mRect.bottom = (double)(ImgInfo.mDefectInfo.DefectPos[i][3]) / ImgInfo.mDefectInfo.ImgHeight*img.GetHeight();
			pImDC->Rectangle(mRect);
		}
	}
	pDC->StretchBlt(mRect.left,mRect.top,mRect.Width(),mRect.Height(),pImDC,0,0,img.GetWidth(),img.GetHeight(),SRCCOPY);
	img.ReleaseDC();
	//CDC *pDC = this->GetDC();
	//HDC hDC = pDC->GetSafeHdc();
	//img.Draw(hDC, mRect);
	
	delete pPen;
}

void CVisionClientDlg::DrawAllImage(vector<Tmtv_ImageInfo> &mImgVec, vector<CRect> &mRcVec, CDC* pDC, COLORREF bkcolor)
{
	int num = mImgVec.size()<mRcVec.size()? mImgVec.size(): mRcVec.size();
	if (num <= 0) return;
	int WarnSignWidth = 5;
	for (int i = 0; i < num;i++)
	{
		DrawWarnSign(mImgVec.at(i), mRcVec.at(i), pDC, WarnSignWidth, bkcolor);
		DrawImage(mImgVec.at(i), mRcVec.at(i), pDC);
	}

}

void CVisionClientDlg::AdjustImgRect(int xnum, int ynum, CRect imgRangeRect, vector<CRect> &mRcVec)
{
	int marginwidth = 5;//��ʾͼ��߿�����
	if (imgRangeRect.Width() <= 100 || imgRangeRect.Height() < 100 || xnum<=0 || ynum <= 0)
	{
		mRcVec.clear();
		return;
	}
	int imgwidth = (imgRangeRect.Width()- (xnum+1)*5)/xnum;
	int imgheight = (imgRangeRect.Height() - (ynum + 1) * 5) / ynum;
	if (imgheight < 20 || imgwidth < 20) return;//������ͼ���Ȼ�߶�С��20�����ؾͲ��ٵ���
	CRect Rc;
	mRcVec.clear();
	for (int j = 1; j <= ynum;j++)
	{
		for (int i = 1; i <= xnum;i++)
		{
			Rc.left = i*marginwidth + (i - 1)*imgwidth;
			Rc.right = i*(marginwidth + imgwidth);
			Rc.top = j*marginwidth + (j - 1)*imgheight;
			Rc.bottom = j*(marginwidth + imgheight);
			mRcVec.push_back(Rc);
		}
	}
}

void CVisionClientDlg::DrawWarnSign(Tmtv_ImageInfo &mImg, CRect &mRc, CDC* pDC, int marginwidth, COLORREF bkcolor)
{
	CRect Brect;
	Brect.left = mRc.left - marginwidth;
	Brect.top = mRc.top - marginwidth;
	Brect.right = mRc.right + marginwidth;
	Brect.bottom = mRc.bottom + marginwidth;
	if (mImg.mDefectInfo.DefectNum>0)
	{
		pDC->FillSolidRect(Brect, RGB(255,0,0));
	}
	//else
	//{
	//	pDC->FillSolidRect(Brect, bkcolor);
	//}
}
void CVisionClientDlg::InitConfig()
{
	//�Ի���Ԫ�س�ʼ��
	xImgNum = 3;
	yImgNum = 3;
	CRect mClientRect;
	this->GetClientRect(&mClientRect);
	mImgRect = mClientRect;
	mImgRect.left += DLG_LF_MARGIN;
	mImgRect.top += DLG_TP_MARGIN;
	AdjustImgRect(xImgNum, yImgNum, mImgRect, mRectVec);
	CWnd* pWnd = GetDlgItem(IDC_LIST_CAMERA);
	if (pWnd!=NULL)
	{
		CRect listRect ;
		listRect.left = 5;
		listRect.top = 108;
		listRect.right = 265;
		listRect.bottom = mClientRect.Height();
		pWnd->MoveWindow(&listRect);
	}
	//////////////////////////////////////////////////////////////////////////
	//ͼ�񲿷ֳ�ʼ��
	if (pImgBmp == NULL) pImgBmp = new CBitmap;
	pImgBmp->DeleteObject();
	int b = pImgBmp->CreateCompatibleBitmap(this->GetDC(), mImgRect.Width(), mImgRect.Height());
	if (pBmpDC!=NULL)
	{
		pBmpDC->SelectObject(pImgBmp);
	}
	if (pVisionRun!=NULL)
	{
		pVisionRun->SetShowImgNum(xImgNum*yImgNum);
	}
}

void CVisionClientDlg::DrawImgToMemBmp()
{
	if (pBmpDC == NULL) return;
	COLORREF bkColor = RGB(120,120,120);
	pBmpDC->FillSolidRect(0,0,mImgRect.Width(), mImgRect.Height(),bkColor);
	EnterCriticalSection(&cs);
	this->DrawAllImage(this->mImageShowVec, mRectVec, pBmpDC, bkColor);
	LeaveCriticalSection(&cs);
}

void CVisionClientDlg::ShowMemBmpToScreen()
{
	if (pBmpDC == NULL)return;
	CDC*pDC = this->GetDC();
	pDC->BitBlt(mImgRect.left, mImgRect.top, mImgRect.Width(), mImgRect.Height(), pBmpDC,0,0,SRCCOPY);
}

int CVisionClientDlg::AddCam(Tmtv_CameraInfo *camInfo)
{
	mCamInfoVec.push_back(*camInfo);
	vector<Tmtv_CameraInfo>::iterator it;
	it = mCamInfoVec.begin();
	for (; it != mCamInfoVec.end(); it++)
	{
		if (it->Indexnum==camInfo->Indexnum)
		{
			*it = *camInfo;
		}
	}
	if (it==mCamInfoVec.end())
	{
		mCamInfoVec.push_back(*camInfo);
	}
	ShowCamInfoToList();
	return 0;// pNetWorkServer->AddNewCam(camInfo);
}

bool CVisionClientDlg::GetCam(Tmtv_CameraInfo *camInfo)
{
	vector<Tmtv_CameraInfo>::iterator it;
	it = mCamInfoVec.begin();
	for (; it != mCamInfoVec.end();it++)
	{
		if (camInfo->Indexnum==it->Indexnum)
		{
			*camInfo = *it;
			return true;
		}
	}
	return false;
}

void CVisionClientDlg::ModifyCam(Tmtv_CameraInfo *camInfo)
{
	vector<Tmtv_CameraInfo>::iterator it;
	it = mCamInfoVec.begin();
	for (; it != mCamInfoVec.end();it++)
	{
		if (it->Indexnum==camInfo->Indexnum)
		{
			*it = *camInfo;
		}
	}
	SaveCamInfo();
}

void CVisionClientDlg::DelCam(int camIndex)
{
	vector<Tmtv_CameraInfo>::iterator it;
	it = mCamInfoVec.begin();
	for (; it != mCamInfoVec.end(); it++)
	{
		if (it->Indexnum == camIndex)
		{
			mCamInfoVec.erase(it);
		}
	}
	SaveCamInfo();
}

int CVisionClientDlg::CheckCam(Tmtv_CameraInfo *camInfo)
{
	vector<Tmtv_CameraInfo>::iterator it;
	it = mCamInfoVec.begin();
	for (; it != mCamInfoVec.end(); it++)
	{
		if (it->Indexnum==camInfo->Indexnum)
		{
			return 1;
		}
		if (strcmp(it->CameraName,camInfo->CameraName)==0)
		{
			return 2;
		}
		if (strcmp(it->CameraPath,camInfo->CameraPath)==0)
		{
			return 3;
		}
	}
	return 0;
}

void CVisionClientDlg::SaveCamInfo()
{
	vector<Tmtv_CameraInfo>::iterator it;
	CMemoryFile mFile;
	bool backcode = mFile.OpenFile_W(_T("setting\\CameraInfo.data"));
	if (backcode==false)
	{
		return;
	}
	it = mCamInfoVec.begin();
	for (; it != mCamInfoVec.end(); it++)
	{
		mFile.WriteMemoryToFile(&(*it),sizeof(Tmtv_CameraInfo));
	}
	mFile.CloseFile();
}

void CVisionClientDlg::LoadCamInfo()
{
	vector<Tmtv_CameraInfo>::iterator it;
	CMemoryFile mFile;
	bool backcode = mFile.OpenFile_R(_T("setting\\CameraInfo.data"));
	if (backcode == false)
	{
		return;
	}
	mCamInfoVec.clear();
	Tmtv_CameraInfo mCamInfo;
	while (mFile.ReadMemoryFromFile(&mCamInfo,sizeof(Tmtv_CameraInfo)))
	{
		mCamInfoVec.push_back(mCamInfo);
	}
	mFile.CloseFile();
}

void CVisionClientDlg::ShowCamInfoToList()
{
	if (mCamInfoVec.empty()) return;
	mListCamera.DeleteAllItems();
	vector<Tmtv_CameraInfo>::iterator it;
	int i = 0;
	CString camIndexStr;
	wchar_t camNameStr[TMTV_LONGSTRLEN];
	for (it = mCamInfoVec.begin(); it != mCamInfoVec.end();it++)
	{
		CCommonFunc::AnsiToUnicode(it->CameraName, camNameStr, TMTV_LONGSTRLEN);
		camIndexStr.Format(_T("%d"), it->Indexnum);
		mListCamera.InsertItem(i, _T(""));//�������б��⣩ 
		mListCamera.SetItemText(i, 1, camIndexStr);
		mListCamera.SetItemText(i, 2, camNameStr);
	}

}

void CVisionClientDlg::ShowZoomImg(Tmtv_ImageInfo& imgInfo)
{
	wchar_t mwchar[TMTV_LONGSTRLEN];
	wchar_t mwtime[TMTV_LONGSTRLEN];
	Tmtv_CameraInfo mCaminfo;
	mCaminfo.Indexnum = imgInfo.mCamId;
	GetCam(&mCaminfo);



	CCommonFunc::AnsiToUnicode(mCaminfo.CameraName, mwchar, TMTV_LONGSTRLEN);
	CCommonFunc::AnsiToUnicode(imgInfo.GrabTime, mwtime, TMTV_LONGSTRLEN);
	CString mstr;
	mstr.Format(_T("%d�����: %s ʱ�䣺%s"), mCaminfo.Indexnum,mwchar,mwtime);
	pZoomImgDlg->SetWindowTextW(mstr);


	Mat mMat;
	mMat=imread(imgInfo.ImagePath);
	if (mMat.empty()) return;
	CRect mWndRect;
	this->GetWindowRect(&mWndRect);
	mWndRect.left += DLG_LF_MARGIN;
	mWndRect.top += DLG_TP_MARGIN;
	if (double(mWndRect.Width())/mWndRect.Height() > double(mMat.cols)/mMat.rows)
	{
		mWndRect.right = mWndRect.left + double(mMat.cols) / mMat.rows*mWndRect.Height();
	}
	else
	{
		mWndRect.bottom = mWndRect.top + mMat.rows / double(mMat.cols)*mWndRect.Width();
	}
	pZoomImgDlg->MoveWindow(&mWndRect);
	CRect mRect;
	pZoomImgDlg->GetClientRect(&mRect);
	cv::Size mSize = cv::Size(mRect.Width(), mRect.Height());
	Mat mat2;
	resize(mMat, mat2, mSize);
	pZoomImgDlg->mImg.Destroy();
	CvImage::Mat2CImage(mat2, pZoomImgDlg->mImg);
	HDC hDC = (pZoomImgDlg->GetDC())->GetSafeHdc();
	pZoomImgDlg->ShowWindow(SW_SHOW);
	pZoomImgDlg->mImg.Draw(hDC, mRect);
	//pZoomImgDlg->mImg.ReleaseDC();
}

void CVisionClientDlg::PointShowZoomImg(CPoint point)
{
	vector<CRect>::iterator it;
	if (point.x>DLG_LF_MARGIN && point.y>DLG_TP_MARGIN)
	{
		Tmtv_ImageInfo mImgInfo;
		EnterCriticalSection(&cs);
		it = mRectVec.begin();
		int i = 0;
		for (; it != mRectVec.end(); it++)
		{
			if ((point.x - DLG_LF_MARGIN) > it->left && (point.x - DLG_LF_MARGIN) < it->right &&
				(point.y - DLG_TP_MARGIN) > it->top && (point.y - DLG_TP_MARGIN) < it->bottom)
			{
				if (i>= mImageShowVec.size())
				{
					LeaveCriticalSection(&cs);
					return;
				}
				mImgInfo = mImageShowVec.at(i);
				break;
			}
			i++;
		}
		LeaveCriticalSection(&cs);
		ShowZoomImg(mImgInfo);

	}
}

void CVisionClientDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//InitConfig();
	//Tmtv_ImageInfo mImg;
	//sprintf(mImg.GrabTime, "2016-02-05-02:02:02");
	//sprintf(mImg.ImagePath, "D:\\aa.bmp");
	//mImg.IsVIP = 1;
	//mImg.IsWarnning = 1;
	//mImg.mDefectInfo.DefectNum = 1;
	//mImg.mDefectInfo.DefectPos[0][0] = 100;
	//mImg.mDefectInfo.DefectPos[0][1] = 180;
	//mImg.mDefectInfo.DefectPos[0][2] = 200;
	//mImg.mDefectInfo.DefectPos[0][3] = 240;
	//mImg.mDefectInfo.ImgWidth = 352;
	//mImg.mDefectInfo.ImgHeight = 288;
	//sprintf(mImg.mCameraInfo.CameraName , "ɳ�ذ����������к���1��");
	//mImg.mCameraInfo.Indexnum = 1;
	//sprintf(mImg.mCameraInfo.CameraPath, "D:\\bb");
	//mImageShowVec.push_back(mImg);
	//DrawImgToMemBmp();
	//ShowMemBmpToScreen();
}


void CVisionClientDlg::OnBnClickedAddcamBt()
{
	if (!m_IsLogin) return;

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pAddCamDlg!=NULL)
	{
		pAddCamDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pAddCamDlg = new CAddCamDlg(this);
		pAddCamDlg->Create(IDD_ADDCAM_DLG, this);
		pAddCamDlg->ShowWindow(SW_SHOW);
	}
}


void CVisionClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	InitConfig();
	// TODO: �ڴ˴������Ϣ����������
}


void CVisionClientDlg::OnBnClickedIsrollingBt()
{
	if (!m_IsLogin) return;
	CButton*pbt = (CButton*)GetDlgItem(IDC_ISROLLING_BT);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_IsRolling)
	{
		m_IsRolling = false;
		pbt->SetWindowTextW(_T("������ʾ"));
		pVisionRun->Suspend();
	}
	else
	{
		m_IsRolling = true;
		pbt->SetWindowTextW(_T("ֹͣ����"));
		pVisionRun->Resume();
	}
}


void CVisionClientDlg::OnBnClickedHistimgBt()
{
	if (!m_IsLogin) return;
	vector<Tmtv_CameraInfo>::iterator it;
	if (pAskHistDlg==NULL)
	{
		pAskHistDlg = new CAskHistDlg(this);
		pAskHistDlg->Create(IDD_ASKHIST_DLG, this);
	}
	pAskHistDlg->mComBoxCtrl.ResetContent();
	CString mstr;
	wchar_t CamName[MAX_STR_LEN] = {0};
	int i = 0;
	for (it = mCamInfoVec.begin(); it != mCamInfoVec.end(); it++)
	{
		CCommonFunc::AnsiToUnicode(it->CameraName, CamName, MAX_STR_LEN);
		mstr.Format(_T("%d �����:%s"), it->Indexnum, CamName);
		pAskHistDlg->mComBoxCtrl.InsertString(i, mstr);
		i++;
	}
	pAskHistDlg->ShowWindow(SW_SHOW);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CVisionClientDlg::OnBnClickedShnewimgBt()
{
	if (!m_IsLogin) return;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	pVisionRun->ShowRealTimeImg();
}


void CVisionClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_IsLogin) return;
	PointShowZoomImg(point);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CVisionClientDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//InitConfig();
	//Tmtv_ImageInfo mImg;
	//sprintf(mImg.GrabTime, "2016-02-05-02:02:02");
	//sprintf(mImg.ImagePath, "D:\\bb.bmp");
	//mImg.IsVIP = 1;
	//mImg.IsWarnning = 0;
	//mImg.mDefectInfo.DefectNum = 1;
	//mImg.mDefectInfo.DefectPos[0][0] = 100;
	//mImg.mDefectInfo.DefectPos[0][1] = 100;
	//mImg.mDefectInfo.DefectPos[0][2] = 200;
	//mImg.mDefectInfo.DefectPos[0][3] = 150;
	//mImg.mDefectInfo.ImgWidth = 352;
	//mImg.mDefectInfo.ImgHeight = 288;
	//sprintf(mImg.mCameraInfo.CameraName, "ɳ�ذ����������к���1��");
	//mImg.mCameraInfo.Indexnum = 2;
	//sprintf(mImg.mCameraInfo.CameraPath, "D:\\bb");
	//mImageShowVec.push_back(mImg);
	//DrawImgToMemBmp();
	//ShowMemBmpToScreen();
}


void CVisionClientDlg::OnBnClickedUserLoginBt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_IsLogin)
	{
		if(pAskHistDlg!=NULL) pAskHistDlg->ShowWindow(SW_HIDE);
		if (pAddCamDlg != NULL) pAddCamDlg->ShowWindow(SW_HIDE);
		m_IsLogin = false;
		GetDlgItem(IDC_USER_LOGIN_BT)->SetWindowTextW(L"�û��ѵǳ�");
		LONGSTR mstr;
		CCommonFunc::UnicodeToAnsi(LoginName.GetBuffer(), mstr, TMTV_LONGSTRLEN);
		VisionClientLogger::mLogger.TraceKeyInfo("%s�û��ǳ��ɹ���", mstr);
	}
	else
	{
		CLoginDlg mLoginDlg;
		INT_PTR nRes;
		nRes = mLoginDlg.DoModal();
		if (nRes != IDOK)
		{
			return;
		}
		LONGSTR username;
		LONGSTR password;
		CCommonFunc::UnicodeToAnsi(mLoginDlg.m_UserName.GetBuffer(), username, TMTV_LONGSTRLEN);
		CCommonFunc::UnicodeToAnsi(mLoginDlg.m_PassWord.GetBuffer(), password, TMTV_LONGSTRLEN);
		string usernamestr(username);
		string passwordstr(password);
		
		string EncrptUserName = mCrypt.AESEncrypt(usernamestr);
		string EncrptPassword = mCrypt.AESEncrypt(passwordstr);

		Tmt_UserInfo mUserInfo;
		strcpy_s(mUserInfo.UserName, TMTV_LONGSTRLEN, EncrptUserName.c_str());
		strcpy_s(mUserInfo.PassWord, TMTV_LONGSTRLEN, EncrptPassword.c_str());

		LONGSTR ExistPassword;
		vector<Tmt_UserInfo> mUserVec;
		Tmt_UserInfo tmpUserInfo;
		CMemoryFile mMemFile;
		mMemFile.OpenFile_R(L"setting\\user.data");
		bool IsExist = false;
		while (mMemFile.ReadMemoryFromFile((&tmpUserInfo), sizeof(Tmt_UserInfo)))
		{
			if (strcmp(tmpUserInfo.UserName, mUserInfo.UserName) == 0)
			{
				if (strcmp(tmpUserInfo.PassWord,mUserInfo.PassWord)==0)
				{
					IsExist = true;
				}
				continue;
			}
			mUserVec.push_back(tmpUserInfo);
		}
		if (!IsExist)
		{
			MessageBox(_T("��½ʧ�ܣ�"));
			return;
		}
		LoginName = mLoginDlg.m_UserName;
		m_IsLogin = true;
		VisionClientLogger::mLogger.TraceKeyInfo("%s�û���¼�ɹ���", username);
		GetDlgItem(IDC_USER_LOGIN_BT)->SetWindowTextW(L"�û��ѵ�½");
	}

}


void CVisionClientDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_IsLogin) return;
	vector<CRect>::iterator it;
	if (point.x > DLG_LF_MARGIN && point.y > DLG_TP_MARGIN)
	{
		Tmtv_ImageInfo mImgInfo;
		EnterCriticalSection(&cs);
		it = mRectVec.begin();
		int i = 0;
		for (; it != mRectVec.end(); it++)
		{
			if ((point.x - DLG_LF_MARGIN) > it->left && (point.x - DLG_LF_MARGIN) < it->right &&
				(point.y - DLG_TP_MARGIN) > it->top && (point.y - DLG_TP_MARGIN) < it->bottom)
			{
				if (i >= mImageShowVec.size())
				{
					LeaveCriticalSection(&cs);
					return;
				}
				mImgInfo = mImageShowVec.at(i);
				break;
			}
			i++;
		}
		LeaveCriticalSection(&cs);
		//pRunState->ResetImgWarn(mImgInfo.mCameraInfo.Indexnum);
		
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CVisionClientDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Tmtv_CameraInfo mcam;
	mcam.Indexnum = 0;
	AddCam(&mcam);
}

LRESULT CVisionClientDlg::OnAskResult(WPARAM wParam, LPARAM lParam)
{

	switch (((Tmtv_MsgInfo*)lParam)->MsgType)
	{
	case Tmtv_MsgInfo::TMTV_ADDCAM_OK:
	{
		MessageBox(L"�������ɹ���");
		break;
	}
	case Tmtv_MsgInfo::TMTV_ADDCAM_FAIL:
	{
		MessageBox(L"������ʧ�ܣ�");
		break;
	}
	case Tmtv_MsgInfo::TMTV_DELCAM_OK:
	{
		MessageBox(L"ɾ������ɹ���");
		break;
	}
	case Tmtv_MsgInfo::TMTV_DELCAM_FAIL:
	{
		MessageBox(L"ɾ�����ʧ�ܣ�");
		break;
	}
	case Tmtv_MsgInfo::TMTV_STARTALGO_FAIL:
	{
		MessageBox(L"�����㷨ʧ�ܣ�");
		break;
	}
	case Tmtv_MsgInfo::TMTV_STARTALGO_OK:
	{
		MessageBox(L"�����㷨�ɹ���");
		break;
	}
	case Tmtv_MsgInfo::TMTV_STARTCAM_OK:
	{
		MessageBox(L"������ɹ���");
		break;
	}
	case Tmtv_MsgInfo::TMTV_STARTCAM_FAIL:
	{
		MessageBox(L"�����ʧ�ܣ�");
		break;
	}
	case Tmtv_MsgInfo::TMTV_STOPCAM_OK:
	{
		MessageBox(L"ֹͣ����ɹ���");
		break;
	}
	case Tmtv_MsgInfo::TMTV_STOPCAM_FAIL:
	{
		MessageBox(L"ֹͣ���ʧ�ܣ�");
		break;
	}

	default:
		break;
	}
	delete (Tmtv_MsgInfo*)lParam;
	return 0;
}
