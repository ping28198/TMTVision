#include "Thread.h"
#include <process.h>
#include "ArWatch.h"
int Mod(int i,int j)
{
	int tmp=i%j;
	if (tmp==j)
	{
		tmp=0;
	}
	return tmp; 
}

///<class_info>
//==============================================================================
//功能描述:简单线程类,可派生重载任务函数 
//线程池功能:
//1:声明后创建线程并执行ThreadMain,线程在ThreadMain退出时销毁,
//2:线程的核心是ThreadMain主函数中负责决定线程是否退出或挂起,
//  线程执行后可以手动挂起和更改线程 任务处理函数,
//  派生类重载任务函数以实现不同功能
//  当任务函数计数值=0,线程退出,计数初值设为-1,不计数
//3:线程主函数重复执行任务处理函数,主函数后包含中断处理
//┏━━━━━━━━┓
//┃线程            ┃ <-随对象声明创建,析构销毁
//┠────────┨
//┃线程主函数      ┃ <-线程创建Create后开始执行,处理任务Task和消息,主函数退出后线程销毁
//┠───┬────┨
//┃任务函数        ┃ <-线程执行中,循环执行任务函数并计数执行次数
//┠───┴────┨ 
//┃中断处理        ┃ <-确定主函数是否关起退出,ForceEnd中断0强制退出,m_times中断3计数退出
//┗━━━━━━━━┛       Suspend中断2挂起等待,主函数循环暂停
//class Thread//3.0
//{
//类功能
//线程计数ID :线程对象声明后+1,析构不改变
DWORD Thread::m_nThreadID = 0;//2.0
	//声明时创建资源
Thread::Thread(HANDLE  hParent)//2.0
{
	::InitializeCriticalSection(&m_section);
	m_bExit = false;
	m_hEvt = 0;
	m_hThread = 0;
	m_times = -1;
	m_hParent = hParent;//2.0
	m_nThreadID = m_nThreadID + 1;//2.0
	m_waitTime = 0;
	m_includeTaskTime = false;//3.1
	m_ThStatus = TH_EXIT;
}
//析构时设置参数等待线程主函数返回,需要等待1秒
Thread::~Thread(void)
{
	Destroy();
	Sleep(1000);//等待线程主函数退出并销毁线程
	if (m_hEvt != 0)
	{
		::CloseHandle(m_hEvt);
	}
	m_ThStatus = TH_EXIT;
	::DeleteCriticalSection(&m_section);
}
//线程功能
//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
void Thread::ThreadMain(void* thisObj)
{
	OutputDebugString(L"<Thread::ThreadMain()>\n");
	Thread* pThisObj = (Thread*)thisObj;
	while (!pThisObj->m_bExit && pThisObj->m_times != 0)//中断0:退出
	{
		double taskTime = 0;//3.1
		//任务处理部分
		pThisObj->m_ThStatus = Thread::TH_SUSPEND;
		::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//中断2:挂起
		pThisObj->m_ThStatus = Thread::TH_RUNNING;
		//::EnterCriticalSection(&pThisObj->m_section);

		if (pThisObj->m_includeTaskTime)//3.1
		{
			ArWatch arWatch;
			arWatch.SetData(&taskTime);
			try
			{
				pThisObj->Task();//3.0
			}
			catch (...)
			{
				LeaveCriticalSection(&(pThisObj->m_section));
				OutputDebugString(L"<\\Thread::task() fail>\n");
			}
			
		}
		else
		{
			try
			{
				pThisObj->Task();//3.0
			}
			catch (...)
			{
				LeaveCriticalSection(&(pThisObj->m_section));
				OutputDebugString(L"<\\Thread::task() fail>\n");
			}
		}
		//::LeaveCriticalSection(&pThisObj->m_section);

		if (pThisObj->m_times>0)//中断3计数退出
		{
			pThisObj->m_times--;
		}
		Sleep(MAX(pThisObj->m_waitTime - taskTime, 0));//3.1
	}
	pThisObj->m_ThStatus = Thread::TH_EXIT;
	OutputDebugString(L"<\\Thread::ThreadMain()>\n");
}
//创建线程
void  Thread::Create(int times, long waiteTime, bool includeTaskTime)//2.0
{
	if (m_hEvt == 0)
	{
		m_hEvt = ::CreateEvent(0, true, false, 0);
	}
	if (m_hThread == 0)
	{
		m_times = times;
		m_bExit = false;
		m_waitTime = MAX(waiteTime, 0);
		m_includeTaskTime = includeTaskTime;//3.1
		m_hThread = (HANDLE)_beginthread(ThreadMain, 0, this);
		if (m_hThread != 0) m_ThStatus = TH_AVIALABLE;
		OutputDebugString(L"<Thread::Create()>\n");
	}
}
//继续执行挂起的线程
void  Thread::Resume(void)//中断1,2取消:释放
{
	if (m_hEvt != 0)
	{
		::SetEvent(m_hEvt);
	}
	OutputDebugString(L"<Thread::Resume()>\n");
}
//挂起线程
void  Thread::Suspend(void)//中断2:挂起
{
	if (m_hEvt != 0)
	{
		::ResetEvent(m_hEvt);
	}
	OutputDebugString(L"<Thread::Suspend()>\n");
}
//设置参数使主函数退出以销毁线程
void  Thread::Destroy(void)//中断0:退出
{
	m_bExit = true;
	Sleep(m_waitTime+100);
	m_ThStatus = TH_EXIT;
	OutputDebugString(L"<Thread::Destroy()>\n");
}
//调用WindowsAPI强制结束当前线程
void Thread::ForceEnd(void)
{
	DWORD exitCode;
	if (GetExitCodeThread(m_hThread, &exitCode))
	{
		TerminateThread(m_hThread, exitCode);
	}
	Destroy();
	OutputDebugString(L"<Thread::ForceEnd()>\n");
}
//任务功能
//虚任务函数,派生类重载此函数,静态ThreadMain函数会调用派生类的Task()
void  Thread::Task(void)
{
	//::EnterCriticalSection(&m_section);
	//::LeaveCriticalSection(&m_section);
	OutputDebugString(L"<Thread::Task()>\n");
}
//};
//==============================================================================
///</class_info>



