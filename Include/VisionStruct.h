///<proj_info>
//==============================================================================
// ��Ŀ�� �����ܼ��
// �ļ��� ��VisionStruct.h
// ��  �� ������ƽ
// ��  ; ����Ҫ�ṹ��
// ��  Ȩ ����������
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ����ƽ      2016.3.20  ���Ͼɰ汾
//1.1     ����        2016.3.29  ��������ֱ���
//                               ������Ϣö��
//                               ���Ӿ�������������Ϣ
//1.2     ����        2016.3.29  �������״̬�ռ����,��Ӧ�����������Ϣ
//2.0     ����        2016.3.29  *�������㷨״̬�ռ����
//==============================================================================
///</ver_info>

#pragma once
#include "CommonDefine.h" 

///<data_info>
//==============================================================================
//��������:�����Ϣ��
//         ���״̬�ռ�:
//                  
//    �D�D�D�D�D�D TMTV_NOCAM
//   |           ��           ��
//   |      TMTV_ADDCAM / TMTV_DELCAM  ���D�D
//   |           ��           ��            |
//   |           TMTV_STOPEDCAM             |
//   |           ��           ��            |
//    �D�� TMTV_STERTCAM / TMTV_STOPCAM     |
//               ��           ��            |
//               TMTV_RUNNINGCAM �D�D�D�D�D��
//
//==============================================================================
///</data_info>
struct Tmtv_CameraInfo //�����Ϣ
{
	int Indexnum=0;//������
	char CameraName[TMTV_MAXSTRLEN];//������Ƶ���Ϣ
	char CameraPath[TMTV_MAXSTRLEN];//�����Ŀ¼·��
	char MaskImgPath[TMTV_MAXSTRLEN];//����ͼ��λ��
	int CameraPos[8];//x,y,z���꣬����
	int CameraWidth = 1920;//1.1 //���ͼƬ���
	int CameraHeight = 1080;//1.1 //���ͼƬ�߶�	
	enum {//2.0 
		    TMTV_NOWARN = 0,     //�������㷨
	    	TMTV_PREWARN = 1,    //�����㷨������
		    TMTV_STARTWARN = 2,  //���������㷨
	};
	int WarnningLevel;//����ȼ�,�����㷨����״̬
	enum {//1.2
		    TMTV_NOCAM = 300,      //δ���ص����
	    	TMTV_STOPEDCAM = 301,  //�Ѽ��ص���ͣ���
		    TMTV_RUNNINGCAM = 302, //�Ѽ��ص��������
	};
	int Status;//1.2//���״̬
	char Reservechar[TMTV_MAXSTRLEN];//����
};

struct Tmtv_DefectInfo	//ȱ����Ϣ
{
	int DefectNum=0;//ȱ������
	int DefectPos[TMTV_MAXDEFECTNUM][8];//left,top,width,height,type,level,..
	int ImgWidth=1920;
	int ImgHeight=1080;//ָ��ͼƬ��С������ͼƬ�ߴ����֮�����ȱ����Ϣ
};

struct Tmtv_ImageInfo	//ͼ����Ϣ���������˷����������
{
	Tmtv_CameraInfo mCameraInfo;
	Tmtv_DefectInfo mDefectInfo;
	char ImagePath[TMTV_MAXSTRLEN];//�ļ�·��
	char GrabTime[64];//�ɼ�ʱ�� ��ʽ����-��-��-ʱ:�֣��롣����2016-03-22-12:00:00
	int IsWarnning=0;//�Ƿ��о���
	int IsVIP=0;//�����Ƿ����ص�
	char Reservechar[TMTV_MAXSTRLEN];//����
};

struct Tmtv_AskInfo//ָ����Ϣ��������˷�����������
{
	int CheckCode = TMTV_CHECKCODE;//��֤�룬��ֹͨ�Ÿ���
    //�������ͣ�101���ر�CameraInfoָ���������102��������CameraInfoָ��������������ݣ�103�����CameraInfoָ�������
	//104���޸�CameraInfo�����ָ���������Ϣ
	enum { 
		   TMTV_ADDCAM = 100,        //���CameraInfoָ����δ�������
		   TMTV_DELCAM = 101,        //ɾ��CameraInfoָ����δ�������
		   TMTV_STERTCAM = 102,      //����CameraInfoָ�����Ѽ������
		   TMTV_STOPCAM = 103,       //�ݶ�CameraInfoָ�����Ѽ������
		   TMTV_GETCAM = 104,        //��ѯCameraInfoָ�����Ѽ��������ָ������
		   TMTV_SETCAM = 105,        //����CameraInfoָ�����Ѽ��������ָ������
	};
	int Asktype;
	Tmtv_CameraInfo CameraInfo;
	DWORD hAskHandle;   //1.1//�����߳̾��,=0��Ч,���ڵ���,��ʱ���򲻵���
	DWORD hAnswerHandle;//1.1//Ӧ���߳̾��,=0��Ч,���ڵ���,��ʱ���򲻵���
};
struct Tmtv_MsgInfo//ָ����Ϣ���������˷����������
{
	int CheckCode = TMTV_CHECKCODE;//��֤�룬��ֹͨ�Ÿ���
	enum {
		   TMTV_ADDCAM_OK = 200,        //���CameraInfoָ����δ��������ɹ�
		   TMTV_DELCAM_OK = 201,        //ɾ��CameraInfoָ����δ��������ɹ�
		   TMTV_STERTCAM_OK = 202,      //����CameraInfoָ�����Ѽ�������ɹ�
		   TMTV_STOPCAM_OK = 203,       //�ݶ�CameraInfoָ�����Ѽ�������ɹ�
		   TMTV_GETCAM_OK = 204,        //��ѯCameraInfoָ�����Ѽ��������ָ�������ɹ�
		   TMTV_SETCAM_OK = 205,        //����CameraInfoָ�����Ѽ��������ָ�������ɹ�
		   TMTV_SNAPED= 298,            //�¼�⵽��mImgInfoͼƬ
	};
	int MsgType;
	//Tmtv_CameraInfo CameraInfo;//���������
	Tmtv_ImageInfo ImgInfo;
	DWORD hAskHandle;            //1.1//�����߳̾��,=0��Ч,���ڵ���,��ʱ���򲻵���
	DWORD hAnswerHandle;         //1.1//Ӧ���߳̾��,=0��Ч,���ڵ���,��ʱ���򲻵���

};