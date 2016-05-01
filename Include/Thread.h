///////////////////////////////////////////////////
/** \file Thread.h
 *  \brief Thread Class and TaskThread Class
 *
 *  \note
 ** Status:
 *
 *              Thread()━▶ ┏━━━━━━━┓ ◀━~Thread()
 *                          ┃    TH_EXIT   ┃
 *                          ┗━━━━━━━┛ ◀━ForceEnd()
 *                            ┃          ▲
 *                            ▼          ┃
 *             Create()━▶ m_bExit==1  m_bExit==0 ◀━ Destroy()
 *                            ┃          ▲
 *                            ▼          ┃
 *                          ┏━━━━━━━┓
 *                          ┃ TH_AVIALABLE ┃
 *                          ┗━━━━━━━┛
 *                            ┃          ▲
 *                            ▼          ┃
 *                          p_Task      p_Task
 *             RegTask()━▶ !=NUll      ==NUllL ◀━ FreeTask()
 *                            ┃          ▲
 *                            ▼          ┃
 *                          ┏━━━━━━━┓
 *                          ┃  TH_RUNNING  ┃
 *                          ┗━━━━━━━┛
 *                            ┃          ▲
 *                            ▼          ┃
 *                          m_hEvt       m_hEvt
 *             Suspend()━▶ Unexist      Exist  ◀━ Resume()
 *          Destroy()invalide ┃          ▲
 *          ForceEnd() valide ▼          ┃
 *                          ┏━━━━━━━┓
 *                          ┃  TH_SUSPEND   ┃
 *                          ┗━━━━━━━┛
 *
 *  \author Leon Contact: towanglei@163.com
 *  \copyright TMTeam
 *  \version 3.1
 *  \History:
 *     2016/04/29 21:17 Fix comments.\n
 *     3.0 : Leon 2016/03/29 06:00 Add m_includeTaskTime = true/false means task time include in m_waiteTime or not.\n
 *     2.0 : Leon 2016/03/29 06:00 Add m_hParent,m_nThreadID,m_waiteTime.\n
 *     1.0 : Leon 2014/05/01 19:03 build.
 *
 */
///////////////////////////////////////////////////

//////////////////////////////////////////////////
#pragma once
#include <Windows.h>
#include <process.h>
#include "CommonDefine.h"
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


///////////////////////////////////////////////////
/** \class Thread : 
 *  \brief 简单线程类,可派生重载任务函数
 *  \note
 *  线程池功能:\n
 *  1:声明后创建线程并执行ThreadMain,线程在ThreadMain退出时销毁,\n
 *  2:线程的核心是ThreadMain主函数中负责决定线程是否退出或挂起,\n
 *    线程执行后可以手动挂起和更改线程 任务处理函数,\n
 *    派生类重载任务函数以实现不同功能\n
 *    当任务函数计数值=0,线程退出,计数初值设为-1,不计数\n
 *  3:线程主函数重复执行任务处理函数,主函数后包含中断处理\n
 *  ┏━━━━━━━━┓\n
 *  ┃线程            ┃ <-随对象声明创建,析构销毁\n
 *  ┠────────┨\n
 *  ┃线程主函数      ┃ <-线程创建Create后开始执行,处理任务Task和消息,主函数退出后线程销毁\n
 *  ┠───┬────┨\n
 *  ┃任务函数        ┃ <-线程执行中,循环执行任务函数并计数执行次数\n
 *  ┠───┴────┨\n
 *  ┃中断处理        ┃ <-确定主函数是否关起退出,ForceEnd中断0强制退出,m_times中断3计数退出\n
 *  ┗━━━━━━━━┛       Suspend中断2挂起等待,主函数循环暂停\n
 *  \author Leon Contact: towanglei@163.com
 *  \version 3.0
 *  \date 2016/04/29 22:34
 */
