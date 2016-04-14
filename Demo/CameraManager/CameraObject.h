///<proj_info>
//==============================================================================
// ��Ŀ�� : ���ܼ��
// �ļ��� : CameraObject.h
// ��  �� : ����
// ��  ; : ���������CameraObject,�ں��߳�����Task()���������Ϣ��ͼ��
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
//��������: ���������CameraObject,�ں��߳�����Task()���������Ϣ��ͼ��
//          �ں������ļ��м�����߳�DirWatchServer����
class CameraObject:public Thread
{
//�๦��
public:
	void *p_Parent;//����ָ��
	CameraObject(void *pParent,HANDLE  hParent = 0);
	~CameraObject();
//�ļ����
private:

	DirWatchServer m_DirWatchServer;
	//Tmtv_CameraInfo m_ImageInfo.mCameraInfo;
	BackgroundDetector m_Detector;
	//static int m_CameraObjectID;
	Tmtv_ImageInfo m_ImageInfo;//��������㷨�ṹ


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
	//����m_DirWatchServer�е�ͼ�����
	void Task();

//��Ϣ����,�ɸ��̵߳��û򷵻ظ����߳�
	//������,����CameraObject����, ׼��m_DirWatchServer����
	bool AddCamera(Tmtv_CameraInfo& cameraInfo);
	//ɾ�����,����CameraObject����, ׼��m_DirWatchServer����
	bool DelCamera();
	//�����,����m_DirWatchServer����
	bool StartCamera();
	//ֹͣ���,����m_DirWatchServer����
	bool StopCamera();
	//�������,����m_DirWatchServer����
	bool SetCamera(Tmtv_CameraInfo& cameraInfo);
	//������㷨,����m_Detector����
	bool StartAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo);
	//ֹͣ����㷨,����m_Detector����
	bool StopAlgorithm();
	//ֹͣ����㷨,����m_Detector����
	bool SetAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo);
	//��ȡ���ImgInfo
	bool GetImgInfo(Tmtv_ImageInfo* pImgInfo);
	//��ȡ���Index
	int GetCamIndex();
	//��ȡ���CamInfo
	bool GetCamInfo(Tmtv_CameraInfo* pCamInfo);

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





