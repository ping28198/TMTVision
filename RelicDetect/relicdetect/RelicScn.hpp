#pragma once
#ifndef RELIC
#define RELIC
#endif 
#include "RelicDetect.h"
#include "RelicObj.h"
class RelicScn :public RelicDetect
{
public:
	//************************************
	// Method:    Match_an_Obj
	// Returns:   bool
	// Parameter: RelicObj obj	��ƥ��Ķ�����Ҫ����
	// ˵��:		  ƥ��һ��Ŀ��
	//************************************
	bool RelicScn::Match_an_Obj(RelicObj obj);
	//************************************
	// Method:    Draw_Obj
	// Returns:   void
	// ˵��:		  ����ƥ��ɹ���Ŀ�����
	//************************************
	void RelicScn::Draw_Obj();
private:
	std::vector<Point2f> corners;
};

