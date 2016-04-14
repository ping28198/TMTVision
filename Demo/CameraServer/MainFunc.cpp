// MainFunc.cpp : 定义控制台应用程序的入口点. 
//
#include "CommonInclude.h"
#include "MessageServer.h"
#include "stdafx.h"
int main(int argc, char *argv[])
{
	if (argc < 7) return 0;
	int remoteRecvPort = atoi(argv[1]);
	NetIP remoteRecvIp = { 0 };
	strcpy_s(remoteRecvIp, TMTV_IPSTRLEN, argv[2]);
	int localRecvPort = atoi(argv[3]);
	NetIP localRecvIP = { 0 };
	strcpy_s(localRecvIP, TMTV_IPSTRLEN, argv[4]);
	int localSendPort = atoi(argv[5]);
	NetIP localSendIP = { 0 };
	strcpy_s(localSendIP, TMTV_IPSTRLEN, argv[6]);
	TmtSocket::GetAvailableNetIP(localRecvIP);
	TmtSocket::GetAvailableNetIP(remoteRecvIp);

	MessageServer messageServer;
	messageServer.Initial(remoteRecvPort, remoteRecvIp, localRecvPort, localRecvIP, localSendPort, NULL,TmtSocket::ADDR_REUSE);
	messageServer.Create();
	messageServer.Resume();


	//Tmtv_AskInfo msgInfo;
	//msgInfo.Asktype = Tmtv_AskInfo::TMTV_ADDCAM;
	//msgInfo.CameraInfo.Status = Tmtv_CameraInfo::TMTV_RUNNINGCAM;
	//msgInfo.CameraInfo.WaiteTime = 2000;
	//msgInfo.CameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
	//strcpy_s(msgInfo.CameraInfo.CameraName, TMTV_SHORTSTRLEN, "TestCamera");
	//strcpy_s(msgInfo.CameraInfo.CameraPath, TMTV_PATHSTRLEN, "D:\\TST\\");
	//strcpy_s(msgInfo.CameraInfo.AlgorithmInfo.DstImgPath, TMTV_PATHSTRLEN, "D:\\TST2\\");
	////messageServer.SendMsgInfo(askInfo);
	//TmtSocket tmtSocket;
	//tmtSocket.SetSendAddr(localRecvPort, localRecvIP, 5006, NULL);//5610 127.0.0.1 5620 222.28.39.16 5621 222.28.39.16
	//Sleep(1000);
	//tmtSocket.SendMsg((void *)& msgInfo, msgInfo.structSize);
	//msgInfo.Asktype = Tmtv_AskInfo::TMTV_STARTCAM;
	//tmtSocket.SendMsg((void *)& msgInfo, msgInfo.structSize);

	//msgInfo.Asktype = Tmtv_AskInfo::TMTV_ADDCAM;
	//msgInfo.CameraInfo.Status = Tmtv_CameraInfo::TMTV_RUNNINGCAM;
	//msgInfo.CameraInfo.WaiteTime = 2000;
	//msgInfo.CameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
	//strcpy_s(msgInfo.CameraInfo.CameraName, TMTV_SHORTSTRLEN, "TestCamera2");
	//strcpy_s(msgInfo.CameraInfo.CameraPath, TMTV_PATHSTRLEN, "D:\\TST");
	//strcpy_s(msgInfo.CameraInfo.AlgorithmInfo.DstImgPath, TMTV_PATHSTRLEN, "D:\\TST2\\");
	////messageServer.SendMsgInfo(askInfo);
	//tmtSocket.SetSendAddr(5003, "127.0.0.1", 5006, NULL);
	//Sleep(1000);
	//tmtSocket.SendMsg((void *)& msgInfo, msgInfo.structSize);
	//msgInfo.Asktype = Tmtv_AskInfo::TMTV_STARTCAM;
	//tmtSocket.SendMsg((void *)& msgInfo, msgInfo.structSize);


	MEGAWSTR  wstring;
	MEGASTR  string;
	while (1)
	{
	/*	messageServer.SendMsgInfo(msgInfo);*/
		//tmtSocket.SendMsg((void *)& msgInfo, msgInfo.structSize);
		system("cls");
		wstring[0] = 0;	
		string[0] = 0;
		messageServer.ToString(wstring, 2, 33);
		CCommonFunc::UnicodeToAnsi(wstring, string, TMTV_MEGASTRLEN);
		printf(string);	
		//wstring[0] = 0;
		//string[0] = 0;
		//cameraServer.ToString(wstring, 2, 32);
		//CCommonFunc::UnicodeToAnsi(wstring, string, TMTV_MEGASTRLEN);
		//printf(string);
		Sleep(1000);
	}
	return 0;
}