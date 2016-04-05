///<proj_info>
//==============================================================================
// ��Ŀ�� : ���ܼ��
// �ļ��� : CameraServer.h
// ��  �� : ����
// ��  ; : ���������CameraServer,�ں��߳�����Task()���������Ϣ��ͼ��
//          �ں������ļ��м�����߳�DirWatchServer����
// ��  Ȩ : ��������
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ����        2016.3.20  ���Ͼɰ汾
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
//��������ͷ�ļ�
#pragma once
#include "DirWatcher.h"
#include "Thread.h"
#include "CommonDefine.h"
#include "VisionStruct.h"
#include "Detector.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//��������: ���������CameraServer,�ں��߳�����Task()���������Ϣ��ͼ��
//          �ں������ļ��м�����߳�DirWatchServer����
class CameraServer:public Thread
{
//�๦��
public:
	CameraServer(HANDLE  hParent = 0);
	~CameraServer();
//�ļ����
public:
	DirWatchServer *pDirWatchServer;
	//Tmtv_CameraInfo m_ImageInfo.mCameraInfo;
	BackgroundDetector m_Detector;
	static int m_CameraServerID;
	Tmtv_ImageInfo m_ImageInfo;//��������㷨�ṹ
//�̹߳���,�ڲ�����,��ֹ�ⲿ����
public:
    //�����߳�
	void  Create(int times = -1, long waiteTime = 0, bool includeTaskTime = true);
	//����ִ�й�����߳�
	void  Resume(void);
	//�����߳�
	void  Suspend(void);
	//���ò���ʹ�������˳��������߳�
	void  Destroy(void);
	//����WindowsAPIǿ�ƽ�����ǰ�߳�
	void  ForceEnd(void);
//������
public:
	//����pDirWatchServer�е�ͼ�����
	void Task();

//��Ϣ����,�ɸ��̵߳��û򷵻ظ����߳�
	//������,����CameraServer����, ׼��pDirWatchServer����
	bool AddCamera(Tmtv_CameraInfo cameraInfo);
	//ɾ�����,����CameraServer����, ׼��pDirWatchServer����
	bool DelCamera(Tmtv_CameraInfo cameraInfo);
	//�����,����pDirWatchServer����
	bool StartCamera(Tmtv_CameraInfo cameraInfo);
	//ֹͣ���,����pDirWatchServer����
	bool StopCamera(Tmtv_CameraInfo cameraInfo);
	//�������,����pDirWatchServer����
	bool SetCamera(Tmtv_CameraInfo cameraInfo);
	//������㷨,����m_Detector����
	bool StartAlgorithm(Tmtv_CameraInfo cameraInfo);
	//ֹͣ����㷨,����m_Detector����
	bool StopAlgorithm(Tmtv_CameraInfo cameraInfo);
	//ֹͣ����㷨,����m_Detector����
	bool SetAlgorithm(Tmtv_CameraInfo cameraInfo);
};
int CameraServer::m_CameraServerID=0;
//==============================================================================
///</class_info>