class Thread
{
//类功能
public:
	/// 线程计数ID :线程对象声明后+1,析构不改变
	static DWORD m_nThreadID;//2.0
	/// 对象类型记录ID
	const int m_classID = 0;//3.0
	HANDLE  m_hThread;
	HANDLE  m_hParent;//2.0
	HANDLE  m_hEvt;
protected:
	bool m_bExit;
	int m_times;
	long m_waiteTime;
	bool m_includeTaskTime;//3.1
public:
	//////////////////////////////////////////////////
	/** \enum THSTATUS
	 *  \brief Thread status 
	 *
     *  \note
	 ** Status:
	 *
	 *              Thread()━▶ ┏━━━━━━━┓ ◀━~Thread()
	 *                          ┃    TH_EXIT   ┃
	 *                          ┗━━━━━━━┛ ◀━ForceEnd()
	 *                            ┃          ▲
	 *                            ▼          ┃
	 *             Create()━▶ m_bExit==1  m_bExit==0 ◀━ Destroy()
	 *                            ┃          ▲
	 *                            ▼          ┃
	 *                          ┏━━━━━━━┓
	 *                          ┃ TH_AVIALABLE ┃
	 *                          ┗━━━━━━━┛
	 *                            ┃          ▲
	 *                            ▼          ┃
	 *                          p_Task      p_Task
	 *             RegTask()━▶ !=NUll      ==NUllL ◀━ FreeTask()
	 *                            ┃          ▲
	 *                            ▼          ┃
	 *                          ┏━━━━━━━┓
	 *                          ┃  TH_RUNNING  ┃
	 *                          ┗━━━━━━━┛
	 *                            ┃          ▲
	 *                            ▼          ┃
	 *                          m_hEvt       m_hEvt
	 *             Suspend()━▶ Unexist      Exist  ◀━ Resume()
	 *          Destroy()invalide ┃          ▲
	 *          ForceEnd() valide ▼          ┃
	 *                          ┏━━━━━━━┓
	 *                          ┃  TH_SUSPEND  ┃
	 *                          ┗━━━━━━━┛
	 *
	 */
	enum THSTATUS 
	{
		TH_EXIT=0, ///< Not initialed thread 
		TH_AVIALABLE, ///< Initialed thread but not run
		TH_RUNNING, ///< Running thread
		TH_SUSPEND ///< Suspend thread 
	};
	//////////////////////////////////////////////////

	/// Thread status
	THSTATUS m_ThStatus;
	/// Thread Setting
	ThSetting m_ThSetting;
	/// 声明时创建资源
	Thread(HANDLE  hParent = 0);
	/// 析构时设置参数等待线程主函数返回,需要等待1秒
	~Thread(void);
//线程功能
private:
	/// 线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
	static void ThreadMain(void* thisObj);
public:
	/// 创建线程
	void  Create(int times = -1, long waiteTime = 0, bool includeTaskTime = false);
	/// 继续执行挂起的线程
	void  Resume(void);
	/// 挂起线程
	void  Suspend(void);
	/// 设置参数使主函数退出以销毁线程
	void  Destroy(void);
	/// 调用WindowsAPI强制结束当前线程
	void  ForceEnd(void);
//任务功能
public:
	/// 防止p_Task,p_Para访问冲突
	CRITICAL_SECTION m_section;
	/// 虚任务函数,派生类重载此函数,静态ThreadMain函数会调用派生类的Task()
	virtual void  Task(void);
};
///////////////////////////////////////////////////


//////////////////////////////////////////////////
/** \struct ThreadTaskFun
 *  \brief TaskFun dealt by TaskThread
 *  \author Leon Contact: towanglei@163.com
 */
#ifndef TASKTHREADFUN
#define TASKTHREADFUN
typedef void(__stdcall* ThreadTaskFun)(void* pPara);
#endif
///////////////////////////////////////////////////


///////////////////////////////////////////////////
/** \class TaskThread :public Thread
 *  \brief 单任务线程类,可动态变更任务函数
 *  \note
 *  1:声明后创建线程并执行ThreadMain,线程在ThreadMain退出时销毁,\n
 *  2:线程的核心是ThreadMain主函数中负责决定线程是否退出或挂起,\n
 *    线程执行后可以手动挂起和更改线程 任务处理函数,\n
 *    当任务函数被置零时,线程挂起,等待新的 任务处理函数\n
 *    当任务函数计数值=0,线程退出,计数初值设为-1,不计数\n
 *  3:线程主函数重复执行任务处理函数,主函数后包含中断处理\n
 *  ┏━━━━━━━━┓\n
 *  ┃线程            ┃ <-随对象声明创建,析构销毁\n
 *  ┠────────┨\n
 *  ┃线程主函数      ┃ <-线程创建Create后开始执行,处理任务Task和消息,主函数退出后线程销毁\n
 *  ┠───┬────┨\n
 *  ┃任务  │参数    ┃ <-线程执行中,单一任务完成后,可以用RegTask改变,\n
 *  ┠───┴────┨       或用FreeTask释放任务,中断1卸载函数,主函数循环等待新函数\n
 *  ┃中断处理        ┃ <-确定主函数是否关起退出,ForceEnd中断0强制退出,m_times中断3计数退出\n
 *  ┗━━━━━━━━┛       Suspend中断2挂起等待,主函数循环暂停\n
 *  \author Leon Contact: towanglei@163.com
 *  \version 3.0
 *  \date 2016/04/29 22:39
 */
