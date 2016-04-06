// MainFunc.cpp : 定义控制台应用程序的入口点. 
//
#include "CommonInclude.h"
#include "MessageServer.h"
#include "CameraServer.h"
#include "stdafx.h"
int main(int argc, char *argv[])
{
	Tmtv_CameraInfo cameraInfo;
	strcpy_s(cameraInfo.CameraName, TMTV_LONGSTRLEN, "TestCamera");
	strcpy_s(cameraInfo.CameraPath, TMTV_LONGSTRLEN, "D:\\TST\\");
	cameraInfo.Status = Tmtv_CameraInfo::TMTV_RUNNINGCAM;
	strcpy_s(cameraInfo.AlgorithmInfo.DstImgPath, TMTV_LONGSTRLEN, "TestCamera");
	strcpy_s(cameraInfo.AlgorithmInfo.DstImgPath, TMTV_LONGSTRLEN, "TestCamera");
	cameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_STARTWARN;
	CameraServer cameraServer;
	cameraServer.AddCamera(cameraInfo);
	cameraServer.StartAlgorithm(cameraInfo.AlgorithmInfo);
	cameraServer.StartCamera();
	
	MessageServer messageServer;
	messageServer.Initial(103, "127.0.0.1", 105, "127.0.0.1", 106, "127.0.0.1");
	messageServer.Create();
	messageServer.Resume();



	MEGAWSTR  wstring;
	MEGASTR  string;
	while (1)
	{
		system("cls");
		wstring[0] = 0;	
		string[0] = 0;
		messageServer.ToString(wstring, 2, 33);
		CCommonFunc::UnicodeToAnsi(wstring, string, TMTV_MEGASTRLEN);
		printf(string);	
		wstring[0] = 0;
		string[0] = 0;
		cameraServer.ToString(wstring, 2, 32);
		CCommonFunc::UnicodeToAnsi(wstring, string, TMTV_MEGASTRLEN);
		printf(string);
		Sleep(1000);
	}
	return 0;
}