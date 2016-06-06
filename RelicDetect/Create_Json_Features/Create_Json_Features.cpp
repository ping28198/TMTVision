// Create_Json_Features.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "RelicObj.h"
#include <filesystem>
#include <string>
#include <vector>
using namespace std::tr2::sys;  //<filesystem>的命名空间  
using namespace std;

namespace fs = std::tr2::sys;

int get_filenames(const std::string& dir, std::vector<std::string>& filenames);

int main()
{
	cout << "输入一个存储目标物体的目录" << endl;
	string default_path;
	getline(cin, default_path);
	path p(default_path);
	cout << system_complete(p) << endl;
	vector<string> filenames;
	get_filenames(system_complete(p).string(), filenames);
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

