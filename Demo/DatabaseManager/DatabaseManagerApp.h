
// DatabaseManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDatabaseManagerApp: 
// �йش����ʵ�֣������ DatabaseManager.cpp
//

class CDatabaseManagerApp : public CWinApp
{
public:
	CDatabaseManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDatabaseManagerApp theApp;