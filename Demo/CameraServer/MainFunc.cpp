// MainFunc.cpp : 定义控制台应用程序的入口点. 
//
//#include "MessageServer.h"
//#include "VisionStruct.h"

//#include "CommonFunc.h"
#include "CommonInclude.h"

#include "DirWatcher.h"
#include "stdafx.h"
int main(int argc, char *argv[])
{
	DirWatchServer dirWatchServer;
	dirWatchServer.RegPath(L"D:\\TST\\");
	dirWatchServer.Create();
	dirWatchServer.Resume();


	//MessageServer messageServer;
	//messageServer.Initial(103, "127.0.0.1", 105, "127.0.0.1", 106, "127.0.0.1");
	//messageServer.Create();
	//messageServer.Resume();



	HUGEWSTR  wstring;
	HUGESTR  string;
	while (1)
	{
		wstring[0] = 0;
		dirWatchServer.ToString(wstring, 2, 33);
		/*CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"\033[;32m%s\033[0m\n", wstring2);*/
		CCommonFunc::UnicodeToAnsi(wstring, string, TMTV_HUGESTRLEN);
		system("cls");
		printf(string);
		string[0] = 0;
		//messageServer.ToString(string, 2, 33);
		//printf(string);
		Sleep(1000);
	}
	return 0;
}