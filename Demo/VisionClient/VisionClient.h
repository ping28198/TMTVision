
// VisionClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVisionClientApp: 
// �йش����ʵ�֣������ VisionClient.cpp
//

class CVisionClientApp : public CWinApp
{
public:
	CVisionClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVisionClientApp theApp;