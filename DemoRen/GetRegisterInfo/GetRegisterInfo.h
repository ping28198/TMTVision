
// GetRegisterInfo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGetRegisterInfoApp: 
// �йش����ʵ�֣������ GetRegisterInfo.cpp
//

class CGetRegisterInfoApp : public CWinApp
{
public:
	CGetRegisterInfoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGetRegisterInfoApp theApp;