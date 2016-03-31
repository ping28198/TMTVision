#include "stdafx.h"
#include "Thread.h"
#include "IThread.h"
//获得总线程数
void GetThreadInfo(void* pThread,void* &hThread,unsigned long &IDThread, int& IDClass)
{
	TaskThread* p_Thread=(TaskThread*)pThread;
	hThread=p_Thread->m_hThread;
	IDThread=p_Thread->m_nThreadID;
	IDClass = p_Thread->m_classID;
}
//简单计数循环线程创建
void* TaskThreadCreate(HANDLE  hParent , int times , long waiteTime)
{
	TaskThread *p_Thread=new TaskThread(hParent);
	p_Thread->Create(times, waiteTime);
	return (void*)p_Thread;
}
//继续执行挂起的线程
void  TaskThreadResume(void*pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->Resume();
}
//挂起线程
void  TaskThreadSuspend(void*pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->Suspend();
}
//设置参数使主函数退出以销毁线程
void  TaskThreadDestroy(void*pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->Destroy();
	delete p_Thread;
}
//注销任务函数
void  TaskThreadFreeTask(void*pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->FreeTask();
}
//注册任务函数
void  TaskThreadRegTask(void* pThread,ThreadTaskFun pFunc, void* pPara)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->RegTask(pFunc,pPara);
}
//线程外强制结束当前线程
void  TaskThreadForceEnd(void* pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->ForceEnd();
}

//简单任务队列线程创建
void* TaskThreadExCreate(HANDLE  hParent , int times , long waiteTime)
{
	TaskThreadEx *p_Thread=new TaskThreadEx(hParent);
	p_Thread->Create(times, waiteTime);
	return (void*)p_Thread;
}
//继续执行挂起的线程
void  TaskThreadExResume(void*pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->Resume();
}
//挂起线程
void  TaskThreadExSuspend(void*pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->Suspend();
}
//设置参数使主函数退出以销毁线程
void  TaskThreadExDestroy(void*pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->Destroy();
}
//强制结束当前线程
void  TaskThreadExForceEnd(void* pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->ForceEnd();
}
//强制删除线程,将当前任务卸载,重启线程
void TaskThreadExRecover(void* pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->Recover();
}
//添加任务函数
void  TaskThreadExAddTask(void* pThread,ThreadTaskFun pFun,void* pPara)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	ThreadTask task;
	task.pFun=pFun;
	task.pPara=pPara;
	p_Thread->AddTailTask(task);
	p_Thread->Resume();
}