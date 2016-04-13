
// UserRegisterDlg.h : 头文件
//

#pragma once
#include "CommonInclude.h"

// CUserRegisterDlg 对话框
class CUserRegisterDlg : public CDialogEx
{
// 构造
public:
	CUserRegisterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERREGISTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	CfgFileOp mCfgOp;
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
	CString m_UserName;
	CString m_PassWord;
	CString m_PassWordConfirm;
	CString m_AuthorizedCode;
	afx_msg void OnBnClickedOk();
};
