
// SoftwareRegister.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSoftwareRegisterApp: 
// �йش����ʵ�֣������ SoftwareRegister.cpp
//

class CSoftwareRegisterApp : public CWinApp
{
public:
	CSoftwareRegisterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSoftwareRegisterApp theApp;