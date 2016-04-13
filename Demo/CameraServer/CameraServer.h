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
#include"CommonInclude.h"
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
	CameraServer(void *pParam=NULL,HANDLE  hParent = 0);
	~CameraServer();
//�ļ����
public:
	DirWatchServer *pDirWatchServer;
	//Tmtv_CameraInfo m_ImageInfo.mCameraInfo;
	BackgroundDetector m_Detector;
	//static int m_CameraServerID;
	Tmtv_ImageInfo m_ImageInfo;//��������㷨�ṹ

	void *pParent;//����ָ��

//�̹߳���,�ڲ�����,��ֹ�ⲿ����
public:
	//�����ļ��ж�ȡ���, ��ʼ��ReadDirectoryChangesW��ز���
	//ִ��ǰǿ��ֹͣ�߳�, ��Ҫ��Create()�ٴ�����,�����̱߳���
	bool RegPath(PATHWSTR path, DWORD action = FILE_NOTIFY_CHANGE_LAST_WRITE);
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
	bool AddCamera(Tmtv_CameraInfo& cameraInfo);
	//ɾ�����,����CameraServer����, ׼��pDirWatchServer����
	bool DelCamera();
	//�����,����pDirWatchServer����
	bool StartCamera();
	//ֹͣ���,����pDirWatchServer����
	bool StopCamera();
	//�������,����pDirWatchServer����
	bool SetCamera(Tmtv_CameraInfo& cameraInfo);
	//������㷨,����m_Detector����
	bool StartAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo);
	//ֹͣ����㷨,����m_Detector����
	bool StopAlgorithm();
	//ֹͣ����㷨,����m_Detector����
	bool SetAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo);

public:
	//���Ժ���,��ʾ������Ϣ
	//method=0 xml ������ʽ���, ������
	//1 ��̸�ʽ���,ֻ�����������
	//2 ���¸�ʽ���,�����������Ԫ��
	//�ֱ�����ɫ��Χ: 40--49        ����ɫ: 30--39
	//	40 : ��                           30 : ��
	//	41 : ��                           31 : ��
	//	42 : ��                           32 : ��
	//	43 : ��                           33 : ��
	//	44 : ��                           34 : ��
	//	45 : ��                           35 : ��
	//	46 : ����                         36 : ����
	//	47 : ��ɫ                         37 : ��ɫ
	void ToString(MEGAWSTR &string, int method = 0, int color = 32);
};

//==============================================================================
///</class_info>





