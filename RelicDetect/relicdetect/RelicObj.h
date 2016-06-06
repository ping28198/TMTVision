#pragma once
#include "RelicDetect.h"
class RelicObj :public RelicDetect
{
public:
	//************************************
	// Method:    Load_Img
	// Returns:   void
	// Parameter: InputArray img
	// 说明:		  载入一张彩色图片，同时存储其灰度图、长宽
	//************************************
	void RelicObj::Load_Img(InputArray img);
	//************************************
	// Method:    Convert_to_Json
	// Returns:   std::string
	// Parameter: RelicObj obj
	// 说明:		  将一个RelicObj对象转换为json对象，并以string类型输出。输出的结果可以用以保存到文件、数据库。
	//************************************
	string Convert_to_Json(RelicObj obj);
	//************************************
	// Method:    Descriptors_to_Json_Obj
	// Returns:   Json::Value
	// Parameter: Mat descriptors
	// 说明:		  将特征描述子转换为json对象。利用了boost::serialization库方法
	//************************************
	Json::Value Descriptors_to_Json_Obj(Mat descriptors);
	//************************************
	// Method:    Keypoints_to_Json_Obj
	// Returns:   Json::Value
	// Parameter: vector<KeyPoint> keypoints
	// 说明:		  将关键点特征转换为json对象。利用了boost::serialization库方法
	//************************************
	Json::Value Keypoints_to_Json_Obj(vector<KeyPoint> keypoints);
	//************************************
	// Method:    Parse_from_Json
	// Returns:   RelicObj
	// Parameter: string json_str
	// 说明:		  从json字串解析，读取图像的特征信息，来初始化RelicObj对象。
	//************************************
	void Parse_from_Json(string json_str);
public:
	int img_width;
	int img_height;
};


