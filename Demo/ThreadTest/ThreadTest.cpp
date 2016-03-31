// ThreadTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Windows.h"
#include <process.h>
//#include "Thread.h"
//#include "ThreadA.h"
//#include "ThreadB.h"
#include "IThread.h"


//调用逻辑测试
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
	TaskThreadExAddTask(para, (ThreadTaskFun)funA, (void*)&paraA); //←┐
	//dstThread->Resume();                  // 函数funC不断将函数funA  |
	Sleep(1);                               // 推送给ThreadExA执行     | 
}                                           // 由于:                   |
											// funA执行小于funC        |
int main(int argc, _TCHAR* argv[])          // 一段时间后ThreadExA     | 
{                                           // 任务队列会存满          |
	//ThreadExA为队列任务线程类             //                         |
	void* pThreadExA=TaskThreadExCreate();  //                         |
	//ThreadC为单任务线程类                 //                         |
	void* pThreadC=TaskThreadCreate();      //                         |
										    // 不断执行函数funC		   |
	//ThreadC的任务为:────────────────────────┘
	void* paraC=pThreadExA;
	int para=0;
	TaskThreadRegTask(pThreadC,(ThreadTaskFun)funC,(void*)paraC/*(void*)&para*/);
	//

	//开始执行
	TaskThreadResume(pThreadExA);
	TaskThreadResume(pThreadC);

	Sleep(100000);
	return 0;
}


////生命周期测试
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

////虚函数测试
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
//	printf("对象地址=%0x\n", (&vtableTest2));
//	printf("对象地址=%0x\n", (vtableTest2));
//
//	printf("虚函数表地址1=%0x\n", *(&vtableTest2));//对象取指针
//	printf("虚函数表1 — 第一个函数地址=%0x\n", (int*)*(int*)(*(int*)(&vtableTest2)));
//	pFun = (Fun)*((int*)*(int*)(&vtableTest2));
//	pFun();
//	pFun = NULL;
//	printf("虚函数表1 — 第2个函数地址=%0x\n", (int*)*((int*)(*(int*)(&vtableTest2))) + 1);
//	pFun = (Fun)*((int*)*(int*)(&vtableTest2) + 1);
//	pFun();
//	printf("虚函数表地址2=%0x\n", (int*)*((int*)(&vtableTest2) + 1));
//	printf("虚函数表2 — 第一个函数地址=%0x\n", (int*)*((int*)*((int*)(&vtableTest2) + 1)));
//	pFun = (Fun)*((int*)(*((int*)(&vtableTest2) + 1)));
//	pFun();
//	while (1);
//	return 0;
//}