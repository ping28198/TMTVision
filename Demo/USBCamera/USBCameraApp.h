
// USBCamera.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUSBCameraApp: 
// �йش����ʵ�֣������ USBCamera.cpp
//

class CUSBCameraApp : public CWinApp
{
public:
	CUSBCameraApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUSBCameraApp theApp;