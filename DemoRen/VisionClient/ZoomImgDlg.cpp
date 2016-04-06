// ZoomImgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VisionClient.h"
#include "ZoomImgDlg.h"
#include "afxdialogex.h"
#include "VisionClientDlg.h"

// CZoomImgDlg 对话框

IMPLEMENT_DYNAMIC(CZoomImgDlg, CDialog)

CZoomImgDlg::CZoomImgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ZOOMIMG_DLG, pParent)
{

}

CZoomImgDlg::~CZoomImgDlg()
{
}

void CZoomImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CZoomImgDlg, CDialog)
	ON_WM_MOVING()
END_MESSAGE_MAP()


// CZoomImgDlg 消息处理程序


void CZoomImgDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);
	if (mImg.IsNull())
	{
		return;
	}
	HDC hDC = (this->GetDC())->GetSafeHdc();
	CRect mRect;
	this->GetClientRect(&mRect);
	mImg.Draw(hDC, mRect);
	//mImg.ReleaseDC();
	// TODO: 在此处添加消息处理程序代码
}
