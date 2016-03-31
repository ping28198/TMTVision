#pragma once
#ifndef __ITHREAD_H__
#define __ITHREAD_H__
#endif

#ifdef ITHREAD_EXPORTS
#define ITHREAD_API __declspec(dllexport)
#else
#define ITHREAD_API __declspec(dllimport)
#endif

//������
#ifndef TASKTHREADFUN
#define TASKTHREADFUN
typedef void (__stdcall* ThreadTaskFun)(void* pPara);
#endif

//������߳���
ITHREAD_API void GetThreadInfo(void* pThread, void* &hThread, unsigned long &IDThread, int& IDClass);



//�������̴߳���
ITHREAD_API void* TaskThreadCreate(HANDLE  hParent = 0, int times=-1,long waiteTime = 0);
//����ִ�й�����߳�
ITHREAD_API void  TaskThreadResume(void*pThread);
//�����߳�
ITHREAD_API void  TaskThreadSuspend(void*pThread);
//���ò���ʹ�������˳��������߳�
ITHREAD_API void  TaskThreadDestroy(void*pThread);
//ע��������
ITHREAD_API void  TaskThreadFreeTask(void*pThread);
//ע��������
ITHREAD_API void  TaskThreadRegTask(void* pThread,ThreadTaskFun pFunc, void* pPara);
//�߳���ǿ�ƽ�����ǰ�߳�
ITHREAD_API void  TaskThreadForceEnd(void* pThread);

//��������̴߳���
ITHREAD_API void* TaskThreadExCreate(HANDLE  hParent = 0, int times = -1, long waiteTime = 0);
//����ִ�й�����߳�
ITHREAD_API void  TaskThreadExResume(void*pThread);
//�����߳�
ITHREAD_API void  TaskThreadExSuspend(void*pThread);
//���ò���ʹ�������˳��������߳�
ITHREAD_API void  TaskThreadExDestroy(void*pThread);
//ǿ�ƽ�����ǰ�߳�
ITHREAD_API void  TaskThreadExForceEnd(void* pThread);
//ǿ��ɾ���߳�,����ǰ����ж��,�����߳�
ITHREAD_API void TaskThreadExRecover(void* pThread);

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
	enum TASKSTATUS{enNull= 0, enFree, enBusy};
	TASKSTATUS status;
	int life;
	//void* nextTask;
}ThreadTask, *PThreadTask;
#endif
//���������
ITHREAD_API void  TaskThreadExAddTask(void* pThread,ThreadTaskFun pFun,void* pPara);