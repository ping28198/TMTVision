//ͨ�ú���ģ��
#pragma once
#ifdef IFUNCTION_EXPORTS
#define IFUNCTION_API __declspec(dllexport)
#else
#define IFUNCTION_API __declspec(dllimport)
#endif

#ifndef __IFUNCTION_H__

//�궨��
#define LONGSTRLEN               256
#define SHORTSTRLEN              32

typedef long FRESULT;
#define RST_OK                   ((FRESULT)0L)
#define RST_FALSE                ((FRESULT)1L)
#define ERR_POINT_INVALID        ((FRESULT)2L) 
#define ERR_MEMORY_INVALID       ((FRESULT)3L)
#define ERR_PARA_INVALID         ((FRESULT)4L)

//ͨ�ò����ṹ
typedef struct tagFunctionPara
{
	//enum { LONGSTRLEN = 256};
	char  Info[LONGSTRLEN];//��������
	int sizePara;//������С
	void*  pPara;//����ָ��
}FunctionPara, *PFunctionPara;
//���������ṹ
typedef struct tagFunctionInfo
{
	enum { PARAMAXNUM = 16 };// , LONGSTRLEN = 256, SHORTSTRLEN = 256};
	char Name[SHORTSTRLEN];//��������
	char Description[LONGSTRLEN];//��������
	FunctionPara*  Paras[PARAMAXNUM];//ͨ������
	int numParas;
}FunctionInfo, *PFunctionInfo;
//�ַ�������
typedef struct tagStrList
{
	enum { LISTMAXNUM = 16 };
	char strList[LISTMAXNUM][SHORTSTRLEN];//��������
	int numList;
}StrList, *PStrList;

//ͨ�ú���ģ��
IFUNCTION_API int GetFunctionList(FunctionInfo* functionInfo);
IFUNCTION_API int GetFunctionInfo(FunctionInfo* functionInfo);

#define __IFUNCTION_H__
#endif

















