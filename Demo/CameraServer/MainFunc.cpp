// MainFunc.cpp : 定义控制台应用程序的入口点. 
//

#include "stdafx.h"
#include "MessageServer.h"
//#include "Thread.h"
#include "VisionStruct.h"
#include "Detector.h"
struct ClassA
{
public:
	ClassA()
	{
		A = 0;
	}
	~ClassA()
	{
		A = 1;
	}
private:
	int A;
};
struct ClassB: public ClassA
{
public:
	ClassB()
	{
		B = new int[32];
	}
	~ClassB()
	{
		if (B != 0)
		{
			delete[]B;
		}
	}
private:
	int * B;
};
int main(int argc, char *argv[])
{
	ClassB * pClassA = new ClassB();
	delete pClassA;



	/*Tmtv_AlgorithmInfo algorithmInfo;
	long a = sizeof(algorithmInfo);

	Tmtv_BackgroundDetectorInfo bdAlgorithmInfo;
	long ab = sizeof(bdAlgorithmInfo);

	Tmtv_AlgorithmInfo* pAlgorithmInfo = &bdAlgorithmInfo;
	long ap = sizeof(*pAlgorithmInfo);




	Tmtv_CameraInfo cameraInfo;
	long b = sizeof(cameraInfo);

	Tmtv_DefectInfo defectInfo;
	long c = sizeof(defectInfo);

	Tmtv_ImageInfo imageInfo;
	long d = sizeof(imageInfo);*/

	//MessageServer messageServer1;
	////messageServer.Initial();
	//messageServer1.Create();
	//messageServer1.Resume();
	//MessageServer messageServer2;

	//messageServer.SendMsg();
	//bool socketOK = socketServer.Initial(char* DstIp, int DstPort, int MyPort = 0, char* MyIp = NULL, int optionFlag = 1);
	return 0;
}