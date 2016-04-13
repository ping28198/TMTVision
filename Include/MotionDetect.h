//通用函数模板
#pragma once
#ifdef MOTIONDETECT_EXPORTS
#define MOTIONDETECT_API __declspec(dllexport)
#else
#define MOTIONDETECT_API __declspec(dllimport)
#endif

#ifndef __MOTIONDETECT_H__

//宏定义
#define LONGSTRLEN               256
#define SHORTSTRLEN              32

typedef long FRESULT;
#define RST_OK                   ((FRESULT)0L)
#define RST_FALSE                ((FRESULT)1L)
#define ERR_POINT_INVALID        ((FRESULT)2L) 
#define ERR_MEMORY_INVALID       ((FRESULT)3L)
#define ERR_PARA_INVALID         ((FRESULT)4L)

//结果数据结构
typedef struct tagDetectRect
{
	long left;
	long right;
	long top;
	long bottom;
	int type;
}DetectRect, *PDetectRect;

//结果数据结构
typedef struct tagDetectResult
{
	enum { RECTMAXNUM = 32 };// , LONGSTRLEN = 256, SHORTSTRLEN = 256};
	DetectRect Rects[RECTMAXNUM];//通用数组
	int numRects;
}DetectResult, *PDetectResult;

//初始化图像队列
MOTIONDETECT_API FRESULT Initial(int imageHeight,int imageWidth, int imageDepth, int imageListNum);
//Push图像队列,采用深拷贝,队列内大于3时返回检测区域
MOTIONDETECT_API FRESULT Push(unsigned char* imageData, long imageDataLen,
	                          DetectResult* detectResult);


#define __MOTIONDETECT_H__
#endif

