///<class_info>
//==============================================================================
//功能描述:单任务线程类,可动态变更任务函数
//线程池功能:
//1:声明后创建线程并执行ThreadMain,线程在ThreadMain退出时销毁,
//2:线程的核心是ThreadMain主函数中负责决定线程是否退出或挂起,
//  线程执行后可以手动挂起和更改线程 任务处理函数,
//  当任务函数被置零时,线程挂起,等待新的 任务处理函数
//  当任务函数计数值=0,线程退出,计数初值设为-1,不计数
//3:线程主函数重复执行任务处理函数,主函数后包含中断处理
//┏━━━━━━━━┓
//┃线程            ┃ <-随对象声明创建,析构销毁
//┠────────┨
//┃线程主函数      ┃ <-线程创建Create后开始执行,处理任务Task和消息,主函数退出后线程销毁
//┠───┬────┨
//┃任务  │参数    ┃ <-线程执行中,单一任务完成后,可以用RegTask改变,
//┠───┴────┨       或用FreeTask释放任务,中断1卸载函数,主函数循环等待新函数
//┃中断处理        ┃ <-确定主函数是否关起退出,ForceEnd中断0强制退出,m_times中断3计数退出
//┗━━━━━━━━┛       Suspend中断2挂起等待,主函数循环暂停
//class TaskThread :public Thread
//{
//类功能
TaskThread::TaskThread(HANDLE  hParent)//2.0
{
	p_Task = 0;
	p_Para = 0;//注销时不能delete,以免父线程中内存泄漏
}
//析构时设置参数等待线程主函数返回,需要等待1秒
TaskThread::~TaskThread()
{
	ForceEnd();
}
//线程功能
//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
void TaskThread::ThreadMain(void* thisObj)
{
	OutputDebugString(L"<TaskThread::ThreadMain()>\n");
	TaskThread* pThisObj = (TaskThread*)thisObj;
	while (!pThisObj->m_bExit && pThisObj->m_times != 0)//中断0:退出
	{
		double taskTime = 0;//3.1
		//任务处理部分
		if (pThisObj->p_Task)//中断1:释放
		{
			pThisObj->m_ThStatus = Thread::TH_SUSPEND;
			::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//中断2:挂起
			pThisObj->m_ThStatus = Thread::TH_RUNNING;

			//::EnterCriticalSection(&pThisObj->m_section);
			if (pThisObj->m_includeTaskTime)//3.1
			{
				ArWatch arWatch;
				arWatch.SetData(&taskTime);
				pThisObj->p_Task(pThisObj->p_Para);//3.0
			}
			else
			{
				pThisObj->p_Task(pThisObj->p_Para);//3.0
			}
			//::LeaveCriticalSection(&pThisObj->m_section);

		}
		else
		{
			pThisObj->m_ThStatus = Thread::TH_AVIALABLE;
			::ResetEvent(pThisObj->m_hEvt);
			::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//中断2:挂起
		}
		if (pThisObj->m_times>0)//中断3计数退出
		{
			pThisObj->m_times--;
		}
		Sleep(MAX(pThisObj->m_waitTime - taskTime, 0));//3.1
	}
	pThisObj->p_Task = 0;
	pThisObj->m_ThStatus = Thread::TH_EXIT;
	OutputDebugString(L"<\\TaskThread::ThreadMain()>\n");
}
//创建线程
void TaskThread::Create(int times, long waiteTime, bool includeTaskTime)
{
	if (m_hEvt == 0)
	{
		m_hEvt = ::CreateEvent(0, true, false, 0);
	}
	if (m_hThread == 0)
	{
		m_times = times;
		m_waitTime = MAX(waiteTime, 0);
		m_includeTaskTime = includeTaskTime;//3.1
		m_hThread = (HANDLE)_beginthread(ThreadMain, 0, this);
		if (m_hThread != 0) m_ThStatus = TH_AVIALABLE;
		OutputDebugString(L"<TaskThread::Create()>\n");
	}
}
//任务功能
//注销任务函数//只在循环中间有效
void  TaskThread::FreeTask(void)
{
	::EnterCriticalSection(&m_section);
	p_Task = 0;
	::LeaveCriticalSection(&m_section);
	OutputDebugString(L"<TaskThread::FreeTask()>\n");
}
//注册任务函数
void  TaskThread::RegTask(ThreadTaskFun pFunc, void* pPara)
{
	::EnterCriticalSection(&m_section);
	p_Task = pFunc;
	p_Para = pPara;//内存由父线程管理
	::LeaveCriticalSection(&m_section);
	OutputDebugString(L"<TaskThread::RegTask()>\n");
}
//};
//==============================================================================
///</class_info>


