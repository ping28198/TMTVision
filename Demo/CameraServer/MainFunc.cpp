// MainFunc.cpp : �������̨Ӧ�ó������ڵ�. 
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