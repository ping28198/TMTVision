///<proj_info>
//==============================================================================
// ��Ŀ�� �����ܼ��
// �ļ��� ��Thread.h
// ��  �� ������
// ��  ; ��Thread       ���߳���,���������������� 
//          TaskThread   �������߳���,�ɶ�̬���������  
//          TaskThreadEx ���������߳���,�ɶ�̬�������������
//			1:�����󴴽��̲߳�ִ��ThreadMain,�߳���ThreadMain�˳�ʱ����,
//			2:�̵߳ĺ�����ThreadMain�������и�������߳��Ƿ��˳������,
//			  �߳�ִ�к�����ֶ�����͸����߳� ��������,
//			  ��������������ʱ,�̹߳���,�ȴ��µ� ��������
//			3:�߳��������ظ�ִ����������,������������жϴ���
// ��  Ȩ ����������
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ����        2014.5.1   ���Ͼɰ汾
//2.0     ����        2016.3.29  ��Ӹ��߳̾�� m_hParent
//                               �߳�ID��Ϊ��̬�Ա�Ϊ������� m_nThreadID
//                               ���ÿ������ִ�к�ȴ�ʱ�� m_waiteTime
//3.0     ����        2016.3.29  �����߳���ֲ㹦��
//==============================================================================
///</ver_info>

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
int Mod(int i, int j)
{
	int tmp = i%j;
	if (tmp == j)
	{
		tmp = 0;
	}
	return tmp;
}
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//��������:���߳���,���������������� 
//�̳߳ع���:
//1:�����󴴽��̲߳�ִ��ThreadMain,�߳���ThreadMain�˳�ʱ����,
//2:�̵߳ĺ�����ThreadMain�������и�������߳��Ƿ��˳������,
//  �߳�ִ�к�����ֶ�����͸����߳� ��������,
//  ������������������ʵ�ֲ�ͬ����
//  ������������ֵ=0,�߳��˳�,������ֵ��Ϊ-1,������
//3:�߳��������ظ�ִ����������,������������жϴ���
//��������������������
//���߳�            �� <-�������������,��������
//�ĩ�����������������
//���߳�������      �� <-�̴߳���Create��ʼִ��,��������Task����Ϣ,�������˳����߳�����
//�ĩ������Щ���������
//��������        �� <-�߳�ִ����,ѭ��ִ��������������ִ�д���
//�ĩ������ة��������� 
//���жϴ���        �� <-ȷ���������Ƿ�����˳�,ForceEnd�ж�0ǿ���˳�,m_times�ж�3�����˳�
//��������������������       Suspend�ж�2����ȴ�,������ѭ����ͣ
class Thread//3.0
{
//�๦��
public:
	//�̼߳���ID :�̶߳���������+1,�������ı�
	static DWORD m_nThreadID;//2.0
	//�������ͼ�¼ID
	const int m_classID = 0;//3.0
	HANDLE  m_hThread;
	HANDLE  m_hParent;//2.0
	HANDLE  m_hEvt;
	bool  m_bExit;
	int m_times;
	long m_waiteTime;
	enum STATUS { enAvialable = 0, enRunning, enSuspend, enExit, enCorpse, enDead };
	STATUS m_Status;
	//����ʱ������Դ
	Thread(HANDLE  hParent = 0)//2.0
	{
		::InitializeCriticalSection(&m_section);
		m_bExit = false;
		m_hEvt = 0;
		m_hThread = 0;
		m_times = -1;
		m_hParent = hParent;//2.0
		m_nThreadID = m_nThreadID + 1;//2.0
		m_waiteTime = 0;
	}
	//����ʱ���ò����ȴ��߳�����������,��Ҫ�ȴ�1��
	~Thread(void)
	{
		Destroy();
		Sleep(1000);//�ȴ��߳��������˳��������߳�
		if (m_hEvt != 0)
		{
			::CloseHandle(m_hEvt);
		}
		::DeleteCriticalSection(&m_section);
	}
//�̹߳���
private:
	//�߳������� :�̴߳�����ʼִ��,��������Task����Ϣ,�������˳����߳�����
	static void ThreadMain(void* thisObj)
	{
		OutputDebugString(L"<Thread::ThreadMain()>\n");
		Thread* pThisObj = (Thread*)thisObj;
		while (!pThisObj->m_bExit && pThisObj->m_times != 0)//�ж�0:�˳�
		{
			//��������
			pThisObj->m_Status = Thread::enSuspend;
			::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//�ж�2:����
			pThisObj->m_Status = Thread::enRunning;
			pThisObj->Task();//3.0
			if (pThisObj->m_times>0)//�ж�3�����˳�
			{
				pThisObj->m_times--;
			}
			Sleep(pThisObj->m_waiteTime);//2.0
		}
		pThisObj->m_Status = Thread::enDead;
		OutputDebugString(L"<\\Thread::ThreadMain()>\n");
	}
public:
	//�����߳�
	void  Create(int times = -1, long waiteTime = 0)//2.0
	{
		if (m_hEvt == 0)
		{
			m_hEvt = ::CreateEvent(0, true, false, 0);
		}
		if (m_hThread == 0)
		{
			m_times = times;
			m_waiteTime = MAX(waiteTime, 0);
			m_hThread = (HANDLE)_beginthread(ThreadMain, 0, this);
			OutputDebugString(L"<Thread::Create()>\n");
		}
	}
	//����ִ�й�����߳�
	void  Resume(void)//�ж�1,2ȡ��:�ͷ�
	{
		if (m_hEvt != 0)
		{
			::SetEvent(m_hEvt);
		}
		OutputDebugString(L"<Thread::Resume()>\n");
	}
	//�����߳�
	void  Suspend(void)//�ж�2:����
	{
		if (m_hEvt != 0)
		{
			::ResetEvent(m_hEvt);
		}
		OutputDebugString(L"<Thread::Suspend()>\n");
	}
	//���ò���ʹ�������˳��������߳�
	void  Destroy(void)//�ж�0:�˳�
	{
		m_bExit = true;
		Sleep(100);
		OutputDebugString(L"<Thread::Destroy()>\n");
	}
	//����WindowsAPIǿ�ƽ�����ǰ�߳�
	void  ForceEnd(void) 
	{
		DWORD exitCode;
		if (GetExitCodeThread(m_hThread, &exitCode))
		{
			TerminateThread(m_hThread, exitCode);
		}
		Destroy();
		OutputDebugString(L"<Thread::ForceEnd()>\n");
	}
//������
public:
	//��ֹp_Task,p_Para���ʳ�ͻ
	CRITICAL_SECTION m_section;
	//��������,���������ش˺���,��̬ThreadMain����������������Task()
	virtual void  Task(void)
	{
		OutputDebugString(L"<Thread::Task()>\n");
	}
};
//�̼߳���ID :�̶߳���������+1,�������ı�
DWORD Thread::m_nThreadID = 0;//2.0
//==============================================================================
///</class_info>



