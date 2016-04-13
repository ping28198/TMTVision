#pragma once
#include "CommonInclude.h"
#include "CameraObject.h"
#include <queue>
class CCameraManager
{
private:
	vector<CCameraObject*> m_pCamServVec;

public:
	CCameraObject* GetCamServer(int CamIndex);
	queue<int> q;









};