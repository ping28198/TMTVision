// Create_Json_Features.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "RelicObj.h"
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
using namespace std::tr2::sys;  //<filesystem>的命名空间  
using namespace std;

namespace fs = std::tr2::sys;

int get_filenames(const std::string& dir, std::vector<std::string>& filenames);
int get_filenames(fs::path dir, std::vector<fs::path>& filenames);
string erase_cin_getline_quotation(string str);
int main()
{
	cout << "输入一个存储目标物体的目录" << endl;
	string default_path;
	getline(cin, default_path);
	default_path = erase_cin_getline_quotation(default_path);
	path p(default_path);
	cout << system_complete(p) << endl;
	vector<fs::path> filenames;
	string json_folder = system_complete(p).string() + "/json";
	if (fs::create_directory(json_folder)) {
		std::cout << "Success" << "\n";
	}
	get_filenames(system_complete(p), filenames);
	for (int i = 0;i < filenames.size();i++)
	{
		RelicObj ro;
		Mat img = imread(filenames[i].string());
		ro.Load_Img(img);
		ro.Calc_Keypoints_and_Descriptors();
		string json_str = ro.Save_to_Json();
		auto filename = filenames[i].filename().string();
		size_t dot = filename.find_last_of(".");
		if (dot!=string::npos)
		{
			filename.erase(dot, dot + 1);
		}
		fs::path new_dir();

		fstream examplefile(json_folder+"/"+ filename+".json", ios::out); //out方式打开文本
		if (examplefile.is_open())
		{
			examplefile << json_str;
			examplefile.close();
		}
	}
	system("pause");
	return 0;
}
 
int get_filenames(const std::string& dir, std::vector<std::string>& filenames)
{
	fs::path path(dir);
	if (!fs::exists(path))
	{
		return -1;
	}
	fs::directory_iterator end_iter;
	for (fs::directory_iterator iter(path); iter != end_iter; ++iter)
	{
		if (fs::is_regular_file(iter->status()))
		{
			filenames.push_back(iter->path().string());
		}
		if (fs::is_directory(iter->status()))
		{
			get_filenames(iter->path().string(), filenames);
		}
	}
	return filenames.size();
}

int get_filenames(fs::path dir, std::vector<fs::path>& filenames)
{
	fs::path path = system_complete(dir);
	if (!fs::exists(path))
	{
		return -1;
	}
	fs::directory_iterator end_iter;
	for (fs::directory_iterator iter(path); iter != end_iter; ++iter)
	{
		if (fs::is_regular_file(iter->status()))
		{
			filenames.push_back(iter->path());
		}
		if (fs::is_directory(iter->status()))
		{
			get_filenames(iter->path(), filenames);
		}
	}
	return filenames.size();
}

string erase_cin_getline_quotation(string str)
{
	size_t first_one = str.find_first_of("\"");
	if (first_one != string::npos)
	{
		str.erase(first_one, first_one + 1);
	}
	size_t last_one = str.find_last_of("\"");
	if (last_one != string::npos)
	{
		str.erase(last_one, last_one + 1);
	}
	return str;
}
