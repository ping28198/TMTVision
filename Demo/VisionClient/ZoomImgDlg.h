#pragma once
#include "CommonInclude.h"

// CZoomImgDlg �Ի���
class CVisionClientDlg;
class CZoomImgDlg : public CDialog
{
	DECLARE_DYNAMIC(CZoomImgDlg)

public:
	CZoomImgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZoomImgDlg();
	CImage mImg;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZOOMIMG_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
};
