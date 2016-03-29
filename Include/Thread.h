///<proj_info>
//==============================================================================
// 项目名 ：智能监控
// 文件名 ：Thread.h
// 作  者 ：王磊
// 用  途 ：线程类,任务池
//			1:声明后创建线程并执行ThreadMain,线程在ThreadMain退出时销毁,
//			2:线程的核心是ThreadMain主函数中负责决定线程是否退出或挂起,
//			  线程执行后可以手动挂起和更改线程 任务处理函数,
//			  当任务函数被置零时,线程挂起,等待新的 任务处理函数
//			3:线程主函数重复执行任务处理函数,主函数后包含中断处理
// 版  权 ：霍比特人
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     王磊        2014.5.1   整合旧版本
//2.0     王磊        2016.3.29  添加父线程句柄 m_hParent
//                               线程ID改为静态以便为对象计数 m_nThreadID
//                               添加每次任务执行后等待时间 m_waiteTime
//								 测试
//==============================================================================
///</ver_info>

#pragma once
#include "Windows.h"
#ifndef MIN
#define MIN(i,j) i<j?i:j
#endif
#ifndef MAX
#define MAX(i,j) i<j?j:i
#endif
#ifndef MOD
#define MOD(i,j) i%j==j?0:i%j
#endif

int Mod(int i,int j);

//任务函数
#ifndef TASKTHREADFUN
#define TASKTHREADFUN
typedef void (__stdcall* ThreadTaskFun)(void* pPara);
#endif

///<data_info>
//==============================================================================
//功能描述:线程池类
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
//==============================================================================
///</data_info>
class Thread
{
private:
	//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
	static void ThreadMain(void* thisObj);
	static DWORD m_nThreadID;//2.0
public:
	bool isEx;
	HANDLE  m_hThread;
	HANDLE  m_hParent;//2.0
	ThreadTaskFun p_Task;
	void*  p_Para;
	HANDLE  m_hEvt;
	bool  m_bExit;
	int m_times;
	long m_waiteTime;
	enum STATUS{enAvialable = 0, enRunning, enSuspend, enExit, enCorpse, enDead};
	STATUS m_Status;
	//声明时创建资源
	Thread(HANDLE  hParent=0);//2.0
	//析构时设置参数等待线程主函数返回,需要等待1秒
	~Thread(void);
	//创建线程
	void  Create(int times=-1,long waiteTime=0);//2.0
	//继续执行挂起的线程
	void  Resume(void);
	//挂起线程
	void  Suspend(void);
	//设置参数使主函数退出以销毁线程
	void  Destroy(void);
	//注销任务函数//只在循环中间有效
	void  FreeTask(void);
	//注册任务函数
	void  RegTask(ThreadTaskFun pFunc, void* pPara);
	//强制结束当前线程
	void  ForceEnd(void);
	//强制结束当前线程,并创建新线程
};

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

///<data_info>
//==============================================================================
//功能描述:含有私有任务队列的线程池类
// 1:结合线程池功能,在对象私有存储空间内设有任务队列
// 2:在主函数中添加任务获取机制
// 3:可以由任务函数内部或主线程发送新任务到指定线程的任务队列,
//    可以有主进程和线程类中的各个线程发送新任务
// 4:任务队列中单一任务包含执行函数指针,和函数参数指针,
//    其中为避免函数参数空间的申请和销毁问题,参数保存在固定长度的数组中
//==============================================================================
///</data_info>
class ThreadEx :public Thread
{
public:
	enum {TASKNUM = 256};
	ThreadTask m_TastList[256];
	//使用任务队列
	int p_TaskListHead;
	int p_TaskListTail;
	int m_TaskNum;
	//CTaskCriticalSection taskCritalSection;
	CRITICAL_SECTION m_section;
	bool AddTailTask(ThreadTask _Task)
	{
		OutputDebugString(L"<AddTailTask()>\n");
		_Task.status=ThreadTask::enFree;
		//检查队列情况
		if (p_TaskListHead==Mod((p_TaskListTail+1),TASKNUM)|| m_TaskNum>=TASKNUM)
		{
			OutputDebugString(L"TaskListFull\n");
			return false;
		}
		int tmpTail=0;
		wchar_t debugStr[64]={0};
		wsprintf(debugStr,L"TaskList:Head=%d,Tail=%d,Num=%d\n",p_TaskListHead,p_TaskListTail,m_TaskNum);
		OutputDebugString(debugStr);
		::EnterCriticalSection(&m_section);
		//taskCritalSection.Enter();
		memcpy(m_TastList+p_TaskListTail,&_Task,sizeof(ThreadTask));
		p_TaskListTail=Mod(p_TaskListTail+1,TASKNUM);
		m_TaskNum++;
		//taskCritalSection.Leave();
		::LeaveCriticalSection(&m_section);
		OutputDebugString(L"TaskListAdd\n");
		wsprintf(debugStr,L"TaskList:Head=%d,Tail=%d,Num=%d\n",p_TaskListHead,p_TaskListTail,m_TaskNum);
		OutputDebugString(debugStr);
		OutputDebugString(L"<~AddTailTask()>\n\n");
		Resume();
		return true;
	}
	bool DelHeadTask()
	{
		OutputDebugString(L"<DelHeadTask()>\n");
		//检查队列情况
		if (p_TaskListHead==p_TaskListTail-1)
		{
			OutputDebugString(L"TaskListEmpty\n");
			return false;
		}
		if (m_TastList[p_TaskListHead].status==ThreadTask::enBusy)
		{
			OutputDebugString(L"TaskListHead is Busy\n");
			return false;
		}
		if (m_TastList[p_TaskListHead].status==ThreadTask::enNull)
		{
			OutputDebugString(L"TaskListHead is Null\n");//队头任务已经执行完并释放
		}
		wchar_t debugStr[64]={0};
		wsprintf(debugStr,L"TaskList:Head=%d,Tail=%d,Num=%d\n",p_TaskListHead,p_TaskListTail,m_TaskNum);
		OutputDebugString(debugStr);
		//taskCritalSection.Enter();
		::EnterCriticalSection(&m_section);
		p_TaskListHead=Mod((p_TaskListHead+1),TASKNUM);
		m_TaskNum--;
		//taskCritalSection.Leave();
		::LeaveCriticalSection(&m_section);
		OutputDebugString(L"TaskListHead is Delleted\n");
		wsprintf(debugStr,L"TaskList:Head=%d,Tail=%d,Num=%d\n",p_TaskListHead,p_TaskListTail,m_TaskNum);
		OutputDebugString(debugStr);
		OutputDebugString(L"<~DelHeadTask()>\n\n");
		return true;
	}
private:
	//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
	static void ThreadMain(void* thisObj);
public:
	//声明时创建线程
	ThreadEx();
	//析构时设置参数等待线程主函数返回,需要等待1秒
	~ThreadEx();
	//创建线程
	void Create(int times=-1);
	//强制删除线程,将当前任务卸载,重启线程,并返回新的线程指针
	void  Recover(void);
};