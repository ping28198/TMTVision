//#include "stdafx.h"
#include "Thread.h"
#include <process.h>
#include "ThreadTimelimited.h"

int Mod(int i,int j)
{
	int tmp=i%j;
	if (tmp==j)
	{
		tmp=0;
	}
	return tmp;
}
//---------------------------------------------------------
//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
void Thread::ThreadMain(void* thisObj)
{
	OutputDebugString(L"Thread::ThreadMain Enter\n");
	Thread* pThisObj = (Thread*)thisObj;
	while(!pThisObj->m_bExit && pThisObj->m_times!=0)//中断0:退出
	{
		//任务处理部分
		if(pThisObj->p_Task)//中断1:释放
		{
			pThisObj->m_Status = Thread::enSuspend;
			::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//中断2:挂起
			pThisObj->m_Status = Thread::enRunning;
			pThisObj->p_Task(pThisObj->p_Para);
		}else
		{
			pThisObj->m_Status = Thread::enAvialable;			
			::ResetEvent(pThisObj->m_hEvt);
			::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//中断2:挂起
		}
		if (pThisObj->m_times>0)//中断3计数退出
		{
			pThisObj->m_times--;
		}
		Sleep(pThisObj->m_waiteTime);//2.0
	}
	pThisObj->p_Task=0;
	pThisObj->m_Status = Thread::enDead;
}
//---------------------------------------------------------
//线程计数ID :线程对象声明后+1,析构不改变
DWORD Thread::m_nThreadID = 0;//2.0
//---------------------------------------------------------
//声明时创建资源
Thread::Thread(HANDLE hParent)//2.0
{
	p_Task = 0;
	m_bExit = false;
	m_hEvt = 0;
	m_hThread = 0;
	m_times = -1;
	isEx = false;
	m_hParent = hParent;//2.0
	m_nThreadID = m_nThreadID + 1;//2.0
	m_waiteTime = 0;
}
//---------------------------------------------------------
//析构时设置参数等待线程主函数返回,需要等待1秒
Thread::~Thread(void)
{
	Destroy();
	Sleep(1000);//等待线程主函数退出并销毁线程
	if (m_hEvt!=0)
	{
		::CloseHandle(m_hEvt);
	}
}
//创建线程
void Thread::Create(int times, long waiteTime)//2.0
{
	if (m_hEvt == 0)
	{
		m_hEvt = ::CreateEvent(0, true, false, 0);
	}
	m_times = times;
	m_waiteTime = MAX(waiteTime,0);
	if (m_hThread == 0)
	{
		m_hThread = (HANDLE)_beginthread(ThreadMain, 0, this);
	}
}
//---------------------------------------------------------
//继续执行挂起的线程
void Thread::Resume(void)//中断1,2取消:释放
{
	if (m_hEvt!=0)
	{
		::SetEvent(m_hEvt);
	}
}
//---------------------------------------------------------
//挂起线程
void  Thread::Suspend(void)//中断2:挂起
{
	if (m_hEvt!=0)
	{
		::ResetEvent(m_hEvt);
	}
}

//---------------------------------------------------------
//设置参数使主函数退出以销毁线程
void  Thread::Destroy()//中断0:退出
{
	m_bExit = true;	
	p_Task = 0;
}
//---------------------------------------------------------
//注销任务函数
void  Thread::FreeTask()//中断1:释放
{
	p_Task = 0;
}
//---------------------------------------------------------
//注册任务函数
void  Thread::RegTask(ThreadTaskFun pFunc, void* pPara)//中断1取消条件:注册新任务
{
	p_Task = pFunc;
	p_Para = pPara;
}
//强制结束当前线程
void   Thread::ForceEnd(void)
{
	DWORD exitCode;
	if (GetExitCodeThread(m_hThread,&exitCode))
	{
		TerminateThread(m_hThread,exitCode);
	}
	Destroy();
}

//---------------------------------------------------------
//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
void ThreadEx::ThreadMain(void* thisObj)
{
	OutputDebugString(L"<ThreadEx::ThreadMain>\n");
	ThreadEx* pThisObj = (ThreadEx*)thisObj;
	while(!pThisObj->m_bExit && pThisObj->m_times!=0)//中断0:退出
	{
		//任务获取部分
		//判断任务队列是否为空
		if (pThisObj->p_TaskListHead==pThisObj->p_TaskListTail-1 || pThisObj->m_TaskNum<=0)
		{
			pThisObj->p_Task=0;//队列为空,设为中断1
			pThisObj->p_Para=0;
		}
		else
		{
			//pThisObj->p_Task=pThisObj->m_TastList[pThisObj->p_TaskListHead].pFun;
			//pThisObj->p_Para=pThisObj->m_TastList[pThisObj->p_TaskListHead].pPara;
			pThisObj->RegTask(pThisObj->m_TastList[pThisObj->p_TaskListHead].pFun,
				pThisObj->m_TastList[pThisObj->p_TaskListHead].pPara);
		}
		//任务处理部分
		pThisObj->m_Status = Thread::enSuspend;
		::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//中断2:挂起
		if(pThisObj->p_Task)//中断1:释放
		{
			pThisObj->m_Status = Thread::enRunning;
			OutputDebugString(L"\nThreadMain RunTask\n\n");
			pThisObj->p_Task(pThisObj->p_Para);
			//任务完成后处理
			pThisObj->p_Task=0;
			pThisObj->p_Para=0;
			pThisObj->DelHeadTask();//内设保护的删除队头任务的函数
		}else
		{
			pThisObj->m_Status = Thread::enAvialable;	
			//为满足任务获取环节,当前任务为空时,扫描任务队列头,而不挂起
			//::ResetEvent(pThisObj->m_hEvt);
			//::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//中断2:挂起
		}
		if (pThisObj->m_times>0)//中断3计数退出
		{
			pThisObj->m_times--;
		}
	}
	pThisObj->p_Task=0;
	pThisObj->m_Status = Thread::enDead;
	OutputDebugString(L"<~ThreadEx::ThreadMain>\n");
}
//声明时创建线程
ThreadEx::ThreadEx()
{
	p_TaskListHead=0;
	p_TaskListTail=p_TaskListHead;
	m_TaskNum=0;
	p_Task = 0;
	isEx=true;
	::InitializeCriticalSection(&m_section);
}
//析构时设置参数等待线程主函数返回,需要等待1秒
ThreadEx::~ThreadEx()
{
	Destroy();
	Sleep(1000);//等待线程主函数退出并销毁线程
	if (m_hEvt!=0)
	{
		::CloseHandle(m_hEvt);
	}
	::DeleteCriticalSection(&m_section);
}
//创建线程
void ThreadEx::Create(int times)
{
	if (m_hEvt==0)
	{
		m_hEvt = ::CreateEvent(0, true, false, 0);
	}
	m_times=times;
	if (m_hThread==0)
	{
		m_hThread = (HANDLE)_beginthread(ThreadMain, 0, this);
	}
}
void ThreadEx::Recover(void)
{
	ForceEnd();
	Sleep(1000);
	Destroy();
	if (m_hEvt!=0)
	{
		::CloseHandle(m_hEvt);
		m_hEvt=0;
	}
	::DeleteCriticalSection(&m_section);
	::InitializeCriticalSection(&m_section);
	DelHeadTask();
	Create();
	Resume();
}