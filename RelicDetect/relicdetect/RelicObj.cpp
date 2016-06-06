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
	//set keypoints and descriptors////////////////////////////////////////////////////////////////////////
	Json::Value feature;	
	feature["keypoints"] = Keypoints_to_Json_Obj(this->keypoints);
	feature["descriptors"] = Descriptors_to_Json_Obj(this->descriptors);
	//////////////////////////////////////////////////////////////////////////
	root["feature"] = feature;
	//root.toStyledString();
	//std::string out = root.toStyledString();
	//std::cout << out << std::endl;
	Json::FastWriter writer;
	return writer.write(root);

}
Json::Value RelicObj::Descriptors_to_Json_Obj(Mat descriptors)
{
	Json::Value root;
	stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << descriptors;
	string str_descriptors;
	str_descriptors = ss.str();
	root = str_descriptors;
	//for (int i = 0;i < descriptors.rows;i++)
	//{
	//	Json::Value array_line;
	//	int line[64];
	//	for (int j = 0;j < descriptors.cols;j++)
	//	{
	//		array_line.append(descriptors.at<int>(i, j)).asInt();
	//	}
	//	//array_line = line;
	//	//cout<< array_line.toStyledString();
	//	root.append(array_line);
	//	//cout << root.toStyledString();
	//}
	return root;
}
Json::Value RelicObj::Keypoints_to_Json_Obj(vector<KeyPoint> keypoints)
{
	stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << keypoints;
	string str_keypoints;
	str_keypoints = ss.str();

	Json::Value all_keypoints = str_keypoints;
	//for (int i = 0;i < keypoints.size();i++)
	//{
	//	Json::Value one_keypoint;
	//	one_keypoint["index"] = i;
	//	one_keypoint["size"] = keypoints[i].size;
	//	one_keypoint["angle"] = keypoints[i].angle;
	//	one_keypoint["reponse"] = keypoints[i].response;
	//	one_keypoint["octave"] = keypoints[i].octave;
	//	one_keypoint["class_id"] = keypoints[i].class_id;
	//	Json::Value pt;
	//	pt["x"] = keypoints[i].pt.x;
	//	pt["y"] = keypoints[i].pt.y;
	//	one_keypoint["pt"] = pt;
	//	//cout << one_keypoint.toStyledString();
	//	all_keypoints.append(one_keypoint);
	//	//cout << all_keypoints.toStyledString();
	//}
	return all_keypoints;
}
RelicObj RelicObj::Parse_from_Json(string json_str)
{
	//using namespace std;
	//std::string strValue = "{\"name\":\"json\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}]}";

	Json::Reader reader;
	Json::Value value;
	RelicObj a;
	if (reader.parse(json_str, value))
	{
		auto height = value["img_size"]["height"].asInt();
		auto width = value["img_size"]["width"].asInt();
		auto keypoints = value["feature"]["keypoints"].asString();
		auto descriptors = value["feature"]["descriptors"].asString();

		stringstream ss_keypoints,ss_descriptors;
		ss_keypoints << keypoints;
		ss_descriptors << descriptors;
		boost::archive::text_iarchive ia_kp(ss_keypoints);
		boost::archive::text_iarchive ia_dp(ss_descriptors);
		vector<KeyPoint> read_keypoints;
		Mat read_descriptors;
		ia_kp >> read_keypoints;
		ia_dp >> read_descriptors;

		this->img_height = height;
		this->img_width = width;
		this->keypoints = read_keypoints;
		this->descriptors = read_descriptors;
		//////////////////////////////////////////////////////////////////////////
		//std::string out = value["name"].asString();
		//std::cout << out << std::endl;
		//const Json::Value arrayObj = value["array"];
		//for (unsigned int i = 0; i < arrayObj.size(); i++)
		//{
		//	if (!arrayObj[i].isMember("cpp"))
		//		continue;
		//	out = arrayObj[i]["cpp"].asString();
		//	std::cout << out;
		//	if (i != (arrayObj.size() - 1))
		//		std::cout << std::endl;
		//}
	}
	return a;
}