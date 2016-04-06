
// VisionClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VisionClient.h"
#include "VisionClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

#define DLG_LF_MARGIN 270
#define DLG_TP_MARGIN 108



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


// CVisionClientDlg 对话框



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
END_MESSAGE_MAP()


// CVisionClientDlg 消息处理程序

BOOL CVisionClientDlg::OnInitDialog()
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

	ShowWindow(SW_MAXIMIZE);
	VisionPublicSet::mLogger.TraceInfo("----------------------------------------初始化");

	// TODO: 在此添加额外的初始化代码
	if (pImgBmp == NULL) pImgBmp = new CBitmap;
	pBmpDC = new CDC;
	pNetWorkServer = new CNetWorkServer(this);
	pRunState = new CVisionRunState(this);
	pAddCamDlg = NULL;
	m_IsRolling = true;
	pZoomImgDlg = new CZoomImgDlg(this);
	m_IsLogin = false;



	//CipherCode mCipCode;
	//string mstr = mCipCode.GetAvailableNetMac();
	//string hex = mCipCode.Chars2Hex(mstr);
	//string mstr2 = mCipCode.Hex2Chars(hex);

	//bool m = mCipCode.CompareNetMac(mstr2);

	InitializeCriticalSection(&cs);
	pBmpDC->CreateCompatibleDC(NULL);
	LONG mStyle;
	mStyle = GetWindowLong(mListCamera.m_hWnd, GWL_STYLE);//获取当前窗口风
	mStyle &= ~LVS_TYPEMASK;
	mStyle |= LVS_REPORT;
	SetWindowLong(mListCamera.m_hWnd, GWL_STYLE, mStyle);
	DWORD dwStyle = mListCamera.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_CHECKBOXES;
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）
	mListCamera.SetExtendedStyle(dwStyle); //设置扩展风格

	//mListCamera.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);
	mListCamera.InsertColumn(0, _T("重点"), LVCFMT_CENTER, 40);        //添加列标题 
	mListCamera.InsertColumn(1, _T("相机号"), LVCFMT_LEFT, 50);
	mListCamera.InsertColumn(2, _T("相机信息"), LVCFMT_LEFT, 250);

	//mt = imread("D:\\aa.jpg");
	InitConfig();
	pRunState->Resume();
	pNetWorkServer->Resume();
	LoadCamInfo();
	ShowCamInfoToList();
	pZoomImgDlg->Create(IDD_ZOOMIMG_DLG, this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVisionClientDlg::OnPaint()
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
		//InitConfig();
		ShowMemBmpToScreen();
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
		_T("宋体")                    // lpszFac
		);
	pImDC->SelectObject(&font);
	pImDC->SetTextColor(RGB(0, 255, 0));
	pImDC->SetBkMode(TRANSPARENT);//写入摄像头相关信息
	CString mstr;
	wchar_t mWstr[256];
	
	mstr.Format(_T("相机号:%d"),ImgInfo.mCameraInfo.Indexnum);
	pImDC->TextOut(0,0,mstr);
	CCommonFunc::AnsiToUnicode(ImgInfo.mCameraInfo.CameraName, mWstr, sizeof(mWstr));
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
	pImDC->SelectStockObject(NULL_BRUSH);//创建一个不填充的画刷
	if (ImgInfo.IsWarnning)//绘制缺陷信息
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
	int marginwidth = 5;//显示图像边框余量
	if (imgRangeRect.Width() <= 100 || imgRangeRect.Height() < 100 || xnum<=0 || ynum <= 0)
	{
		mRcVec.clear();
		return;
	}
	int imgwidth = (imgRangeRect.Width()- (xnum+1)*5)/xnum;
	int imgheight = (imgRangeRect.Height() - (ynum + 1) * 5) / ynum;
	if (imgheight < 20 || imgwidth < 20) return;//当计算图像宽度或高度小于20个像素就不再调整
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
	if (mImg.IsWarnning)
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
	//对话框元素初始化
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
	//图像部分初始化
	if (pImgBmp == NULL) pImgBmp = new CBitmap;
	pImgBmp->DeleteObject();
	int b = pImgBmp->CreateCompatibleBitmap(this->GetDC(), mImgRect.Width(), mImgRect.Height());
	if (pBmpDC!=NULL)
	{
		pBmpDC->SelectObject(pImgBmp);
	}
	if (pRunState!=NULL)
	{
		pRunState->SetShowImgNum(xImgNum*yImgNum);
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

void CVisionClientDlg::AddCam(Tmtv_CameraInfo *camInfo)
{
	mCamInfoVec.push_back(*camInfo);
	SaveCamInfo();
	ShowCamInfoToList();
	pNetWorkServer->AddNewCam(camInfo);
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
		mListCamera.InsertItem(i, _T(""));//添加项（即行标题） 
		mListCamera.SetItemText(i, 1, camIndexStr);
		mListCamera.SetItemText(i, 2, camNameStr);
	}

}

