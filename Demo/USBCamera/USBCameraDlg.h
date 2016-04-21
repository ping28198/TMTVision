
// USBCameraDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CImageWnd.h"

// CUSBCameraDlg 对话框
class CUSBCameraDlg : public CDialog
{
// 构造
public:
	CUSBCameraDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USBCAMERA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void HideToTray();
	afx_msg LRESULT OnShowFromTray(WPARAM   wParam, LPARAM   lParam);
public:
	CStatic m_CamStc;
	CComboBox m_CamCob;
	CStatic m_ControlStc;
	CButton m_StartCamBtn;
	CButton m_StopCamBtn;
	CButton m_UplevelBtn;
	CButton m_DownLevelBtn;
	CStatic m_CamPathStc;
	CEdit m_CamPathEdt;
	CButton m_SetCamPathBtn;
	CStatic m_CapTimeStc;
	CEdit m_CapTimeEdt;
	CStatic m_CapNumStc;
	CEdit m_CapNumEdt;
	CButton m_ShowImgBtn;
	CImageWnd m_ImagePan;

	afx_msg void OnBnClickedStartcamBtn();
	afx_msg void OnBnClickedStopcamBtn();
	afx_msg void OnBnClickedUplevelBtn();
	afx_msg void OnBnClickedDownlevelBtn();
	afx_msg void OnBnClickedSetcampathBtn();
	afx_msg void OnCbnSelchangeCamCob();
	afx_msg void OnBnClickedShowimgBtn();

	bool m_Maxed;
	int m_DiffWndWidth;
	int m_DiffWndHeight;
	int m_ImageCtrlWidth;
	int m_ImageCtrlHeight;
	CRect m_MaxRect;
	CRect m_MinRect;
#define BORDERWIDTH   2
#define CONTROLHEIGHT 24
	void SetSize(bool maxed = false, int imageWidth = 8 * CONTROLHEIGHT, int imageHeight = 0);

	void Update();
};
