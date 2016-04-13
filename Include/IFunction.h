//通用函数模板
#pragma once
#ifdef IFUNCTION_EXPORTS
#define IFUNCTION_API __declspec(dllexport)
#else
#define IFUNCTION_API __declspec(dllimport)
#endif

#ifndef __IFUNCTION_H__

//宏定义
#define LONGSTRLEN               256
#define SHORTSTRLEN              32

typedef long FRESULT;
#define RST_OK                   ((FRESULT)0L)
#define RST_FALSE                ((FRESULT)1L)
#define ERR_POINT_INVALID        ((FRESULT)2L) 
#define ERR_MEMORY_INVALID       ((FRESULT)3L)
#define ERR_PARA_INVALID         ((FRESULT)4L)

//通用参数结构
typedef struct tagFunctionPara
{
	//enum { LONGSTRLEN = 256};
	char  Info[LONGSTRLEN];//参数描述
	int sizePara;//参数大小
	void*  pPara;//参数指针
}FunctionPara, *PFunctionPara;
//函数描述结构
typedef struct tagFunctionInfo
{
	enum { PARAMAXNUM = 16 };// , LONGSTRLEN = 256, SHORTSTRLEN = 256};
	char Name[SHORTSTRLEN];//参数描述
	char Description[LONGSTRLEN];//函数描述
	FunctionPara*  Paras[PARAMAXNUM];//通用数组
	int numParas;
}FunctionInfo, *PFunctionInfo;
//字符串数组
typedef struct tagStrList
{
	enum { LISTMAXNUM = 16 };
	char strList[LISTMAXNUM][SHORTSTRLEN];//参数描述
	int numList;
}StrList, *PStrList;

//通用函数模板
IFUNCTION_API int GetFunctionList(FunctionInfo* functionInfo);
IFUNCTION_API int GetFunctionInfo(FunctionInfo* functionInfo);

#define __IFUNCTION_H__
#endif

















