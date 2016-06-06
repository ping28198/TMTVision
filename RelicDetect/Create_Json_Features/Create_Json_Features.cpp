// Create_Json_Features.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "RelicObj.h"
#include <filesystem>
#include <string>
#include <vector>
using namespace std::tr2::sys;  //<filesystem>�������ռ�  
using namespace std;

namespace fs = std::tr2::sys;

int get_filenames(const std::string& dir, std::vector<std::string>& filenames);

int main()
{
	cout << "����һ���洢Ŀ�������Ŀ¼" << endl;
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

