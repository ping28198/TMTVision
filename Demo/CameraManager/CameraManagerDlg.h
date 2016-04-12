
// CameraManagerDlg.h : 头文件
//

#pragma once
#define WM_SHOWFROMTRAY (WM_USER+10)

// CCameraManagerDlg 对话框
class CCameraManagerDlg : public CDialog
{
// 构造
public:
	CCameraManagerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERAMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam); 
	void HideToTray();
	afx_msg LRESULT OnShowFromTray(WPARAM   wParam, LPARAM   lParam);
};
