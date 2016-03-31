#pragma once
#include "Thread.h"
class ThreadA :
	public Thread
{
public:
	ThreadA(){ runTimes = 0; }
	~ThreadA(){}
	static long runTimes;
	//ÈÎÎñº¯Êý
	void  Task(void)
	{
		printf("ThreadA::Task(%d)\n", runTimes);
		wchar_t printStr[256];
		wsprintf(printStr, L"ThreadA::Task(%d)\n", runTimes);
		OutputDebugString(printStr);
		runTimes++;
	}
};
long ThreadA::runTimes = 0;
