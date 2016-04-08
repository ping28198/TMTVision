
// UserRegisterDlg.h : ͷ�ļ�
//

#pragma once
#include "CommonInclude.h"

// CUserRegisterDlg �Ի���
class CUserRegisterDlg : public CDialogEx
{
// ����
public:
	CUserRegisterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERREGISTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
private:
	CfgFileOp mCfgOp;
	TmtCrypter mCrypt;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