class TaskThread :public Thread
{
//类功能
public:
	const int m_classID = 1;///< 对象类型记录
    /// 声明时创建资源
	TaskThread(HANDLE  hParent = 0);
	/// 析构时设置参数等待线程主函数返回,需要等待1秒
	~TaskThread();
//线程功能
private:
	/// 线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
	static void ThreadMain(void* thisObj);
public:
	/// 创建线程
	void  Create(int times = -1, long waiteTime = 0, bool includeTaskTime = false);
//任务功能
protected:
	ThreadTaskFun p_Task;
	void*  p_Para;
public:
	/// 注销任务函数//只在循环中间有效
	void  FreeTask(void);
	/// 注册任务函数
	void  RegTask(ThreadTaskFun pFunc, void* pPara);
};
///////////////////////////////////////////////////


//////////////////////////////////////////////////
/** \struct ThreadTask
 *  \brief 线程任务
 *  \author Leon Contact: towanglei@163.com
 */
#ifndef THREADTASKSTRUCT
#define THREADTASKSTRUCT
typedef struct tagThreadTask
{
	//enum {PARASIZE = 256};
	char  charFun[32];///< 函数描述与当前描述相同时,使用当前函数
					  //char pPara[PARASIZE];
	void* pPara;
	ThreadTaskFun pFun;///< 任务函数指针为0时使用当前函数
	enum TASKSTATUS { enNull = 0, enFree, enBusy };
	TASKSTATUS status;
	int life;
	//void* nextTask;
}ThreadTask, *PThreadTask;
#endif
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class TaskThreadEx :public TaskThread
 *  \brief 队列任务线程类,可动态变更任务函数队列
 *  \note
 *  1:结合线程池功能,在对象私有存储空间内设有任务队列\n
 *  2:在主函数中添加任务获取机制\n
 *  3:可以由任务函数内部或主线程发送新任务到指定线程的任务队列,\n
 *     可以有主进程和线程类中的各个线程发送新任务\n
 *  4:任务队列中单一任务包含执行函数指针,和函数参数指针,\n
 *     其中为避免函数参数空间的申请和销毁问题,参数保存在固定长度的数组中\n
 *  ┏━━━━━━━━┓\n
 *  ┃线程            ┃ <-随对象声明创建,析构销毁\n
 *  ┠────────┨\n
 *  ┃线程主函数      ┃ <-线程创建Create后开始执行,处理任务Task和消息,主函数退出后线程销毁\n
 *  ┠───┬────┨\n
 *  ┃  ↘  │ ↘     ┃ <-循环队列,固定长度,循环存储\n
 *  ┃空任务│空任务  ┃\n
 *  ┃任务0 │参数0   ┃ <-队列头线程执行后下移,或DelHeadTask删除\n
 *  ┃任务1 │参数1   ┃ <-队列等待中的任务\n
 *  ┃  :   │  :     ┃\n
 *  ┃任务n │参数n   ┃ <-队列尾可以用AddTailTask添加.\n
 *  ┃  :   │  :     ┃\n
 *  ┃空任务│空参数  ┃\n
 *  ┃  ↙  │ ↙     ┃ <-循环队列,固定长度,循环存储\n
 *  ┠───┴────┨\n
 *  ┃中断处理        ┃ <-确定主函数是否关起退出,ForceEnd中断0强制退出,m_times中断3计数退出\n
 *  ┗━━━━━━━━┛       Suspend中断2挂起等待,主函数循环暂停.\n
 *                             或TaskList任务为空,中断1卸载函数,主函数循环等待新函数\n
 *  \author Leon Contact: towanglei@163.com
 *  \version 3.0
 *  \date 2016/04/29 22:52
 */
class TaskThreadEx :public TaskThread
{
//类功能
public:
	const int m_classID = 2;///< 对象类型标记
	/// 声明时创建线程
	TaskThreadEx(HANDLE  hParent = 0);
//线程功能
private:
	/// 线程主函数 :线程创建后开始执行,处理任务Task和消息,主函数退出后线程销毁
	static void ThreadMain(void* thisObj);
public:
	/// 创建线程
	void  Create(int times = -1, long waiteTime = 0, bool includeTaskTime = false);
	/// 强制删除线程,将当前任务卸载,重启线程,并返回新的线程指针
	void  Recover(void);
//任务功能
protected:
	enum { TASKNUM = 256 };
	ThreadTask m_TastList[256];
	/// 使用任务队列
	int p_TaskListHead;
	int p_TaskListTail;
	int m_TaskNum;
public:
	/// 添加任务进队列尾
	bool AddTailTask(ThreadTask _Task);
	/// 从队列头删除任务
	bool DelHeadTask();
};
///////////////////////////////////////////////////