void CVisionClientDlg::ShowZoomImg(Tmtv_ImageInfo& imgInfo)
{
	wchar_t mwchar[TMTV_LONGSTRLEN];
	wchar_t mwtime[TMTV_LONGSTRLEN];
	CCommonFunc::AnsiToUnicode(imgInfo.mCameraInfo.CameraName, mwchar, TMTV_LONGSTRLEN);
	CCommonFunc::AnsiToUnicode(imgInfo.GrabTime, mwtime, TMTV_LONGSTRLEN);
	CString mstr;
	mstr.Format(_T("%d号相机: %s 时间：%s"),imgInfo.mCameraInfo.Indexnum,mwchar,mwtime);
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
	// TODO: 在此添加控件通知处理程序代码
	//InitConfig();
	Tmtv_ImageInfo mImg;
	sprintf(mImg.GrabTime, "2016-02-05-02:02:02");
	sprintf(mImg.ImagePath, "D:\\aa.bmp");
	mImg.IsVIP = 1;
	mImg.IsWarnning = 1;
	mImg.mDefectInfo.DefectNum = 1;
	mImg.mDefectInfo.DefectPos[0][0] = 100;
	mImg.mDefectInfo.DefectPos[0][1] = 100;
	mImg.mDefectInfo.DefectPos[0][2] = 150;
	mImg.mDefectInfo.DefectPos[0][3] = 200;
	mImg.mDefectInfo.ImgWidth = 352;
	mImg.mDefectInfo.ImgHeight = 288;
	sprintf(mImg.mCameraInfo.CameraName , "沙特阿拉伯、地中海，1街");
	mImg.mCameraInfo.Indexnum = 1;
	sprintf(mImg.mCameraInfo.CameraPath, "D:\\bb");
	mImageShowVec.push_back(mImg);
	DrawImgToMemBmp();
	ShowMemBmpToScreen();
}


void CVisionClientDlg::OnBnClickedAddcamBt()
{
	if (!m_IsLogin) return;

	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此处添加消息处理程序代码
}


void CVisionClientDlg::OnBnClickedIsrollingBt()
{
	if (!m_IsLogin) return;
	CButton*pbt = (CButton*)GetDlgItem(IDC_ISROLLING_BT);
	// TODO: 在此添加控件通知处理程序代码
	if (m_IsRolling)
	{
		m_IsRolling = false;
		pbt->SetWindowTextW(_T("滚动显示"));
		pRunState->Suspend();
	}
	else
	{
		m_IsRolling = true;
		pbt->SetWindowTextW(_T("停止滚动"));
		pRunState->Resume();
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
		mstr.Format(_T("%d 号相机:%s"), it->Indexnum, CamName);
		pAskHistDlg->mComBoxCtrl.InsertString(i, mstr);
		i++;
	}
	pAskHistDlg->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void CVisionClientDlg::OnBnClickedShnewimgBt()
{
	if (!m_IsLogin) return;
	// TODO: 在此添加控件通知处理程序代码
	pRunState->ShowRealTimeImg();
}


void CVisionClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_IsLogin) return;
	PointShowZoomImg(point);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CVisionClientDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//InitConfig();
	Tmtv_ImageInfo mImg;
	sprintf(mImg.GrabTime, "2016-02-05-02:02:02");
	sprintf(mImg.ImagePath, "D:\\bb.bmp");
	mImg.IsVIP = 1;
	mImg.IsWarnning = 0;
	mImg.mDefectInfo.DefectNum = 1;
	mImg.mDefectInfo.DefectPos[0][0] = 100;
	mImg.mDefectInfo.DefectPos[0][1] = 100;
	mImg.mDefectInfo.DefectPos[0][2] = 200;
	mImg.mDefectInfo.DefectPos[0][3] = 150;
	mImg.mDefectInfo.ImgWidth = 352;
	mImg.mDefectInfo.ImgHeight = 288;
	sprintf(mImg.mCameraInfo.CameraName, "沙特阿拉伯、地中海，1街");
	mImg.mCameraInfo.Indexnum = 2;
	sprintf(mImg.mCameraInfo.CameraPath, "D:\\bb");
	mImageShowVec.push_back(mImg);
	DrawImgToMemBmp();
	ShowMemBmpToScreen();
}


void CVisionClientDlg::OnBnClickedUserLoginBt()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_IsLogin)
	{
		if(pAskHistDlg!=NULL) pAskHistDlg->ShowWindow(SW_HIDE);
		if (pAddCamDlg != NULL) pAddCamDlg->ShowWindow(SW_HIDE);
		m_IsLogin = false;
		GetDlgItem(IDC_USER_LOGIN_BT)->SetWindowTextW(L"用户已登出");
		LONGSTR mstr;
		CCommonFunc::UnicodeToAnsi(LoginName.GetBuffer(), mstr, TMTV_LONGSTRLEN);
		VisionPublicSet::mLogger.TraceKeyInfo("%s用户登出成功！", mstr);
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
			MessageBox(_T("登陆失败！"));
			return;
		}
		LoginName = mLoginDlg.m_UserName;
		m_IsLogin = true;
		VisionPublicSet::mLogger.TraceKeyInfo("%s用户登录成功！", username);
		GetDlgItem(IDC_USER_LOGIN_BT)->SetWindowTextW(L"用户已登陆");
	}

}


void CVisionClientDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
		pRunState->ResetImgWarn(mImgInfo.mCameraInfo.Indexnum);
		
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}
