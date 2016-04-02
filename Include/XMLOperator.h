///<proj_info>
//==============================================================================
// 项目名 ：智能监控
// 文件名 ：XMLOperator.h
// 作  者 ：杨朝霖
// 用  途 ：该类用于对XML文件进行操作 
//          TaskThread   单任务线程类,可动态变更任务函数  
//          TaskThreadEx 队列任务线程类,可动态变更任务函数队列
//			1:声明后创建线程并执行ThreadMain,线程在ThreadMain退出时销毁,
//			2:线程的核心是ThreadMain主函数中负责决定线程是否退出或挂起,
//			  线程执行后可以手动挂起和更改线程 任务处理函数,
//			  当任务函数被置零时,线程挂起,等待新的 任务处理函数
//			3:线程主函数重复执行任务处理函数,主函数后包含中断处理
// 版  权 ：霍比特人
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     杨朝霖      添加XMLOperator类
//2.0     王磊        2012.2.24  扩增m_strText大小以方便载入大规模字符串
//2.0     王磊        2012.3.10  #import <MSXML2.Tlb>以适应不同系统                              
//3.0     杨朝霖      2016.4.2   改为#import "MSXML3.dll"王磊整理
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
//任务函数
//描述：节点的数据结构
typedef struct tagNode
{
	wchar_t   m_strName[20];//节点名，不超过20个字节
	wchar_t   m_strText[50]; //节点数值, 不超过50个字节
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
//功能描述:XML操作对象
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