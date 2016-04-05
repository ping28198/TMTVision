// ClassTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DirWatcher.h"
#include "CommonFunc.h"

#   define COL(x)  "\033[;" #x "m"  
#   define RED     COL(31)  
#   define GREEN   COL(32)  
#   define YELLOW  COL(33)  
#   define BLUE    COL(34)  
#   define MAGENTA COL(35)  
#   define CYAN    COL(36)  
#   define WHITE   COL(0)  
#   define GRAY    "\033[0m"  

int main()
{
	//setlocale(LC_ALL, "chs");
	DirWatchServer dirWatchServer;
	dirWatchServer.RegPath(L"D:\\TST\\");
	dirWatchServer.Create();
	dirWatchServer.Resume();
	HUGEWSTR  wstring;
	HUGEWSTR  wstring2;
	HUGESTR  string;
	while (1)
	{
		wstring[0] = 0;
		wstring2[0] = 0;
		dirWatchServer.ToString(wstring,2,33);
		/*CCommonFunc::SafeStringPrintf(wstring, TMTV_HUGESTRLEN, L"\033[;32m%s\033[0m\n", wstring2);*/
		CCommonFunc::UnicodeToAnsi(wstring, string, TMTV_HUGESTRLEN);
		system("cls");
		printf(string);
		Sleep(1000);
	}
    return 0;
}