///<datastruct_info>
//==============================================================================
//������
#ifndef TASKTHREADFUN
#define TASKTHREADFUN
typedef void(__stdcall* ThreadTaskFun)(void* pPara);
#endif
//==============================================================================
///</datastruct_info>

///<class_info>
//==============================================================================
//��������:�������߳���,�ɶ�̬���������
//�̳߳ع���:
//1:�����󴴽��̲߳�ִ��ThreadMain,�߳���ThreadMain�˳�ʱ����,
//2:�̵߳ĺ�����ThreadMain�������и�������߳��Ƿ��˳������,
//  �߳�ִ�к�����ֶ�����͸����߳� ��������,
//  ��������������ʱ,�̹߳���,�ȴ��µ� ��������
//  ������������ֵ=0,�߳��˳�,������ֵ��Ϊ-1,������
//3:�߳��������ظ�ִ����������,������������жϴ���
//��������������������
//���߳�            �� <-�������������,��������
//�ĩ�����������������
//���߳�������      �� <-�̴߳���Create��ʼִ��,��������Task����Ϣ,�������˳����߳�����
//�ĩ������Щ���������
//������  ������    �� <-�߳�ִ����,��һ������ɺ�,������RegTask�ı�,
//�ĩ������ة���������       ����FreeTask�ͷ�����,�ж�1ж�غ���,������ѭ���ȴ��º���
//���жϴ���        �� <-ȷ���������Ƿ�����˳�,ForceEnd�ж�0ǿ���˳�,m_times�ж�3�����˳�
//��������������������       Suspend�ж�2����ȴ�,������ѭ����ͣ
class TaskThread :public Thread
{
//�๦��
public:
	const int m_classID = 1;//3.0 �������ͼ�¼
    //����ʱ������Դ
	TaskThread(HANDLE  hParent = 0)//2.0
	{
		p_Task = 0;
		p_Para = 0;//ע��ʱ����delete,���⸸�߳����ڴ�й©
	}
	//����ʱ���ò����ȴ��߳�����������,��Ҫ�ȴ�1��
	~TaskThread()
	{
		ForceEnd();
	}
//�̹߳���
private:
	//�߳������� :�̴߳�����ʼִ��,��������Task����Ϣ,�������˳����߳�����
	static void ThreadMain(void* thisObj) 
	{
		OutputDebugString(L"<TaskThread::ThreadMain()>\n");
		TaskThread* pThisObj = (TaskThread*)thisObj;
		while (!pThisObj->m_bExit && pThisObj->m_times != 0)//�ж�0:�˳�
		{
			//��������
			if (pThisObj->p_Task)//�ж�1:�ͷ�
			{
				pThisObj->m_Status = Thread::enSuspend;
				::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//�ж�2:����
				pThisObj->m_Status = Thread::enRunning;
				pThisObj->p_Task(pThisObj->p_Para);
			}
			else
			{
				pThisObj->m_Status = Thread::enAvialable;
				::ResetEvent(pThisObj->m_hEvt);
				::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//�ж�2:����
			}
			if (pThisObj->m_times>0)//�ж�3�����˳�
			{
				pThisObj->m_times--;
			}
		}
		pThisObj->p_Task = 0;
		pThisObj->m_Status = Thread::enDead;
		OutputDebugString(L"<\\TaskThread::ThreadMain()>\n");
	}
public:
	//�����߳�
	void  Create(int times = -1, long waiteTime = 0)//2.0
	{
		if (m_hEvt == 0)
		{
			m_hEvt = ::CreateEvent(0, true, false, 0);
		}
		if (m_hThread == 0)
		{
			m_times = times;
			m_waiteTime = MAX(waiteTime, 0);
			m_hThread = (HANDLE)_beginthread(ThreadMain, 0, this);
			OutputDebugString(L"<TaskThread::Create()>\n");
		}
	}
//������
public:
	ThreadTaskFun p_Task;
	void*  p_Para;
	//ע��������//ֻ��ѭ���м���Ч
	void  FreeTask(void)
	{
		::EnterCriticalSection(&m_section);
		p_Task = 0;
		::LeaveCriticalSection(&m_section);
		OutputDebugString(L"<TaskThread::FreeTask()>\n");
	}
	//ע��������
	void  RegTask(ThreadTaskFun pFunc, void* pPara)
	{
		::EnterCriticalSection(&m_section);
		p_Task = pFunc;
		p_Para = pPara;//�ڴ��ɸ��̹߳���
		::LeaveCriticalSection(&m_section);
		OutputDebugString(L"<TaskThread::RegTask()>\n");
	}
};
//==============================================================================
///</class_info>


