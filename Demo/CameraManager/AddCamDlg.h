#pragma once



#include "afxwin.h"
#include "CommonInclude.h"



#define MAX_RANGE_NUM 5

// CAddCamDlg 对话框
class CCameraManagerDlg;
class CAddCamDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddCamDlg)
public:
	CAddCamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddCamDlg();
	CCameraManagerDlg* pParent;
	char Campath[TMTV_PATHSTRLEN];
	char Camname[TMTV_PATHSTRLEN];
	Tmtv_CameraInfo mCamInfo;
	vector<CPoint> mWarnRange[MAX_RANGE_NUM];
	int mRangeIndex;
	CRect mImgRect;
	CImage mimg;//调整尺寸后图像图片
	CImage maskimg;//掩码图片
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDCAM_DLG };
#endif

public:
	void DrawImg();
	void DrawWarnRange();
	bool GenerateMskImg(CString& mstr);









protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool isDrawImg;
	int m_CamIndex;
	CString m_CamName;
	int mCamPos_X;
	int m_CamPos_Y;
	int mCamPos_Z;
	afx_msg void OnBnClickedAddSetcampathBt();
	afx_msg void OnBnClickedAddFinishBt();
	afx_msg void OnBnClickedAddSetmskBt();
	afx_msg void OnBnClickedAddRedrawBt();
	afx_msg void OnBnClickedAddAddrangeBt();
	CString m_CamPath;
	CComboBox m_WarningLevel;
	afx_msg void OnBnClickedAddOpenimg();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
};
