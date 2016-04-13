// AddCamDlg.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "VisionClient.h"
#include "AddCamDlg.h"
#include "afxdialogex.h"
#include "VisionClientDlg.h"

// CAddCamDlg 对话框

IMPLEMENT_DYNAMIC(CAddCamDlg, CDialog)

CAddCamDlg::CAddCamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ADDCAM_DLG, pParent)
	, m_CamIndex(0)
	, m_CamName(_T(""))
	, mCamPos_X(0)
	, m_CamPos_Y(0)
	, mCamPos_Z(0)
	, m_CamPath(_T(""))
{
	this->pParent = (CVisionClientDlg*)pParent;

}

CAddCamDlg::~CAddCamDlg()
{
}


void CAddCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADD_CAM_NUM_EDIT, m_CamIndex);
	DDV_MinMaxInt(pDX, m_CamIndex, 0, 1024);
	DDX_Text(pDX, IDC_ADD_CAMNAME_EDIT, m_CamName);
	DDX_Text(pDX, IDC_ADD_CAMPOS_X_EDIT, mCamPos_X);
	DDX_Text(pDX, IDC_ADD_CAMPOS_Y_EDIT, m_CamPos_Y);
	DDX_Text(pDX, IDC_ADD_CAMPOS_Z_EDIT, mCamPos_Z);
	DDX_Text(pDX, IDC_ADD_CAMPATH_ST, m_CamPath);
	DDX_Control(pDX, IDC_COMBO1, m_WarningLevel);
}


BEGIN_MESSAGE_MAP(CAddCamDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD_SETCAMPATH_BT, &CAddCamDlg::OnBnClickedAddSetcampathBt)
	ON_BN_CLICKED(IDC_ADD_FINISH_BT, &CAddCamDlg::OnBnClickedAddFinishBt)
	ON_BN_CLICKED(IDC_ADD_SETMSK_BT, &CAddCamDlg::OnBnClickedAddSetmskBt)
	ON_BN_CLICKED(IDC_ADD_REDRAW_BT, &CAddCamDlg::OnBnClickedAddRedrawBt)
	ON_BN_CLICKED(IDC_ADD_ADDRANGE_BT, &CAddCamDlg::OnBnClickedAddAddrangeBt)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CAddCamDlg 消息处理程序


void CAddCamDlg::OnBnClickedAddSetcampathBt()
{
	// TODO: 在此添加控件通知处理程序代码
	memset(Campath, 0, sizeof(Campath));
	CString strDir;
	wchar_t mPath[MAX_PATH];
	BROWSEINFO bi;
	char buffer[MAX_PATH];
	ZeroMemory(buffer, MAX_PATH);
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = LPWSTR(buffer);
	bi.lpszTitle = L"选择一个文件夹";
	bi.ulFlags = BIF_EDITBOX;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST pList = NULL;
	if ((pList = SHBrowseForFolder(&bi)) != NULL)
	{
		SHGetPathFromIDList(pList, mPath);
		CStatic*pSt = (CStatic*)this->GetDlgItem(IDC_ADD_CAMPATH_ST);
		pSt->SetWindowTextW(mPath);
		CCommonFunc::UnicodeToAnsi(mPath, Campath, sizeof(mPath));
	}
	else
	{
		return;
	}
}

