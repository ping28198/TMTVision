///<proj_info>
//==============================================================================
// ��Ŀ�� �����ܼ��
// �ļ��� ��XMLOperator.h
// ��  �� �����
// ��  ; ���������ڶ�XML�ļ����в��� 
//          TaskThread   �������߳���,�ɶ�̬���������  
//          TaskThreadEx ���������߳���,�ɶ�̬�������������
//			1:�����󴴽��̲߳�ִ��ThreadMain,�߳���ThreadMain�˳�ʱ����,
//			2:�̵߳ĺ�����ThreadMain�������и�������߳��Ƿ��˳������,
//			  �߳�ִ�к�����ֶ�����͸����߳� ��������,
//			  ��������������ʱ,�̹߳���,�ȴ��µ� ��������
//			3:�߳��������ظ�ִ����������,������������жϴ���
// ��  Ȩ ����������
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ���      ���XMLOperator��
//2.0     ����        2012.2.24  ����m_strText��С�Է���������ģ�ַ���
//2.0     ����        2012.3.10  #import <MSXML2.Tlb>����Ӧ��ͬϵͳ                              
//3.0     ���      2016.4.2   ��Ϊ#import "MSXML3.dll"��������
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
#pragma once
#include "CommonDefine.h"
#import "MSXML3.dll"
using namespace MSXML2;
//==============================================================================
///</header_info>

///<datastruct_info>
//==============================================================================
//������
//�������ڵ�����ݽṹ
typedef struct tagNode
{
	wchar_t   m_strName[20];//�ڵ�����������20���ֽ�
	wchar_t   m_strText[50]; //�ڵ���ֵ, ������50���ֽ�
	tagNode*  m_pBrother;
	tagNode*  m_pParent;
	tagNode*  m_pChild;
	tagNode(tagNode* pParent = NULL)
	{
		m_pParent = pParent;
		m_pBrother = NULL;
		m_pChild = NULL;
		if (m_pParent)
		{
			PNode pNode = m_pParent->m_pChild;
			while (pNode)
			{
				pNode = pNode->m_pBrother;
			}
			pNode = this;
		}
	}
}Node, *PNode;
//==============================================================================
///</datastruct_info>

///<class_info>
//==============================================================================
//��������:XML��������
class XMLOperator
{
public:
	 void AddRoot(wchar_t* strName, wchar_t* strText = NULL);
	 void AddChild(wchar_t* strParent, wchar_t* strName, wchar_t* strText = NULL);
	 bool SaveXML(wchar_t* strFileName);

	 bool GetValue(wchar_t* strName, wchar_t* strValue);
	 bool ChangeValue(SHORTSTR strNameArray[],int iDepth, wchar_t* strValue);
	 bool GetValue(SHORTSTR strNameArray[], int iDepth, SHORTSTR& strValue);
	 bool LoadXML(wchar_t* strFileName);
public:
	XMLOperator(void);
	~XMLOperator(void);
private:
	bool m_bWriteNewXML;
	MSXML2::IXMLDOMDocumentPtr m_pDoc; 
	MSXML2::IXMLDOMElementPtr  m_xmlRoot;
	
	PNode   m_pRoot;
	PNode   m_pLoadRoot;

	void    DestroyRoot(PNode  pRoot);
	void    DestroyNode(PNode pNode);
	bool    FindNode(wchar_t* strName, PNode pNode,  PNode& pDestNode);
	void    AddNodeToXML(PNode pParent, 
		                 MSXML2::IXMLDOMElementPtr& pXMLParent,
						 MSXML2::IXMLDOMDocumentPtr pDoc);
	void    GetXMLNodeInfo(MSXML2::IXMLDOMNodePtr pNode);

	bool    IsLeaf(MSXML2::IXMLDOMNodePtr pNode);
};
//==============================================================================
///</class_info>