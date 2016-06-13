
// VisionClientDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "CommonInclude.h"
//#include "VisionRunState.h"
//#include "NetWorkServer.h"
#include "AddCamDlg.h"
#include "AskHistDlg.h"
#include "ZoomImgDlg.h"
#include "LoginDlg.h"
#include "CipherCode.h"
#include "VisionRun.h"
// CVisionClientDlg 对话框




class CVisionClientDlg : public CDialogEx
{
// 构造
public:
	CVisionClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VISIONCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	CRITICAL_SECTION cs;
	vector<Tmtv_CameraInfo> mCamInfoVec;//相机信息
	vector<Tmtv_ImageInfo> mImageShowVec;//待显示图像信息容器
	vector<CRect> mRectVec;//待显示区域
	CRect mImgRect;
	CBitmap* pImgBmp;
	CDC *pBmpDC;
	int xImgNum;
	int yImgNum;
	//CNetWorkServer *pNetWorkServer;
	CVisionRun *pVisionRun;
	CAddCamDlg* pAddCamDlg;
	CAskHistDlg* pAskHistDlg;
	CZoomImgDlg* pZoomImgDlg;
	bool m_IsRolling;
	TmtCrypter mcrypt;
	//Mat mt;//测试
	//////////////////////////////////////////////////////////////////////////
	//将单个图像绘制到指定区域且绘制信息
	//图像信息，区域信息
	 void DrawImage(Tmtv_ImageInfo& ImgInfo,CRect mRect, CDC* pDC);//
	//////////////////////////////////////////////////////////////////////////
	//绘制所有图像
	//图像信息容器，区域容器，绘图句柄，背景颜色
	 void DrawAllImage(vector<Tmtv_ImageInfo> &mImgVec, vector<CRect> &mRcVec, CDC* pDC, COLORREF bkcolor);
	
	//////////////////////////////////////////////////////////////////////////
	//调整图像显示区域
	//水平显示个数，竖直显示个数，图像区域，所有图像区容器
	 void AdjustImgRect(int xnum,int ynum,CRect imgRangeRect, vector<CRect> &mRcVec);
	
	//////////////////////////////////////////////////////////////////////////
	//绘制警告标志
	 void DrawWarnSign(Tmtv_ImageInfo &mImg, CRect &mRc, CDC* pDC, int marginwidth,COLORREF bkcolor);

	void InitConfig();//重设配置参数
	void DrawImgToMemBmp();//绘制函数
	void ShowMemBmpToScreen();//显示函数

	//相机信息相关函数
	int AddCam(Tmtv_CameraInfo *camInfo);
	bool GetCam(Tmtv_CameraInfo *camInfo);
	void ModifyCam(Tmtv_CameraInfo *camInfo);
	void DelCam(int camIndex);
	//////////////////////////////////////////////////////////////////////////
	//相机信息是否有重复
	//返回0=没有重复；1=相机号重复；2=相机描述重复；3=相机目录重复
	int CheckCam(Tmtv_CameraInfo *camInfo);
	void SaveCamInfo();
	void LoadCamInfo();
	//////////////////////////////////////////////////////////////////////////
	//窗体信息更新相关函数
	void ShowCamInfoToList();
	



	//////////////////////////////////////////////////////////////////////////
	//放大图像相关函数
	void ShowZoomImg(Tmtv_ImageInfo& imgInfo);
	void PointShowZoomImg(CPoint point);

private:
	bool m_IsLogin;//标志用户是否登录
	CString LoginName;
	TmtCrypter mCrypt;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl mListCamera;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedAddcamBt();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedIsrollingBt();
	afx_msg void OnBnClickedHistimgBt();
	afx_msg void OnBnClickedShnewimgBt();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedUserLoginBt();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton3();
	//自定义消息函数
	afx_msg LRESULT OnAskResult(WPARAM wParam, LPARAM lParam);
};
