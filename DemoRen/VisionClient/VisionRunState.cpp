
#pragma once
#include "stdafx.h"
#include "VisionRunState.h"
#include "VisionClientDlg.h"

#define CycleTick 50 //周期，乘以100 ms



Logger VisionPublicSet::mLogger;

void VisionPublicSet::InitialSetting()
{
	
}

//void __stdcall VisionThreadTask(void* pParam)
//{
//	CVisionRunState *pParent = (CVisionRunState*)pParam;
//	OutputDebugString(L"WatchingNetworkTask: Running.\n");
//	pParent->Timmer();
//}

CVisionRunState::CVisionRunState(void* pParam)
{
	pParent = pParam;
	ShowImgIndex = 0;
	mtick = 0;
	ShowImgNum = 6;
	InitializeCriticalSection(&cs);
	//创建时启动进程
	Create();
	Suspend();
}

CVisionRunState::~CVisionRunState()
{
	DeleteCriticalSection(&cs);
	Suspend();
	Destroy();
}

bool CVisionRunState::AddNewImg(Tmtv_ImageInfo* imgInfo)
{
	vector<Tmtv_ImageInfo>::iterator it;
	bool isExist = false;
	EnterCriticalSection(&cs);
	it = mImgInfoVec.begin();
	for (; it != mImgInfoVec.end();it++)
	{
		if (it->mCameraInfo.Indexnum==imgInfo->mCameraInfo.Indexnum)
		{
			*it = *imgInfo;
			isExist = true;
		}
	}
	if(!isExist)	mImgInfoVec.push_back(*imgInfo);
	LeaveCriticalSection(&cs);
	CString mstr;
	mstr.Format(_T("HistoryImgInfo\\%d.data"),imgInfo->mCameraInfo.Indexnum);
	CMemoryFile::WriteMemoryToFile_AW(imgInfo, sizeof(Tmtv_ImageInfo), mstr.GetBuffer());
	return true;
}

bool CVisionRunState::DelImgInfo(int CameraIndex)
{
	vector<Tmtv_ImageInfo>::iterator it;
	EnterCriticalSection(&cs);
	it = mImgInfoVec.begin();
	for (; it != mImgInfoVec.end(); it++)
	{
		if (it->mCameraInfo.Indexnum == CameraIndex)
		{
			mImgInfoVec.erase(it);
		}
	}
	LeaveCriticalSection(&cs);
	return true;
}

void CVisionRunState::Timmer()
{
	int mRunState = RunStateEnum::ShowIdle;
	Sleep(100);
	EnterCriticalSection(&cs);
	mtick++;
	if (mtick>= CycleTick)
	{
		VisionPublicSet::mLogger.TraceInfo("VisionRunState线程运行一次");
		mRunState = ShowState;
		mtick = 0;
	}
	LeaveCriticalSection(&cs);
	if (mRunState == RunStateEnum::ShowNormal)
	{
		this->SelectImgToShow();
		this->ShowImg();
	}
	if (mRunState == RunStateEnum::ShowHistImg)
	{
		this->SelectCameraToShow();
		this->ShowImg();
	}
	
}

void CVisionRunState::SetShowImgNum(int num)
{
	if (num < 0) return;
	if (num > 64) return;
	EnterCriticalSection(&cs);
	ShowImgNum = num;
	LeaveCriticalSection(&cs);
}
void CVisionRunState::UpdateImg()
{
	EnterCriticalSection(&cs);
	mtick = CycleTick;
	LeaveCriticalSection(&cs);
}


void CVisionRunState::ShowHistoryImg(int CameraIndex, char* pTime)
{
	EnterCriticalSection(&cs);
	ShowState = RunStateEnum::ShowHistImg;
	HistImgCameraIndex = CameraIndex;
	strcpy_s(HistImgTime,sizeof(HistImgTime),pTime);
	mtick = CycleTick;
	HistImgImgIndex = 0;
	LeaveCriticalSection(&cs);
}

void CVisionRunState::DelCameraImg(int CameraIndex)
{
	vector<Tmtv_ImageInfo>::iterator it;
	EnterCriticalSection(&cs);
	it = mImgInfoVec.begin();
	for (; it != mImgInfoVec.end(); it++)
	{
		if (it->mCameraInfo.Indexnum==CameraIndex)
		{
			mImgInfoVec.erase(it);
		}
	}
	LeaveCriticalSection(&cs);
}

