#pragma once
#include "Thread.h"
class ThreadB :
	public Thread
{
public:
	ThreadB() { runTimes = 0; }
	~ThreadB() {}
	static long runTimes;
	//ÈÎÎñº¯Êý
	void  Task(void)
	{
		printf("ThreadB::Task(%d)\n", runTimes);
		wchar_t printStr[256];
		wsprintf(printStr, L"ThreadB::Task(%d)\n", runTimes);
		OutputDebugString(printStr);
		runTimes++;
	}
};
long ThreadB::runTimes=0;