///<datastruct_info>
//==============================================================================
//�߳�����
#ifndef THREADTASKSTRUCT
#define THREADTASKSTRUCT
typedef struct tagThreadTask
{
	//enum {PARASIZE = 256};
	char  charFun[32];//���������뵱ǰ������ͬʱ,ʹ�õ�ǰ����
					  //char pPara[PARASIZE];
	void* pPara;
	ThreadTaskFun pFun;//������ָ��Ϊ0ʱʹ�õ�ǰ����
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
//��������:���������߳���,�ɶ�̬�������������
// 1:����̳߳ع���,�ڶ���˽�д洢�ռ��������������
// 2:������������������ȡ����
// 3:�������������ڲ������̷߳���������ָ���̵߳��������,
//    �����������̺��߳����еĸ����̷߳���������
// 4:��������е�һ�������ִ�к���ָ��,�ͺ�������ָ��,
//    ����Ϊ���⺯�������ռ���������������,���������ڹ̶����ȵ�������
//��������������������
//���߳�            �� <-�������������,��������
//�ĩ�����������������
//���߳�������      �� <-�̴߳���Create��ʼִ��,��������Task����Ϣ,�������˳����߳�����
//�ĩ������Щ���������
//��  �I  �� �I     �� <-ѭ������,�̶�����,ѭ���洢
//�������񩦿�����  �� 
//������0 ������0   �� <-����ͷ�߳�ִ�к�����,��DelHeadTaskɾ��
//������1 ������1   �� <-���еȴ��е�����
//��  ��  ��  ��    ��
//������n ������n   �� <-����β������AddTailTask���.
//��  ��  ��  ��    ��
//�������񩦿ղ���  ��
//��  �I  �� �I     �� <-ѭ������,�̶�����,ѭ���洢
//�ĩ������ة���������       
//���жϴ���        �� <-ȷ���������Ƿ�����˳�,ForceEnd�ж�0ǿ���˳�,m_times�ж�3�����˳�
//��������������������       Suspend�ж�2����ȴ�,������ѭ����ͣ.
//                           ��TaskList����Ϊ��,�ж�1ж�غ���,������ѭ���ȴ��º���
class TaskThreadEx :public TaskThread
{
//�๦��
public:
	const int m_classID = 2;//3.0 �������ͼ�
	//����ʱ�����߳�
	TaskThreadEx(HANDLE  hParent = 0)
	{
		p_TaskListHead = 0;
		p_TaskListTail = p_TaskListHead;
		m_TaskNum = 0;
		//p_Task = 0;
	}
//�̹߳���
private:
	//�߳������� :�̴߳�����ʼִ��,��������Task����Ϣ,�������˳����߳�����
	static void ThreadMain(void* thisObj)
	{
		OutputDebugString(L"<TaskThreadEx::ThreadMain()>\n");
		TaskThreadEx* pThisObj = (TaskThreadEx*)thisObj;
		while (!pThisObj->m_bExit && pThisObj->m_times != 0)//�ж�0:�˳�
		{
			//�����ȡ����
			//�ж���������Ƿ�Ϊ��
			if (pThisObj->p_TaskListHead == pThisObj->p_TaskListTail - 1 || pThisObj->m_TaskNum <= 0)
			{
				pThisObj->p_Task = 0;//����Ϊ��,��Ϊ�ж�1
				pThisObj->p_Para = 0;
			}
			else
			{
				pThisObj->RegTask(pThisObj->m_TastList[pThisObj->p_TaskListHead].pFun,
					pThisObj->m_TastList[pThisObj->p_TaskListHead].pPara);
			}
			//��������
			pThisObj->m_Status = Thread::enSuspend;
			::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//�ж�2:����
			if (pThisObj->p_Task)//�ж�1:�ͷ�
			{
				pThisObj->m_Status = Thread::enRunning;
				OutputDebugString(L"ThreadMain RunTask\n");
				pThisObj->p_Task(pThisObj->p_Para);
				//������ɺ���
				pThisObj->p_Task = 0;
				pThisObj->p_Para = 0;
				pThisObj->DelHeadTask();//���豣����ɾ����ͷ����ĺ���
			}
			else
			{
				pThisObj->m_Status = Thread::enAvialable;
				//Ϊ���������ȡ����,��ǰ����Ϊ��ʱ,ɨ���������ͷ,��������
				//::ResetEvent(pThisObj->m_hEvt);
				//::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//�ж�2:����
			}
			if (pThisObj->m_times>0)//�ж�3�����˳�
			{
				pThisObj->m_times--;
			}
		}
		pThisObj->p_Task = 0;
		pThisObj->m_Status = Thread::enDead;
		OutputDebugString(L"<\\TaskThreadEx::ThreadMain()>\n");
	}
public:
	//�����߳�
	void  Create(int times = -1, long waiteTime = 0)//2.0
	{
		if (m_hEvt == 0)
		{
			m_hEvt = ::CreateEvent(0, true, false, 0);
		}
		if (m_hThread == 0)
		{
			m_times = times;
			m_waiteTime = MAX(waiteTime, 0);
			m_hThread = (HANDLE)_beginthread(ThreadMain, 0, this);
			OutputDebugString(L"<TaskThreadEx::Create()>\n");
		}
	}
	//ǿ��ɾ���߳�,����ǰ����ж��,�����߳�,�������µ��߳�ָ��
	void  Recover(void)
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
//������
public:
	enum { TASKNUM = 256 };
	ThreadTask m_TastList[256];
	//ʹ���������
	int p_TaskListHead;
	int p_TaskListTail;
	int m_TaskNum;
	bool AddTailTask(ThreadTask _Task)
	{
		OutputDebugString(L"<AddTailTask()>\n");
		_Task.status = ThreadTask::enFree;
		//���������
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
	bool DelHeadTask()
	{
		OutputDebugString(L"<DelHeadTask()>\n");
		//���������
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
			OutputDebugString(L"TaskListHead is Null\n");//��ͷ�����Ѿ�ִ���겢�ͷ�
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
};
//==============================================================================
///</class_info>