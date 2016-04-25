// TestJson.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <F:\Tdevelop\TMT\TMTVision\Lib\jsoncpp\include\json\json.h>

#include <fstream>
#include <string>
#include <iostream>
using namespace std;
void readjson()
{
	std::ifstream ifs;
	ifs.open("test.json");

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root, false))
	{
		return;
	}

	Json::Value add_value = root["defects"];
	for (int i = 0; i < add_value.size(); ++i)
	{
		Json::Value temp_value = add_value[i];
		std::string x = temp_value["x"].asString();
		std::string y = temp_value["y"].asString();
		std::string width = temp_value["width"].asString();
		std::string height = temp_value["height"].asString();
		cout << "x: " << x << "y: " << y << "width" << width << "height" << height << std::endl;

		// use value array[index]
		//Json::Value temp_value = add_value[i];
		//std::string strName = add_value[i]["name"].asString();
		//std::string strMail = add_value[i]["email"].asString();
		//std::cout << "name: " << strName << " email: " << strMail << std::endl;
	}
}
void writejson()
{
	std::ifstream ifs;
	ifs.open("test.json");

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root, false))
	{
		return ;
	}

	Json::Value& add_value = root["area"];
	Json::Value append_value;
	append_value["x"] = "99";
	append_value["y"] = "88";
	append_value["width"] = "77";
	append_value["height"] = "66";
	add_value.append(append_value);

	//for (int i = 0; i < add_value.size(); ++i)
	//{
	//	Json::Value temp_value = add_value[i];
	//	std::string strName = temp_value["name"].asString();
	//	std::string strMail = temp_value["email"].asString();
	//	std::cout << "name: " << strName << " email: " << strMail << std::endl;
	//}

	Json::FastWriter writer;
	std::string json_append_file = writer.write(root);

	std::ofstream ofs;
	ofs.open("test_append.json");
	ofs << json_append_file;

}
int _tmain(int argc, _TCHAR* argv[])
{
	//readjson();
	writejson();
	system("Pause");

	return 0;
}