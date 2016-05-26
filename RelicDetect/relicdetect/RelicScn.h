#pragma once

#include "RelicDetect.h"
#include "RelicObj.h"
class RelicScn :public RelicDetect
{
public:
	bool RelicScn::Match_an_Obj(RelicObj obj);
	void RelicScn::Draw_Obj();
private:
	std::vector<Point2f> corners;
};

