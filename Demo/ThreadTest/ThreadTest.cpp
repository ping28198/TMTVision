// ThreadTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Windows.h"
#include <process.h>
//#include "Thread.h"
//#include "ThreadA.h"
//#include "ThreadB.h"
#include "IThread.h"


//�����߼�����
unsigned char a = 0;
void __stdcall funA(void* para)
{
	//printf("funA:(int)(*para)=%d\n",(*(int*)para));
	printf("funA:(int)(*para)=%d\n", (int)a);
	a = (a += (*(int*)para)) % 128;
	Sleep(100);
}
void __stdcall funB(void* para)
{
	printf("funB:(float)(*para)=%f\n", (*(float*)para));
	Sleep(1000);
}
void __stdcall funC(void* para)
{
	int paraA = 2;
	TaskThreadExAddTask(para, (ThreadTaskFun)funA, (void*)&paraA); //����
	//dstThread->Resume();                  // ����funC���Ͻ�����funA  |
	Sleep(1);                               // ���͸�ThreadExAִ��     | 
}                                           // ����:                   |
											// funAִ��С��funC        |
int main(int argc, _TCHAR* argv[])          // һ��ʱ���ThreadExA     | 
{                                           // ������л����          |
	//ThreadExAΪ���������߳���             //                         |
	void* pThreadExA=TaskThreadExCreate();  //                         |
	//ThreadCΪ�������߳���                 //                         |
	void* pThreadC=TaskThreadCreate();      //                         |
										    // ����ִ�к���funC		   |
	//ThreadC������Ϊ:��������������������������������������������������
	void* paraC=pThreadExA;
	int para=0;
	TaskThreadRegTask(pThreadC,(ThreadTaskFun)funC,(void*)paraC/*(void*)&para*/);
	//

	//��ʼִ��
	TaskThreadResume(pThreadExA);
	TaskThreadResume(pThreadC);

	Sleep(100000);
	return 0;
}


////�������ڲ���
//int main()
//{
//	ThreadA threadA;
//	ThreadB threadB;
//	threadA.Create(-1, 200);
//	threadA.Resume();
//	threadB.Create(30, 400);
//	threadB.Resume();
//	threadA.Suspend();
//	Sleep(10000);
//	threadA.Resume();
//	//threadB.Suspend();
//	Sleep(10000);
//	threadA.ForceEnd();
//	threadA.Resume(); //ForceEnd
//	Sleep(10000);
//	threadB.Destroy();
//	threadA.Destroy();
//    return 0;
//}

////�麯������
//unsigned char a = 0;
//class Base1
//{
//public:
//	//char a;
//	virtual void fun1()
//	{
//		printf("Base1::fun1()\n");
//	}
//	void fun2()
//	{
//		//a=3;
//		printf("Base1::fun2()\n");
//	}
//	void fun3()
//	{
//		//a=3;
//		printf("Base1::fun3()\n");
//	}
//};
//class Base2
//{
//public:
//	//char a;
//	virtual void fun4()
//	{
//		//a=3;
//		printf("Base2::fun4()\n");
//	}
//};
//class Child1 :public Base1
//{
//public:
//	//char a;
//	void fun1()
//	{
//		printf("Child1::fun1()\n");
//	}
//	virtual void fun2()
//	{
//		//a=3;
//		printf("Child1::fun2()\n");
//	}
//	void fun3()
//	{
//		//a=3;
//		printf("Child1::fun3()\n");
//	}
//};
//
//class Child2 :public Child1
//{
//public:
//	char a;
//	//void fun1();
//	void fun2()
//	{
//		//a=3;
//		printf("Child2::fun2()\n");
//	}
//	void fun3()
//	{
//		//a=3;
//		printf("Child2::fun3()\n");
//	}
//
//};
//class Child3 :public Base1, public Base2
//{
//public:
//	virtual void fun3()
//	{
//		//a=3;
//		printf("Child3::fun3()\n");
//	}
//};
//int _tmain(int argc, _TCHAR* argv[])
//{
//	Child3 vtableTest2;
//
//	printf("sizeof(vtableTest)=%d\n", (int)sizeof(vtableTest2));
//
//	typedef void(*Fun)(void);
//	Fun pFun = NULL;
//	printf("�����ַ=%0x\n", (&vtableTest2));
//	printf("�����ַ=%0x\n", (vtableTest2));
//
//	printf("�麯�����ַ1=%0x\n", *(&vtableTest2));//����ȡָ��
//	printf("�麯����1 �� ��һ��������ַ=%0x\n", (int*)*(int*)(*(int*)(&vtableTest2)));
//	pFun = (Fun)*((int*)*(int*)(&vtableTest2));
//	pFun();
//	pFun = NULL;
//	printf("�麯����1 �� ��2��������ַ=%0x\n", (int*)*((int*)(*(int*)(&vtableTest2))) + 1);
//	pFun = (Fun)*((int*)*(int*)(&vtableTest2) + 1);
//	pFun();
//	printf("�麯�����ַ2=%0x\n", (int*)*((int*)(&vtableTest2) + 1));
//	printf("�麯����2 �� ��һ��������ַ=%0x\n", (int*)*((int*)*((int*)(&vtableTest2) + 1)));
//	pFun = (Fun)*((int*)(*((int*)(&vtableTest2) + 1)));
//	pFun();
//	while (1);
//	return 0;
//}