#pragma once
#include "stdafx.h"
#include "CommonInclude.h"
#include "CameraManager.h"

class CCameraManagerDlg;
class CUpdateDataThread : public Thread
{
public:
	CUpdateDataThread(void* pParent);
	~CUpdateDataThread();
	void Initial();


private:
	void Task();


private:
	CCameraManagerDlg* pParent;//对话框指针
	CameraManager* pCamManager;//相机管理类指针
	unsigned long m_NetCount;

};