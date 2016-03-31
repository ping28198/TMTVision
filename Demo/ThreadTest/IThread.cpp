#include "stdafx.h"
#include "Thread.h"
#include "IThread.h"
//������߳���
void GetThreadInfo(void* pThread,void* &hThread,unsigned long &IDThread, int& IDClass)
{
	TaskThread* p_Thread=(TaskThread*)pThread;
	hThread=p_Thread->m_hThread;
	IDThread=p_Thread->m_nThreadID;
	IDClass = p_Thread->m_classID;
}
//�򵥼���ѭ���̴߳���
void* TaskThreadCreate(HANDLE  hParent , int times , long waiteTime)
{
	TaskThread *p_Thread=new TaskThread(hParent);
	p_Thread->Create(times, waiteTime);
	return (void*)p_Thread;
}
//����ִ�й�����߳�
void  TaskThreadResume(void*pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->Resume();
}
//�����߳�
void  TaskThreadSuspend(void*pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->Suspend();
}
//���ò���ʹ�������˳��������߳�
void  TaskThreadDestroy(void*pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->Destroy();
	delete p_Thread;
}
//ע��������
void  TaskThreadFreeTask(void*pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->FreeTask();
}
//ע��������
void  TaskThreadRegTask(void* pThread,ThreadTaskFun pFunc, void* pPara)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->RegTask(pFunc,pPara);
}
//�߳���ǿ�ƽ�����ǰ�߳�
void  TaskThreadForceEnd(void* pThread)
{
	TaskThread *p_Thread=(TaskThread*)pThread;
	p_Thread->ForceEnd();
}

//����������̴߳���
void* TaskThreadExCreate(HANDLE  hParent , int times , long waiteTime)
{
	TaskThreadEx *p_Thread=new TaskThreadEx(hParent);
	p_Thread->Create(times, waiteTime);
	return (void*)p_Thread;
}
//����ִ�й�����߳�
void  TaskThreadExResume(void*pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->Resume();
}
//�����߳�
void  TaskThreadExSuspend(void*pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->Suspend();
}
//���ò���ʹ�������˳��������߳�
void  TaskThreadExDestroy(void*pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->Destroy();
}
//ǿ�ƽ�����ǰ�߳�
void  TaskThreadExForceEnd(void* pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->ForceEnd();
}
//ǿ��ɾ���߳�,����ǰ����ж��,�����߳�
void TaskThreadExRecover(void* pThread)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	p_Thread->Recover();
}
//���������
void  TaskThreadExAddTask(void* pThread,ThreadTaskFun pFun,void* pPara)
{
	TaskThreadEx *p_Thread=(TaskThreadEx*)pThread;
	ThreadTask task;
	task.pFun=pFun;
	task.pPara=pPara;
	p_Thread->AddTailTask(task);
	p_Thread->Resume();
}