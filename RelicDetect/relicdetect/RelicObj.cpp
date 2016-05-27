#include "RelicObj.h"
void RelicObj::Load_Img(InputArray img)
{
	this->img_color = img.getMat();
	cvtColor(img,this->img_gray,CV_BGR2GRAY);
	this->img_height = this->img_gray.rows;
	this->img_width = this->img_gray.cols;
}

string RelicObj::Convert_to_Json(RelicObj obj)
{
	using namespace std;

	Json::Value root;
	//set size node////////////////////////////////////////////////////////////////////////
	Json::Value size;
	size["height"] = obj.img_height;
	size["width"] = obj.img_width;
	root["img_size"] = size;
	//////////////////////////////////////////////////////////////////////////
	Json::Value arrayObj;
	Json::Value item;

	//item["keypoints"] = "jsoncpp";
	//item["descriptors"] = "jsoninjava";
	//item["php"] = "support";
	//arrayObj.append(item);
	
	Json::Value keypoints = NULL;
	Json::Value descriptors = NULL;
	Json::Value feature;
	
	feature["keypoinsts"] = keypoints;
	feature["descriptors"] = descriptors;
	//////////////////////////////////////////////////////////////////////////
	root["feature"] = feature;
	//root.toStyledString();
	std::string out = root.toStyledString();
	std::cout << out << std::endl;
	Json::FastWriter writer;
	return writer.write(root);

}
string RelicObj::Descriptors_to_Json_Obj(Mat descriptors)
{

}
RelicObj RelicObj::Parse_from_Json(string json_str)
{
	using namespace std;
	//std::string strValue = "{\"name\":\"json\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}]}";

	Json::Reader reader;
	Json::Value value;
	RelicObj a;
	if (reader.parse(json_str, value))
	{
		auto height = value["img_size"]["height"].asInt();
		auto width = value["img_size"]["width"].asInt();
		std::string out = value["name"].asString();
		std::cout << out << std::endl;
		const Json::Value arrayObj = value["array"];
		for (unsigned int i = 0; i < arrayObj.size(); i++)
		{
			if (!arrayObj[i].isMember("cpp"))
				continue;
			out = arrayObj[i]["cpp"].asString();
			std::cout << out;
			if (i != (arrayObj.size() - 1))
				std::cout << std::endl;
		}
	}
	return a;
}