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
//�߳������� :�̴߳�����ʼִ��,��������Task����Ϣ,�������˳����߳�����
void Thread::ThreadMain(void* thisObj)
{
	OutputDebugString(L"Thread::ThreadMain Enter\n");
	Thread* pThisObj = (Thread*)thisObj;
	while(!pThisObj->m_bExit && pThisObj->m_times!=0)//�ж�0:�˳�
	{
		//��������
		if(pThisObj->p_Task)//�ж�1:�ͷ�
		{
			pThisObj->m_Status = Thread::enSuspend;
			::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//�ж�2:����
			pThisObj->m_Status = Thread::enRunning;
			pThisObj->p_Task(pThisObj->p_Para);
		}else
		{
			pThisObj->m_Status = Thread::enAvialable;			
			::ResetEvent(pThisObj->m_hEvt);
			::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//�ж�2:����
		}
		if (pThisObj->m_times>0)//�ж�3�����˳�
		{
			pThisObj->m_times--;
		}
		Sleep(pThisObj->m_waiteTime);//2.0
	}
	pThisObj->p_Task=0;
	pThisObj->m_Status = Thread::enDead;
}
//---------------------------------------------------------
//�̼߳���ID :�̶߳���������+1,�������ı�
DWORD Thread::m_nThreadID = 0;//2.0
//---------------------------------------------------------
//����ʱ������Դ
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
//����ʱ���ò����ȴ��߳�����������,��Ҫ�ȴ�1��
Thread::~Thread(void)
{
	Destroy();
	Sleep(1000);//�ȴ��߳��������˳��������߳�
	if (m_hEvt!=0)
	{
		::CloseHandle(m_hEvt);
	}
}
//�����߳�
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
//����ִ�й�����߳�
void Thread::Resume(void)//�ж�1,2ȡ��:�ͷ�
{
	if (m_hEvt!=0)
	{
		::SetEvent(m_hEvt);
	}
}
//---------------------------------------------------------
//�����߳�
void  Thread::Suspend(void)//�ж�2:����
{
	if (m_hEvt!=0)
	{
		::ResetEvent(m_hEvt);
	}
}

//---------------------------------------------------------
//���ò���ʹ�������˳��������߳�
void  Thread::Destroy()//�ж�0:�˳�
{
	m_bExit = true;	
	p_Task = 0;
}
//---------------------------------------------------------
//ע��������
void  Thread::FreeTask()//�ж�1:�ͷ�
{
	p_Task = 0;
}
//---------------------------------------------------------
//ע��������
void  Thread::RegTask(ThreadTaskFun pFunc, void* pPara)//�ж�1ȡ������:ע��������
{
	p_Task = pFunc;
	p_Para = pPara;
}
//ǿ�ƽ�����ǰ�߳�
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
//�߳������� :�̴߳�����ʼִ��,��������Task����Ϣ,�������˳����߳�����
void ThreadEx::ThreadMain(void* thisObj)
{
	OutputDebugString(L"<ThreadEx::ThreadMain>\n");
	ThreadEx* pThisObj = (ThreadEx*)thisObj;
	while(!pThisObj->m_bExit && pThisObj->m_times!=0)//�ж�0:�˳�
	{
		//�����ȡ����
		//�ж���������Ƿ�Ϊ��
		if (pThisObj->p_TaskListHead==pThisObj->p_TaskListTail-1 || pThisObj->m_TaskNum<=0)
		{
			pThisObj->p_Task=0;//����Ϊ��,��Ϊ�ж�1
			pThisObj->p_Para=0;
		}
		else
		{
			//pThisObj->p_Task=pThisObj->m_TastList[pThisObj->p_TaskListHead].pFun;
			//pThisObj->p_Para=pThisObj->m_TastList[pThisObj->p_TaskListHead].pPara;
			pThisObj->RegTask(pThisObj->m_TastList[pThisObj->p_TaskListHead].pFun,
				pThisObj->m_TastList[pThisObj->p_TaskListHead].pPara);
		}
		//��������
		pThisObj->m_Status = Thread::enSuspend;
		::WaitForSingleObject(pThisObj->m_hEvt, INFINITE);//�ж�2:����
		if(pThisObj->p_Task)//�ж�1:�ͷ�
		{
			pThisObj->m_Status = Thread::enRunning;
			OutputDebugString(L"\nThreadMain RunTask\n\n");
			pThisObj->p_Task(pThisObj->p_Para);
			//������ɺ���
			pThisObj->p_Task=0;
			pThisObj->p_Para=0;
			pThisObj->DelHeadTask();//���豣����ɾ����ͷ����ĺ���
		}else
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
	pThisObj->p_Task=0;
	pThisObj->m_Status = Thread::enDead;
	OutputDebugString(L"<~ThreadEx::ThreadMain>\n");
}
//����ʱ�����߳�
ThreadEx::ThreadEx()
{
	p_TaskListHead=0;
	p_TaskListTail=p_TaskListHead;
	m_TaskNum=0;
	p_Task = 0;
	isEx=true;
	::InitializeCriticalSection(&m_section);
}
//����ʱ���ò����ȴ��߳�����������,��Ҫ�ȴ�1��
ThreadEx::~ThreadEx()
{
	Destroy();
	Sleep(1000);//�ȴ��߳��������˳��������߳�
	if (m_hEvt!=0)
	{
		::CloseHandle(m_hEvt);
	}
	::DeleteCriticalSection(&m_section);
}
//�����߳�
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