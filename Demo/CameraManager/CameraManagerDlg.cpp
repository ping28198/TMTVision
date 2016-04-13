
// CameraManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CameraManagerApp.h"
#include "CameraManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCameraManagerDlg �Ի���



CCameraManagerDlg::CCameraManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAMERAMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCameraManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCameraManagerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_SHOWFROMTRAY, OnShowFromTray)
END_MESSAGE_MAP()


// CCameraManagerDlg ��Ϣ��������

BOOL CCameraManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCameraManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCameraManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCameraManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	switch (nID)
	{
	case SC_MINIMIZE:
		HideToTray();
		break;
	case SC_CLOSE:
		if (::MessageBox(NULL,L"ȷ��Ҫ�˳���?", L"�����˳�", MB_YESNO) == IDYES)
		{
			//����ж�س���
			CDialog::OnSysCommand(nID, lParam);
		}
		break;
	default:
		CDialog::OnSysCommand(nID, lParam);
		break;
	}
}
void CCameraManagerDlg::HideToTray()
{
	NOTIFYICONDATA   nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWFROMTRAY;//�Զ������Ϣ���� 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(nid.szTip,128, L"�ƻ��������� ");//��Ϣ��ʾ��Ϊ���ƻ��������ѡ� 
	Shell_NotifyIcon(NIM_ADD, &nid);//������������ͼ�� 
	ShowWindow(SW_HIDE);//���������� 
}
LRESULT CCameraManagerDlg::OnShowFromTray(WPARAM   wParam, LPARAM   lParam)
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ 
{
	if (wParam != IDR_MAINFRAME)
		return   1;
	CPoint pt;
    CCameraManagerApp* app = (CCameraManagerApp*)AfxGetApp();
	switch (lParam) {
	case WM_RBUTTONUP:
	{ // Let's track a popup menu
		GetCursorPos(&pt);
		HMENU hmenu = LoadMenu(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_TRAYMENU));
		HMENU hpopup = GetSubMenu(hmenu, 0);
		switch (TrackPopupMenu(hpopup,            // Popup menu to track
			TPM_RETURNCMD |    // Return menu code
			TPM_RIGHTBUTTON,   // Track right mouse button?
			pt.x, pt.y,        // screen coordinates
			0,                 // reserved
			m_hWnd,            // owner
			NULL))             // LPRECT user can click in
							   // without dismissing menu
		{
		case ID_TRYMENU_OPEN: //OpenWindow(hwnd); break;
			ShowWindow(SW_SHOWDEFAULT);
			break;
		case ID_TRYMENU_SET:
			ShowWindow(SW_SHOWDEFAULT);
			break;
		case ID_TRYMENU_EXIT:
			//ж�ز���
			if (::MessageBox(NULL, L"ȷ��Ҫ�˳���?", L"�����˳�", MB_YESNO) == IDYES)
			{
				//����ж�س���

				CDialog::OnOK();
			}
			break;
		default:
			break;
		}
	}
	case WM_LBUTTONDBLCLK:
		ShowWindow(SW_SHOWDEFAULT);
	}
	return 0;
}