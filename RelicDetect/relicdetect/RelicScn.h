#pragma once

#include "RelicDetect.h"
class RelicScn :public RelicDetect
{
public:
	bool Match_a_Obj(InputArray obj_descriptors);
};

