#pragma once
#ifndef __ITHREAD_H__
#define __ITHREAD_H__
#endif

#ifdef ITHREAD_EXPORTS
#define ITHREAD_API __declspec(dllexport)
#else
#define ITHREAD_API __declspec(dllimport)
#endif

//任务函数
#ifndef TASKTHREADFUN
#define TASKTHREADFUN
typedef void (__stdcall* ThreadTaskFun)(void* pPara);
#endif

//获得总线程数
ITHREAD_API void GetThreadInfo(void* pThread, void* &hThread, unsigned long &IDThread, int& IDClass);



//单任务线程创建
ITHREAD_API void* TaskThreadCreate(HANDLE  hParent = 0, int times=-1,long waiteTime = 0);
//继续执行挂起的线程
ITHREAD_API void  TaskThreadResume(void*pThread);
//挂起线程
ITHREAD_API void  TaskThreadSuspend(void*pThread);
//设置参数使主函数退出以销毁线程
ITHREAD_API void  TaskThreadDestroy(void*pThread);
//注销任务函数
ITHREAD_API void  TaskThreadFreeTask(void*pThread);
//注册任务函数
ITHREAD_API void  TaskThreadRegTask(void* pThread,ThreadTaskFun pFunc, void* pPara);
//线程外强制结束当前线程
ITHREAD_API void  TaskThreadForceEnd(void* pThread);

//任务队列线程创建
ITHREAD_API void* TaskThreadExCreate(HANDLE  hParent = 0, int times = -1, long waiteTime = 0);
//继续执行挂起的线程
ITHREAD_API void  TaskThreadExResume(void*pThread);
//挂起线程
ITHREAD_API void  TaskThreadExSuspend(void*pThread);
//设置参数使主函数退出以销毁线程
ITHREAD_API void  TaskThreadExDestroy(void*pThread);
//强制结束当前线程
ITHREAD_API void  TaskThreadExForceEnd(void* pThread);
//强制删除线程,将当前任务卸载,重启线程
ITHREAD_API void TaskThreadExRecover(void* pThread);

//线程任务
#ifndef THREADTASKSTRUCT
#define THREADTASKSTRUCT
typedef struct tagThreadTask
{
	//enum {PARASIZE = 256};
	char  charFun[32];//函数描述与当前描述相同时,使用当前函数
	//char pPara[PARASIZE];
	void* pPara;
	ThreadTaskFun pFun;//任务函数指针为0时使用当前函数
	enum TASKSTATUS{enNull= 0, enFree, enBusy};
	TASKSTATUS status;
	int life;
	//void* nextTask;
}ThreadTask, *PThreadTask;
#endif
//添加任务函数
ITHREAD_API void  TaskThreadExAddTask(void* pThread,ThreadTaskFun pFun,void* pPara);