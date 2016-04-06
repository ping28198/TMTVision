///<proj_info>
//==============================================================================
// ��Ŀ�� : ���ܼ��
// �ļ��� : ObjToString.h
// ��  �� : ����
// ��  ; : ��Ҫ�ṹ��תΪ�ַ���������ʾ
// ��  Ȩ : ��������
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ����        2016.4.6    ����
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
#pragma once
#include "VisionStruct.h"
#include "CommonFunc.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//��������:�ṹ��ת��Ϊ�ַ���
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
class ObjToString
{
public:
	static void ToString(MEGAWSTR wstring, Tmtv_AlgorithmInfo algorithmInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_CameraInfo  camraInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_DefectInfo  defectInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_ImageInfo  imageInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_AskInfo  askInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_MsgInfo  msgInfo, int method = 0, int color = 37);
	static int InfoCheck(void* pBuffer, long  bufferSize);
	static int ToString(MEGAWSTR wstring, void* pBuffer, long  bufferSize, int method = 0, int color = 37);
};
//==============================================================================
///</class_info>