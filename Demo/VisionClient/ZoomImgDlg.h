#pragma once
#include "CommonInclude.h"

// CZoomImgDlg 对话框
class CVisionClientDlg;
class CZoomImgDlg : public CDialog
{
	DECLARE_DYNAMIC(CZoomImgDlg)

public:
	CZoomImgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZoomImgDlg();
	CImage mImg;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZOOMIMG_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
};