void CAddCamDlg::OnBnClickedAddFinishBt()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CCommonFunc::UnicodeToAnsi(m_CamName.GetBuffer(), Camname, TMTV_PATHSTRLEN);
	mCamInfo.Indexnum = m_CamIndex;
	mCamInfo.CameraPos[0] = mCamPos_X;
	mCamInfo.CameraPos[1] = m_CamPos_Y;
	mCamInfo.CameraPos[2] = mCamPos_Z;
	strcpy_s(mCamInfo.CameraName, TMTV_PATHSTRLEN, Camname);
	strcpy_s(mCamInfo.CameraPath, TMTV_PATHSTRLEN, Campath);
	int warnindex = m_WarningLevel.GetCurSel();
	if (warnindex>=0&&warnindex<=4)
	{
		mCamInfo.AlgorithmInfo.WarnningLevel = m_CamIndex;
	}
	if (strlen(mCamInfo.CameraName)==0)
	{
		MessageBox(_T("相机描述不可为空！"));
		return;
	}
	if (strlen(mCamInfo.CameraPath)==0)
	{
		MessageBox(_T("请先选择相机目录！"));
		return;
	}
	int b = pParent->CheckCam(&mCamInfo);
	if (b==1)
	{
		MessageBox(_T("相机号重复！"));
		return;
	}
	else if (b == 2)
	{
		MessageBox(_T("相机描述重复！"));
		return;
	}
	else if (b == 3)
	{
		MessageBox(_T("相机目录重复！"));
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//生成掩码图像
	CString mstr;
	mstr.Format(_T("maskimg\\%d.jpg"), mCamInfo.Indexnum);
	if (!GenerateMskImg(mstr))
	{
		MessageBox(_T("请检查掩码图像设置！"));
		return;
	}
	//保存掩码图像地址
	wchar_t wchar[TMTV_PATHSTRLEN] = { 0 };
	CCommonFunc::GetAppPath(wchar, TMTV_PATHSTRLEN);
	mstr =_T("\\") + mstr;
	mstr = wchar + mstr;
	CCommonFunc::UnicodeToAnsi(mstr.GetBuffer(), mCamInfo.AlgorithmInfo.MaskImgPath, TMTV_PATHSTRLEN);

	mCamInfo.Status = Tmtv_CameraInfo::TMTV_RUNNINGCAM;
	mCamInfo.WaiteTime = 2000;
	mCamInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;

	if(pParent->AddCam(&mCamInfo)!=0)
	MessageBox(_T("添加成功！"));

	mRangeIndex = 0;
	for (int i = 0; i < MAX_RANGE_NUM; i++)
	{
		mWarnRange[i].clear();
	}
	mimg.Destroy();
	CButton* pBt = (CButton*)GetDlgItem(IDC_ADD_REDRAW_BT);
	pBt->EnableWindow(FALSE);
	pBt = (CButton*)GetDlgItem(IDC_ADD_ADDRANGE_BT);
	pBt->EnableWindow(FALSE);
	isDrawImg = false;
	memset(Campath, 0, sizeof(Campath));
	memset(Camname, 0, sizeof(Camname));

	mRangeIndex = 0;
}


