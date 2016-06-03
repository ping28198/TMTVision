#pragma once
#include "RelicDetect.h"
class RelicObj :public RelicDetect
{
public:
	void RelicObj::Load_Img(InputArray img);
	string Convert_to_Json(RelicObj obj);
	Json::Value Descriptors_to_Json_Obj(Mat descriptors);
	Json::Value Keypoints_to_Json_Obj(vector<KeyPoint> keypoints);
	RelicObj Parse_from_Json(string json_str);
public:
	int img_width;
	int img_height;
};


