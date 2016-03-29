///<proj_info>
//==============================================================================
// ��Ŀ�� �����ܼ��
// �ļ��� ��Thread.h
// ��  �� ������
// ��  ; ���߳���,�����
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
//								 ����
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

//������
#ifndef TASKTHREADFUN
#define TASKTHREADFUN
typedef void (__stdcall* ThreadTaskFun)(void* pPara);
#endif

///<data_info>
//==============================================================================
//��������:�̳߳���
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
//==============================================================================
///</data_info>
class Thread
{
private:
	//�߳������� :�̴߳�����ʼִ��,��������Task����Ϣ,�������˳����߳�����
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
	//����ʱ������Դ
	Thread(HANDLE  hParent=0);//2.0
	//����ʱ���ò����ȴ��߳�����������,��Ҫ�ȴ�1��
	~Thread(void);
	//�����߳�
	void  Create(int times=-1,long waiteTime=0);//2.0
	//����ִ�й�����߳�
	void  Resume(void);
	//�����߳�
	void  Suspend(void);
	//���ò���ʹ�������˳��������߳�
	void  Destroy(void);
	//ע��������//ֻ��ѭ���м���Ч
	void  FreeTask(void);
	//ע��������
	void  RegTask(ThreadTaskFun pFunc, void* pPara);
	//ǿ�ƽ�����ǰ�߳�
	void  ForceEnd(void);
	//ǿ�ƽ�����ǰ�߳�,���������߳�
};

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

///<data_info>
//==============================================================================
//��������:����˽��������е��̳߳���
// 1:����̳߳ع���,�ڶ���˽�д洢�ռ��������������
// 2:������������������ȡ����
// 3:�������������ڲ������̷߳���������ָ���̵߳��������,
//    �����������̺��߳����еĸ����̷߳���������
// 4:��������е�һ�������ִ�к���ָ��,�ͺ�������ָ��,
//    ����Ϊ���⺯�������ռ���������������,���������ڹ̶����ȵ�������
//==============================================================================
///</data_info>
class ThreadEx :public Thread
{
public:
	enum {TASKNUM = 256};
	ThreadTask m_TastList[256];
	//ʹ���������
	int p_TaskListHead;
	int p_TaskListTail;
	int m_TaskNum;
	//CTaskCriticalSection taskCritalSection;
	CRITICAL_SECTION m_section;
	bool AddTailTask(ThreadTask _Task)
	{
		OutputDebugString(L"<AddTailTask()>\n");
		_Task.status=ThreadTask::enFree;
		//���������
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
		//���������
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
			OutputDebugString(L"TaskListHead is Null\n");//��ͷ�����Ѿ�ִ���겢�ͷ�
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
	//�߳������� :�̴߳�����ʼִ��,��������Task����Ϣ,�������˳����߳�����
	static void ThreadMain(void* thisObj);
public:
	//����ʱ�����߳�
	ThreadEx();
	//����ʱ���ò����ȴ��߳�����������,��Ҫ�ȴ�1��
	~ThreadEx();
	//�����߳�
	void Create(int times=-1);
	//ǿ��ɾ���߳�,����ǰ����ж��,�����߳�,�������µ��߳�ָ��
	void  Recover(void);
};