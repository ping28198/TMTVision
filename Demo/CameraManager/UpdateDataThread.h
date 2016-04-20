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
	CCameraManagerDlg* pParent;//�Ի���ָ��
	CameraManager* pCamManager;//���������ָ��
	unsigned long m_NetCount;

};