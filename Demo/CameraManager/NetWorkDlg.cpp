// NetWorkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetWorkDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CameraManagerDlg.h"
// NetWorkDlg �Ի���

IMPLEMENT_DYNAMIC(NetWorkDlg, CDialogEx)

NetWorkDlg::NetWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MESSAGEINFO_DLG, pParent)
{
	this->pParent = (CCameraManagerDlg*)pParent;
}

NetWorkDlg::~NetWorkDlg()
{
}

void NetWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NetWorkDlg, CDialogEx)
END_MESSAGE_MAP()


// NetWorkDlg ��Ϣ�������
