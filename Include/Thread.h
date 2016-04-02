///<proj_info>
//==============================================================================
// 项目名 ：智能监控
// 文件名 ：Thread.h
// 作  者 ：王磊
// 用  途 ：Thread       简单线程类,可派生重载任务函数 
//          TaskThread   单任务线程类,可动态变更任务函数  
//          TaskThreadEx 队列任务线程类,可动态变更任务函数队列
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
//3.0     王磊        2016.3.29  整理线程类分层功能
//==============================================================================
///</ver_info>

///<algorithm_info>
//==============================================================================
//功能描述:线程类状态空间转换:
//
//
//             Thread()━▶ ┏━━━━━━━┓ ◀━~Thread()
//                         ┃    enExit    ┃
//                         ┗━━━━━━━┛ ◀━ForceEnd()
//                           ┃          ▲
//                           ▼          ┃
//            Create()━▶ m_bExit==1  m_bExit==0 ◀━ Destroy()
//                           ┃          ▲
//                           ▼          ┃
//                         ┏━━━━━━━┓
//                         ┃ enAvialable  ┃
//                         ┗━━━━━━━┛
//                           ┃          ▲
//                           ▼          ┃
//                         p_Task      p_Task
//            RegTask()━▶ !=NUll      ==NUllL ◀━ FreeTask()
//                           ┃          ▲
//                           ▼          ┃
//                         ┏━━━━━━━┓
//                         ┃  enRunning   ┃
//                         ┗━━━━━━━┛
//                           ┃          ▲
//                           ▼          ┃
//                         m_hEvt       m_hEvt
//            Suspend()━▶ Unexist      Exist  ◀━ Resume()
//                           ┃          ▲
//                           ▼          ┃
//                         ┏━━━━━━━┓
//                         ┃  enSuspend   ┃
//                         ┗━━━━━━━┛
//
//==============================================================================
///</algorithm_info>

///<header_info>
//==============================================================================
#pragma once
#include "Windows.h"
#include <process.h>
#ifndef MIN
#define MIN(i,j) i<j?i:j
#endif
#ifndef MAX
#define MAX(i,j) i<j?j:i
#endif
#ifndef MOD
#define MOD(i,j) i%j==j?0:i%j
#endif
int Mod(int i, int j);
//==============================================================================
///</header_info>

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
class Thread//3.0
{
//类功能
public:
	//线程计数ID :线程对象声明后+1,析构不改变
	static DWORD m_nThreadID;//2.0
	//对象类型记录ID
	const int m_classID = 0;//3.0
	HANDLE  m_hThread;
	HANDLE  m_hParent;//2.0
	HANDLE  m_hEvt;
protected:
	bool  m_bExit;
	int m_times;
	long m_waiteTime;
public:
	enum THSTATUS { enAvialable = 0, enRunning, enSuspend, enExit, enCorpse};
	THSTATUS m_ThStatus;
	//声明时创建资源
	Thread(HANDLE  hParent = 0);
	//析构时设置参数等待线程主函数返回,需要等待1秒
	~Thread(void);
//线程功能
private:
	//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
	static void ThreadMain(void* thisObj);
public:
	//创建线程
	void  Create(int times = -1, long waiteTime = 0);
	//继续执行挂起的线程
	void  Resume(void);
	//挂起线程
	void  Suspend(void);
	//设置参数使主函数退出以销毁线程
	void  Destroy(void);
	//调用WindowsAPI强制结束当前线程
	void  ForceEnd(void);
//任务功能
public:
	//防止p_Task,p_Para访问冲突
	CRITICAL_SECTION m_section;
	//虚任务函数,派生类重载此函数,静态ThreadMain函数会调用派生类的Task()
	virtual void  Task(void);
};
//==============================================================================
///</class_info>



///<datastruct_info>
//==============================================================================
//任务函数
#ifndef TASKTHREADFUN
#define TASKTHREADFUN
typedef void(__stdcall* ThreadTaskFun)(void* pPara);
#endif
//==============================================================================
///</datastruct_info>

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
class TaskThread :public Thread
{
//类功能
public:
	const int m_classID = 1;//3.0 对象类型记录
    //声明时创建资源
	TaskThread(HANDLE  hParent = 0);
	//析构时设置参数等待线程主函数返回,需要等待1秒
	~TaskThread();
//线程功能
private:
	//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
	static void ThreadMain(void* thisObj);
public:
	//创建线程
	void  Create(int times = -1, long waiteTime = 0);
//任务功能
protected:
	ThreadTaskFun p_Task;
	void*  p_Para;
public:
	//注销任务函数//只在循环中间有效
	void  FreeTask(void);
	//注册任务函数
	void  RegTask(ThreadTaskFun pFunc, void* pPara);
};
//==============================================================================
///</class_info>


///<datastruct_info>
//==============================================================================
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
	enum TASKSTATUS { enNull = 0, enFree, enBusy };
	TASKSTATUS status;
	int life;
	//void* nextTask;
}ThreadTask, *PThreadTask;
#endif
//==============================================================================
///</datastruct_info>

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
//┃  ↖  │ ↖     ┃ <-循环队列,固定长度,循环存储
//┃空任务│空任务  ┃ 
//┃任务0 │参数0   ┃ <-队列头线程执行后下移,或DelHeadTask删除
//┃任务1 │参数1   ┃ <-队列等待中的任务
//┃  ：  │  ：    ┃
//┃任务n │参数n   ┃ <-队列尾可以用AddTailTask添加.
//┃  ：  │  ：    ┃
//┃空任务│空参数  ┃
//┃  ↖  │ ↖     ┃ <-循环队列,固定长度,循环存储
//┠───┴────┨       
//┃中断处理        ┃ <-确定主函数是否关起退出,ForceEnd中断0强制退出,m_times中断3计数退出
//┗━━━━━━━━┛       Suspend中断2挂起等待,主函数循环暂停.
//                           或TaskList任务为空,中断1卸载函数,主函数循环等待新函数
class TaskThreadEx :public TaskThread
{
//类功能
public:
	const int m_classID = 2;//3.0 对象类型记
	//声明时创建线程
	TaskThreadEx(HANDLE  hParent = 0);
//线程功能
private:
	//线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
	static void ThreadMain(void* thisObj);
public:
	//创建线程
	void  Create(int times = -1, long waiteTime = 0);
	//强制删除线程,将当前任务卸载,重启线程,并返回新的线程指针
	void  Recover(void);
//任务功能
protected:
	enum { TASKNUM = 256 };
	ThreadTask m_TastList[256];
	//使用任务队列
	int p_TaskListHead;
	int p_TaskListTail;
	int m_TaskNum;
public:
	bool AddTailTask(ThreadTask _Task);
	bool DelHeadTask();
};
//==============================================================================
///</class_info>