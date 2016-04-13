//ͨ�ú���ģ��
#pragma once
#ifdef MOTIONDETECT_EXPORTS
#define MOTIONDETECT_API __declspec(dllexport)
#else
#define MOTIONDETECT_API __declspec(dllimport)
#endif

#ifndef __MOTIONDETECT_H__

//�궨��
#define LONGSTRLEN               256
#define SHORTSTRLEN              32

typedef long FRESULT;
#define RST_OK                   ((FRESULT)0L)
#define RST_FALSE                ((FRESULT)1L)
#define ERR_POINT_INVALID        ((FRESULT)2L) 
#define ERR_MEMORY_INVALID       ((FRESULT)3L)
#define ERR_PARA_INVALID         ((FRESULT)4L)

//������ݽṹ
typedef struct tagDetectRect
{
	long left;
	long right;
	long top;
	long bottom;
	int type;
}DetectRect, *PDetectRect;

//������ݽṹ
typedef struct tagDetectResult
{
	enum { RECTMAXNUM = 32 };// , LONGSTRLEN = 256, SHORTSTRLEN = 256};
	DetectRect Rects[RECTMAXNUM];//ͨ������
	int numRects;
}DetectResult, *PDetectResult;

//��ʼ��ͼ�����
MOTIONDETECT_API FRESULT Initial(int imageHeight,int imageWidth, int imageDepth, int imageListNum);
//Pushͼ�����,�������,�����ڴ���3ʱ���ؼ������
MOTIONDETECT_API FRESULT Push(unsigned char* imageData, long imageDataLen,
	                          DetectResult* detectResult);


#define __MOTIONDETECT_H__
#endif

