void CVisionRunState::ResetImgWarn(int CameraIndex)
{
	vector<Tmtv_ImageInfo>::iterator it;
	EnterCriticalSection(&cs);
	it = mImgInfoVec.begin();
	for (; it != mImgInfoVec.end(); it++)
	{
		if (it->mCameraInfo.Indexnum == CameraIndex)
		{
			it->IsWarnning = 0;
		}
	}
	mtick = CycleTick;
	LeaveCriticalSection(&cs);
}

void CVisionRunState::ShowRealTimeImg()
{
	EnterCriticalSection(&cs);
	ShowState = RunStateEnum::ShowNormal;
	mtick = CycleTick;
	LeaveCriticalSection(&cs);
}

void CVisionRunState::Task(void)
{
	Timmer();
}

void CVisionRunState::SelectImgToShow()
{
	vector<Tmtv_ImageInfo>::iterator it;
	int count = 0;
	EnterCriticalSection(&cs);
	CVisionClientDlg* pDlg = (CVisionClientDlg*)pParent;
	pDlg->mImageShowVec.swap(vector<Tmtv_ImageInfo>());
	for ( it = mImgInfoVec.begin(); it != mImgInfoVec.end(); it++)//加入含有警示信息的图片
	{
		if (it->IsWarnning)
		{
			pDlg->mImageShowVec.push_back(*it);
			count++;
		}
		if (count == ShowImgNum)
		{
			ShowImgIndex = it->mCameraInfo.Indexnum;
			LeaveCriticalSection(&cs);
			return;
		}
	}
	for (it = mImgInfoVec.begin(); it != mImgInfoVec.end(); it++)//加入后面的图片
	{
		if (it->IsWarnning) continue;
		if (it->mCameraInfo.Indexnum > ShowImgIndex)
		{
			pDlg->mImageShowVec.push_back(*it);
			count++;
		}
		if (count == ShowImgNum)
		{
			ShowImgIndex = it->mCameraInfo.Indexnum;
			LeaveCriticalSection(&cs);
			return;
		}
	}
	for (it = mImgInfoVec.begin(); it != mImgInfoVec.end(); it++)//加入前面的图片
	{
		if (it->IsWarnning) continue;
		if (it->mCameraInfo.Indexnum <= ShowImgIndex)
		{
			pDlg->mImageShowVec.push_back(*it);
			count++;
		}
		if (count == ShowImgNum)
		{
			ShowImgIndex = it->mCameraInfo.Indexnum;
			LeaveCriticalSection(&cs);
			return;
		}
	}
	LeaveCriticalSection(&cs);
}

bool CVisionRunState::SelectCameraToShow()
{
	vector<Tmtv_ImageInfo>::iterator it;
	int count = 0;
	int mindex = 0;
	CVisionClientDlg* pDlg = (CVisionClientDlg*)pParent;
	pDlg->mImageShowVec.swap(vector<Tmtv_ImageInfo>());
	for (it = mHistImgInfoVec.begin(); it != mHistImgInfoVec.end(); it++)//加入后面的图片
	{
		mindex++;
		if (mindex > HistImgImgIndex)
		{
			pDlg->mImageShowVec.push_back(*it);
			count++;
			if (count== ShowImgNum)
			{
				HistImgImgIndex = mindex;
			}
		}
	}
	mindex = 0;
	for (it = mHistImgInfoVec.begin(); it != mHistImgInfoVec.end(); it++)//加入前面的图片
	{
		mindex++;
		if (mindex <= HistImgImgIndex)
		{
			pDlg->mImageShowVec.push_back(*it);
			count++;
			if (count == ShowImgNum)
			{
				HistImgImgIndex = mindex;
			}
		}
	}
	return true;
}

bool CVisionRunState::ReadCameraInfo(int CameraIndex)
{
	CString mstr;
	mstr.Format(_T("HistoryImgInfo\\%d.data"), CameraIndex);
	CMemoryFile mFile;
	bool backcode = mFile.OpenFile_R(mstr.GetBuffer());
	if (backcode == false)
	{
		mFile.CloseFile();
		return false;
	}
	mHistImgInfoVec.swap(vector<Tmtv_ImageInfo>());
	Tmtv_ImageInfo mImgInfo;
	char mtime[64] = { 0 };
	while (mFile.ReadMemoryFromFile(&mImgInfo,sizeof(mImgInfo)))
	{
		strcpy_s(mtime, 64, mImgInfo.GrabTime);
		mtime[10]='\0';
		if (strcmp(HistImgTime, mtime)==0)
		{
			mHistImgInfoVec.push_back(mImgInfo);
		}
	}
	return true;
}

void CVisionRunState::ShowImg()
{
	CVisionClientDlg* pDlg = (CVisionClientDlg*)pParent;
	pDlg->DrawImgToMemBmp();
	pDlg->ShowMemBmpToScreen();
}