///<class_info>
//==============================================================================
//功能描述:队列任务线程类,可动态变更任务函数队列
// 1:结合线程池功能,在对象私有存储空间内设有任务队列
// 2:在主函数中添加任务获取机制
// 3:可以由任务函数内部或主线程发送新任务到指定线程的任务队列,
//    可以有主进程和线程类中的各个线程发送新任务
// 4:任务队列中单一任务包含执行函数指针,和函数参数指针,
//    其中为避免函数参数空间的申请和销毁问题,参数保存在固定长度的数组中
//┏━━━━━━━━┓
//┃线程            ┃ <-随对象声明创建,析构销毁
//┠────────┨
//┃线程主函数      ┃ <-线程创建Create后开始执行,处理任务Task和消息,主函数退出后线程销毁
//┠───┬────┨
//┃  ↘  │ ↘     ┃ <-循环队列,固定长度,循环存储
//┃空任务│空任务  ┃ 
//┃任务0 │参数0   ┃ <-队列头线程执行后下移,或DelHeadTask删除
//┃任务1 │参数1   ┃ <-队列等待中的任务
//┃  :   │  :     ┃
//┃任务n │参数n   ┃ <-队列尾可以用AddTailTask添加.
//┃  :   │  :     ┃
//┃空任务│空参数  ┃
//┃  ↙  │ ↙     ┃ <-循环队列,固定长度,循环存储
//┠───┴────┨       
//┃中断处理        ┃ <-确定主函数是否关起退出,ForceEnd中断0强制退出,m_times中断3计数退出
//┗━━━━━━━━┛       Suspend中断2挂起等待,主函数循环暂停.
//                           或TaskList任务为空,中断1卸载函数,主函数循环等待新函数
//class TaskThreadEx :public TaskThread
//{
//类功能
TaskThreadEx::TaskThreadEx(HANDLE  hParent)
{
	p_TaskListHead = 0;
	p_TaskListTail = p_TaskListHead;
	m_TaskNum = 0;
	//p_Task = 0;
}
//线程功能
//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
void TaskThreadEx::ThreadMain(void* thisObj)
{
	OutputDebugString(L"<TaskThreadEx::ThreadMain()>\n");
	TaskThreadEx* pThisObj = (TaskThreadEx*)thisObj;
	while (!pThisObj->m_bExit && pThisObj->m_times != 0)//中断0:退出
	{
		double taskTime = 0;//3.1
		//任务获取部分
		//判断任务队列是否为空
		if (pThisObj->p_TaskListHead == pThisObj->p_TaskListTail - 1 || pThisObj->m_TaskNum <= 0)
		{
			pThisObj->p_Task = 0;//队列为空,设为中断1
			pThisObj->p_Para = 0;
		}
		else
		{
			pThisObj->RegTask(pThisObj->m_TastList[pThisObj->p_TaskListHead].pFun,
				pThisObj->m_TastList[pThisObj->p_TaskListHead].pPara);
		}
		//任务处理部分
		pThisObj->m_ThStatus = Thread::TH_SUSPEND;
		::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//中断2:挂起
		if (pThisObj->p_Task)//中断1:释放
		{
			pThisObj->m_ThStatus = Thread::TH_RUNNING;
			OutputDebugString(L"ThreadMain RunTask\n");

			//::EnterCriticalSection(&pThisObj->m_section);
			if (pThisObj->m_includeTaskTime)//3.1
			{
				ArWatch arWatch;
				arWatch.SetData(&taskTime);
				pThisObj->p_Task(pThisObj->p_Para);//3.0
			}
			else
			{
				pThisObj->p_Task(pThisObj->p_Para);//3.0
			}
			//::LeaveCriticalSection(&pThisObj->m_section);

			//任务完成后处理
			pThisObj->p_Task = 0;
			pThisObj->p_Para = 0;
			pThisObj->DelHeadTask();//内设保护的删除队头任务的函数
		}
		else
		{
			pThisObj->m_ThStatus = Thread::TH_AVIALABLE;
			//为满足任务获取环节,当前任务为空时,扫描任务队列头,而不挂起
			//::ResetEvent(pThisObj->m_hEvt);
			//::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//中断2:挂起
		}
		if (pThisObj->m_times>0)//中断3计数退出
		{
			pThisObj->m_times--;
		}
		Sleep(MAX(pThisObj->m_waitTime - taskTime, 0));//3.1
	}
	pThisObj->p_Task = 0;
	pThisObj->m_ThStatus = Thread::TH_EXIT;
	OutputDebugString(L"<\\TaskThreadEx::ThreadMain()>\n");
}
//创建线程
void TaskThreadEx::Create(int times, long waiteTime, bool includeTaskTime)
{
	if (m_hEvt == 0)
	{
		m_hEvt = ::CreateEvent(0, true, false, 0);
	}
	if (m_hThread == 0)
	{
		m_times = times;
		m_waitTime = MAX(waiteTime, 0);
		m_includeTaskTime = includeTaskTime;//3.1
		m_hThread = (HANDLE)_beginthread(ThreadMain, 0, this);
		if (m_hThread != 0) m_ThStatus = TH_AVIALABLE;
		OutputDebugString(L"<TaskThreadEx::Create()>\n");
	}
}
//强制删除线程,将当前任务卸载,重启线程,并返回新的线程指针
void  TaskThreadEx::Recover(void)
{
	ForceEnd();
	Sleep(1000);
	Destroy();
	if (m_hEvt != 0)
	{
		::CloseHandle(m_hEvt);
		m_hEvt = 0;
	}
	::DeleteCriticalSection(&m_section);
	::InitializeCriticalSection(&m_section);
	DelHeadTask();
	Create();
	Resume();
	OutputDebugString(L"<TaskThreadEx::Recover()>\n");
}
//任务功能
bool TaskThreadEx::AddTailTask(ThreadTask _Task)
{
	OutputDebugString(L"<AddTailTask()>\n");
	_Task.status = ThreadTask::enFree;
	//检查队列情况
	if (p_TaskListHead == Mod((p_TaskListTail + 1), TASKNUM) || m_TaskNum >= TASKNUM)
	{
		OutputDebugString(L"TaskListFull\n");
		return false;
	}
	int tmpTail = 0;
	wchar_t debugStr[64] = { 0 };
	wsprintf(debugStr, L"TaskList:Head=%d,Tail=%d,Num=%d\n", p_TaskListHead, p_TaskListTail, m_TaskNum);
	OutputDebugString(debugStr);
	::EnterCriticalSection(&m_section);
	//taskCritalSection.Enter();
	memcpy(m_TastList + p_TaskListTail, &_Task, sizeof(ThreadTask));
	p_TaskListTail = Mod(p_TaskListTail + 1, TASKNUM);
	m_TaskNum++;
	//taskCritalSection.Leave();
	::LeaveCriticalSection(&m_section);
	OutputDebugString(L"TaskListAdd\n");
	wsprintf(debugStr, L"TaskList:Head=%d,Tail=%d,Num=%d\n", p_TaskListHead, p_TaskListTail, m_TaskNum);
	OutputDebugString(debugStr);
	OutputDebugString(L"<\\AddTailTask()>\n");
	Resume();
	return true;
}
bool TaskThreadEx::DelHeadTask()
{
	OutputDebugString(L"<DelHeadTask()>\n");
	//检查队列情况
	if (p_TaskListHead == p_TaskListTail - 1)
	{
		OutputDebugString(L"TaskListEmpty\n");
		return false;
	}
	if (m_TastList[p_TaskListHead].status == ThreadTask::enBusy)
	{
		OutputDebugString(L"TaskListHead is Busy\n");
		return false;
	}
	if (m_TastList[p_TaskListHead].status == ThreadTask::enNull)
	{
		OutputDebugString(L"TaskListHead is Null\n");//队头任务已经执行完并释放
	}
	wchar_t debugStr[64] = { 0 };
	wsprintf(debugStr, L"TaskList:Head=%d,Tail=%d,Num=%d\n", p_TaskListHead, p_TaskListTail, m_TaskNum);
	OutputDebugString(debugStr);
	//taskCritalSection.Enter();
	::EnterCriticalSection(&m_section);
	p_TaskListHead = Mod((p_TaskListHead + 1), TASKNUM);
	m_TaskNum--;
	//taskCritalSection.Leave();
	::LeaveCriticalSection(&m_section);
	OutputDebugString(L"TaskListHead is Delleted\n");
	wsprintf(debugStr, L"TaskList:Head=%d,Tail=%d,Num=%d\n", p_TaskListHead, p_TaskListTail, m_TaskNum);
	OutputDebugString(debugStr);
	OutputDebugString(L"<\\DelHeadTask()>\n");
	return true;
}
//};
//==============================================================================
///</class_info>