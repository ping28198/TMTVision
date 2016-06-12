#pragma once
#include "RelicDetect.h"
class RelicObj :public RelicDetect
{
public:
	//************************************
	// Method:    Load_Img
	// Returns:   void
	// Parameter: InputArray img
	// ˵��:		  ����һ�Ų�ɫͼƬ��ͬʱ�洢��Ҷ�ͼ������
	//************************************
	void RelicObj::Load_Img(InputArray img);
	//************************************
	// Method:    Convert_to_Json
	// Returns:   std::string
	// Parameter: RelicObj obj
	// ˵��:		  ��һ��RelicObj����ת��Ϊjson���󣬲���string�������������Ľ���������Ա��浽�ļ������ݿ⡣
	//************************************
	string Convert_to_Json(RelicObj obj);
	string Save_to_Json();
	//************************************
	// Method:    Descriptors_to_Json_Obj
	// Returns:   Json::Value
	// Parameter: Mat descriptors
	// ˵��:		  ������������ת��Ϊjson����������boost::serialization�ⷽ��
	//************************************
	Json::Value Descriptors_to_Json_Obj(Mat descriptors);
	//************************************
	// Method:    Keypoints_to_Json_Obj
	// Returns:   Json::Value
	// Parameter: vector<KeyPoint> keypoints
	// ˵��:		  ���ؼ�������ת��Ϊjson����������boost::serialization�ⷽ��
	//************************************
	Json::Value Keypoints_to_Json_Obj(vector<KeyPoint> keypoints);
	//************************************
	// Method:    Parse_from_Json
	// Returns:   RelicObj
	// Parameter: string json_str
	// ˵��:		  ��json�ִ���������ȡͼ���������Ϣ������ʼ��RelicObj����
	//************************************
	void Parse_from_Json(string json_str);
public:
	int img_width;
	int img_height;
};


