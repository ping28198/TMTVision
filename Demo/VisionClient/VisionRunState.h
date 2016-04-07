#pragma once
#include "CommonInclude.h"
class VisionPublicSet
{
public:
	static Logger mLogger;
	static void InitialSetting();

};





class CVisionRunState:public Thread
{
public:
	CRITICAL_SECTION cs;//本地线程锁
	CVisionRunState(void* pParam);
	~CVisionRunState();
	enum RunStateEnum
	{
		ShowIdle=0,
		ShowNormal,
		ShowHistImg,
	};

	bool AddNewImg(Tmtv_ImageInfo* imgInfo);
	bool DelImgInfo(int CameraIndex);
	void SetShowImgNum(int num);
	void UpdateImg();
	void ShowHistoryImg(int CameraIndex,char* pTime);//例：2016-05-03
	void DelCameraImg(int CameraIndex);
	void ResetImgWarn(int CameraIndex);
	void ShowRealTimeImg();



	void Timmer();

private:
	void Task(void);
	void SelectImgToShow();
	bool SelectCameraToShow();
	bool ReadCameraInfo(int CameraIndex);
	void ShowImg();
private:
	void* pParent;
	vector<Tmtv_ImageInfo> mImgInfoVec;
	vector<Tmtv_ImageInfo> mHistImgInfoVec;
	int ShowImgNum;
	int ShowImgIndex;
	int ShowState;//=0正常滚动显示，=1显示历史图像
	int HistImgCameraIndex;
	int HistImgImgIndex;
	char HistImgTime[64];
	unsigned long mtick;
};

