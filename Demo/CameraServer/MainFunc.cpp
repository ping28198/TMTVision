// MainFunc.cpp : 定义控制台应用程序的入口点. 
//

#include "stdafx.h"
#include "VisionStruct.h"
#include "MessageServer.h"
MessageServer messageServer;
int main(int argc, char *argv[])
{
	messageServer.LoadSetting(L"");
	messageServer.Create();
	messageServer.Resume();
	return 0;
}