void CAddCamDlg::OnBnClickedAddSetmskBt()
{
	// TODO: 在此添加控件通知处理程序代码
	mRangeIndex = 0;
	char filepath[256];
	Mat mMat;
	CString FilePathName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("JPG Files (*.jpg)|*.jpg|BMP Files (*.bmp)|*.bmp|"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{

		FilePathName = dlg.GetPathName(); //文件名保存在了FilePathName里
		CCommonFunc::UnicodeToAnsi(FilePathName.GetBuffer(), filepath, 256);
		mMat = imread(filepath);
		if(mMat.empty()) return;
		cv::Size mSize = cv::Size(mImgRect.Width(), mImgRect.Height());
		Mat image2 = Mat(mSize, CV_32S);
		resize(mMat, image2, mSize);
		CvImage::Mat2CImage(image2, mimg);
		if (mimg.IsNull())
		{
			CButton* pBt = (CButton*)GetDlgItem(IDC_ADD_REDRAW_BT);
			pBt->EnableWindow(FALSE);
			pBt = (CButton*)GetDlgItem(IDC_ADD_ADDRANGE_BT);
			pBt->EnableWindow(FALSE);
			isDrawImg = false;
			return;
		}
		int nBPP = mMat.channels()*8;
		if (!maskimg.IsNull()) maskimg.Destroy();
		maskimg.Create(mMat.cols, mMat.rows,nBPP );
		CButton* pBt = (CButton*)GetDlgItem(IDC_ADD_REDRAW_BT);
		pBt->EnableWindow(TRUE);
		pBt = (CButton*)GetDlgItem(IDC_ADD_ADDRANGE_BT);
		pBt->EnableWindow(TRUE);
		mRangeIndex = 0;
		for (int i = 0; i < MAX_RANGE_NUM; i++)
		{
			mWarnRange[i].clear();
		}
	}
	else
	{
		return;
	}
	DrawImg();
	isDrawImg = true;
}


void CAddCamDlg::OnBnClickedAddRedrawBt()
{
	// TODO: 在此添加控件通知处理程序代码
	mRangeIndex = 0;
	for (int i = 0; i < MAX_RANGE_NUM;i++)
	{
		mWarnRange[i].clear();
	}
	DrawImg();
	DrawWarnRange();
}
void CAddCamDlg::OnBnClickedAddAddrangeBt()
{
	// TODO: 在此添加控件通知处理程序代码
	if (mWarnRange[mRangeIndex].size()<3)
	{
		MessageBox(_T("请先绘制完成上一个区域！"));
		return;
	}
	if (mRangeIndex==MAX_RANGE_NUM-1)
	{
		MessageBox(_T("达到区域数量上限！"));
		return;
	}
	mRangeIndex++;
	mWarnRange[mRangeIndex].clear();
}


void CAddCamDlg::OnBnClickedAddOpenimg()
{
	// TODO: 在此添加控件通知处理程序代码

}
void CAddCamDlg::DrawImg()
{
	if (mimg.IsNull()) return;
	CDC *pDC = this->GetDC();
	HDC mHdc = pDC->GetSafeHdc();
	mimg.StretchBlt(mHdc, mImgRect);

}

void CAddCamDlg::DrawWarnRange()
{
	CDC *pDC = this->GetDC();
	vector<CPoint>::iterator it;
	CPen mpen(PS_SOLID, 2, RGB(255, 0, 0));
	pDC->SelectObject(&mpen);
	for (int i = 0; i <= mRangeIndex;i++)
	{
		if (mWarnRange[i].empty()) continue;
		it = mWarnRange[i].begin();
		pDC->MoveTo(mImgRect.left + it->x, mImgRect.top + it->y);
		for (it++; it != mWarnRange[i].end();it++)
		{
			pDC->LineTo(mImgRect.left + it->x, mImgRect.top + it->y);
			pDC->MoveTo(mImgRect.left + it->x, mImgRect.top + it->y);
		}
		it = mWarnRange[i].begin();
		pDC->LineTo(mImgRect.left + it->x, mImgRect.top + it->y);
	}
}


bool CAddCamDlg::GenerateMskImg(CString& mstr)
{
	if (mRangeIndex==0 && mWarnRange[mRangeIndex].size()<3)
	{
		return false;
	}
	if (maskimg.IsNull())
	{
		return false;
	}
	vector<CPoint>::iterator it;
	CPen *pPen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	CBrush *pBr = new CBrush(RGB(255, 255, 255));
	CDC *pDC = CDC::FromHandle(maskimg.GetDC());

	pDC->FillSolidRect(CRect(0, 0, maskimg.GetWidth(), maskimg.GetHeight()), RGB(0, 0, 0));
	pDC->SelectObject(pPen);
	pDC->SelectObject(pBr);
	//memDC.SetBkColor(RGB(255,255,255));
	CPoint mPt;
	for (int i = 0; i <= mRangeIndex; i++)
	{
		it = mWarnRange[i].begin();
		pDC->BeginPath();
		mPt = *it;
		mPt.x = mPt.x*double(maskimg.GetWidth()) / mImgRect.Width();
		mPt.y = mPt.y*double(maskimg.GetHeight()) / mImgRect.Height();
		pDC->MoveTo(mPt);
		for (it++; it != mWarnRange[i].end(); it++)
		{
			mPt = *it;
			mPt.x = mPt.x*double(maskimg.GetWidth()) / mImgRect.Width();
			mPt.y = mPt.y*double(maskimg.GetHeight()) / mImgRect.Height();
			pDC->LineTo(mPt);
		}
		pDC->EndPath();
		pDC->StrokeAndFillPath();
	}
	delete pPen;
	delete pBr;
	maskimg.ReleaseDC();
	maskimg.Save(mstr);
	return true;
}

void CAddCamDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint mPt;
	if (isDrawImg)
	{
		if (point.x <= mImgRect.right && point.x >= mImgRect.left && point.y <= mImgRect.bottom && point.y >= mImgRect.top)
		{
			mPt.x = point.x - mImgRect.left;
			mPt.y = point.y - mImgRect.top;
			mWarnRange[mRangeIndex].push_back(mPt);
			DrawImg();
			DrawWarnRange();
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}


BOOL CAddCamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->m_WarningLevel.InsertString(0, _T("5"));
	this->m_WarningLevel.InsertString(0, _T("4"));
	this->m_WarningLevel.InsertString(0, _T("3"));
	this->m_WarningLevel.InsertString(0, _T("2"));
	this->m_WarningLevel.InsertString(0, _T("1"));
	mRangeIndex = 0;
	memset(Campath, 0, sizeof(Campath));
	memset(Camname, 0, sizeof(Camname));

	CRect mrc;
	this->GetClientRect(&mrc);
	mImgRect.left = mrc.left +470;
	mImgRect.top = mrc.top;
	mImgRect.bottom = mrc.bottom;
	mImgRect.right = mrc.right;
	mRangeIndex = 0;

	isDrawImg = false;


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
