#include "XMLOperator.h"
/////////////////////////////////////////////////////////////////
//构造函数
XMLOperator::XMLOperator(void)
{
	m_bWriteNewXML = false;
	m_pRoot = NULL;
	//there to CoInitializeEx(NULL,);
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
}

XMLOperator::~XMLOperator(void)
{
	//CoUninitialize();
}
////////////////////////////////////////////////////////////////
//接口函数

//---------------------------------------------------------------------
//描述: 添加根节点
void XMLOperator::AddRoot(wchar_t* strName, wchar_t* strText)
{
	if(m_pRoot)
	{
		DestroyRoot(m_pRoot);
		m_pRoot = NULL;
	}
	m_pRoot = new tagNode();
	wcscpy_s(m_pRoot->m_strName, _countof(m_pRoot->m_strName), strName);
	if(strText)
	{
		wcscpy_s(m_pRoot->m_strText, _countof(m_pRoot->m_strText), strText);
	}else
	{
		wsprintf(m_pRoot->m_strText, L"%s", L"") ;

	}
}

//---------------------------------------------------------------------
//描述: 添加子节点
void XMLOperator::AddChild(wchar_t* strParent, wchar_t* strName, wchar_t* strText)
{
	PNode pDestNode = NULL;
	FindNode(strParent,m_pRoot, pDestNode);
	if(pDestNode)
	{
		PNode pInsertNode = new Node();
		pInsertNode->m_pParent = pDestNode;
		wcscpy_s(pInsertNode->m_strName, strName);
		if(strText && wcslen(strText) < 50)
		{
			wcscpy_s(pInsertNode->m_strText, strText);
		}else
		{
			wsprintf(pInsertNode->m_strText, L"%s", L"") ;
		}

		
		
		if(NULL == pDestNode->m_pChild)
		{
			 pDestNode->m_pChild = pInsertNode;
		}
		else
		{
			PNode pBrotherNode = pDestNode->m_pChild;
			while(pBrotherNode->m_pBrother && pInsertNode)
			{
				pBrotherNode = pBrotherNode->m_pBrother;
			}
			pBrotherNode->m_pBrother = pInsertNode;
		}
	}
}
//---------------------------------------------------------------------
//描述: 保存XML文件
bool XMLOperator::SaveXML(wchar_t* strFileName)
{
	if(NULL == m_pRoot)
	{
		return false;
	}
	
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(!SUCCEEDED(hr)) 
    {                
		MessageBoxW(NULL, L"无法创建DOMDocument对象, 请检查是否安装了MS XML Parser 运行库!", L"错误", MB_OK); 
		return false;
    } 

	if(NULL == m_pRoot)
	{
		MessageBoxW(NULL, L"无法创建空的XML文件", L"错误", MB_OK);
		return false;
	}
	_variant_t  vtFileName(strFileName);	
	
	MSXML2::IXMLDOMElementPtr  xmlRoot ;
	pDoc->raw_createElement(_bstr_t(m_pRoot->m_strName), &xmlRoot);
	if(0 == xmlRoot)
	{
		wchar_t strErrorInfo[100];
		wsprintf(strErrorInfo, L"无效的参数名称:%s", m_pRoot->m_strName);
		MessageBox(NULL,strErrorInfo , L"错误", MB_OK);
	}else
	{
		if(wcscmp(m_pRoot->m_strText, L"") != 0)
		{
			xmlRoot->put_text(_bstr_t(m_pRoot->m_strText));
		}
		pDoc->raw_appendChild(xmlRoot, NULL);

		AddNodeToXML(m_pRoot, xmlRoot, pDoc);
	
		pDoc->save(vtFileName);
	}
	return true;
}

//---------------------------------------------------------------------
//描述: 读取XML文件
bool  XMLOperator::LoadXML(wchar_t* strFileName)
{
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(!SUCCEEDED(hr)) 
    {                
		MessageBoxW(NULL, L"无法创建DOMDocument对象, 请检查是否安装了MS XML Parser 运行库!", L"错误", MB_OK); 
		return false;
    } 
	_variant_t vtFileName(strFileName);
	if(!pDoc->load(vtFileName))
	{
		MessageBoxW(NULL, L"无法读取XML文件", L"错误", MB_OK);
		return false;	

	}else
	{
		MSXML2::IXMLDOMElementPtr pParent = pDoc->documentElement;
		MSXML2::IXMLDOMNodePtr pChild;
		pParent->get_firstChild(&pChild);
		_bstr_t strRoot = pParent->GettagName();


		BSTR strChild;
		BSTR strText;

		if(IsLeaf(pParent))
		{
			pParent->get_text(&strText);
			AddRoot(strRoot, strText);
		}else
		{
			AddRoot(strRoot);
		}
	
		if(pChild)
		{
			pChild->get_nodeName(&strChild);		    
			if(IsLeaf(pChild))
			{
				pChild->get_text(&strText);
				AddChild(strRoot, strChild, strText);
			}else
			{
				AddChild(strRoot, strChild);
			}		
			
			GetXMLNodeInfo(pChild);
		}
		while(true)
		{
			pChild = pChild->nextSibling;
			if(pChild)
			{
				pChild->get_nodeName(&strChild);	
				if(IsLeaf(pChild))
				{
					pChild->get_text(&strText);	
					AddChild(strRoot, strChild, strText);
				}else
				{
					AddChild(strRoot, strChild);
				}				
				
				GetXMLNodeInfo(pChild);
			}else
			{
				break;
			}
		}

	}
	pDoc.Release();

	return true;
}
//---------------------------------------------------------------------
//描述: 获取参数值
bool XMLOperator::GetValue(wchar_t* strName, wchar_t* strValue)
{
	PNode pDestNode = NULL;
	FindNode(strName,m_pRoot, pDestNode);
	if(pDestNode)
	{
		wsprintf(strValue, L"%s", pDestNode->m_strText);
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------
//描述: 
bool  XMLOperator::ChangeValue(SHORTWSTR strNameArray[],int iDepth, wchar_t*  strValue)
{
	PNode pDestNode = NULL;

	FindNode(strNameArray[0],m_pRoot, pDestNode);
	if(1 == iDepth)
	{
		wsprintf(pDestNode->m_strText, L"%s",  strValue);
		return true;
	}
	
	if(pDestNode)
	{
		PNode pTempNode = NULL;
		for(int i = 1; i < iDepth; i++)
		{
			bool bResult = FindNode(strNameArray[i],pDestNode, pTempNode);	
			if(pTempNode && bResult)
			{
				pDestNode = pTempNode; 
			}else
			{
				return false;
			}
		}
		wsprintf(pDestNode->m_strText,  L"%s", strValue);
	}else
	{
		return false;
	}

	return true;	

}
//---------------------------------------------------------------------
//描述: 
bool XMLOperator::GetValue(SHORTWSTR strNameArray[],int iDepth, SHORTWSTR& strValue)
{
	PNode pDestNode = NULL;

	FindNode(strNameArray[0],m_pRoot, pDestNode);
	if(1 == iDepth)
	{
		wsprintf(strValue, L"%s", pDestNode->m_strText);
		return true;
	}
	
	if(pDestNode)
	{
		PNode pTempNode = NULL;
		for(int i = 1; i < iDepth; i++)
		{
			bool bResult = FindNode(strNameArray[i],pDestNode, pTempNode);	
			if(pTempNode && bResult)
			{
				pDestNode = pTempNode; 
			}else
			{
				return false;
			}
		}
		wsprintf(strValue, L"%s", pDestNode->m_strText);
	}else
	{
		return false;
	}


	return true;	
}

/////////////////////////////////////////////////////////////////
//私有函数

//------------------------------------------------------------------
//描述: 销毁根结点下的所有数据
void    XMLOperator::DestroyRoot(PNode  pRoot)
{
	DestroyNode(pRoot);
}
//------------------------------------------------------------------
//描述: 销毁指定的节点
void    XMLOperator::DestroyNode(PNode pNode)
{
	if(pNode && NULL != pNode->m_pChild)
	{
		DestroyNode(pNode->m_pChild);
		DestroyNode(pNode->m_pBrother);
	}
	
	
	if(pNode && NULL == pNode->m_pChild)
	{
		//PNode pParent = pNode->m_pParent;
		//pParent->m_pChild = NULL;
		
		PNode pDelNode;	
		while(pNode)
		{
			pDelNode = pNode;
			pNode = pNode->m_pBrother;
			delete pDelNode;
			pDelNode = NULL;
		}		
		return;
	}
}

//------------------------------------------------------------------
//描述: 查找指定的节点
bool   XMLOperator::FindNode(wchar_t* strName, PNode pNode, PNode& pDestNode)
{
	if(wcscmp(pNode->m_strName, strName) == 0)
	{
		pDestNode =  pNode;
		return true;
	}else
	{
		bool bResult = false;
		if(pNode->m_pChild)
		{
			bResult = FindNode(strName, pNode->m_pChild, pDestNode);
		}
		if(pNode->m_pBrother && false == bResult)
		{
			bResult = FindNode(strName, pNode->m_pBrother, pDestNode);
		}
		return bResult;
	}
}
//------------------------------------------------------------------
//描述: 添加节点到XML文件中
void    XMLOperator::AddNodeToXML(PNode pParent, 
								   MSXML2::IXMLDOMElementPtr& pXMLParent,
								   MSXML2::IXMLDOMDocumentPtr pDoc)
{
	MSXML2::IXMLDOMElementPtr  xmlElement ;
	if(NULL == pParent)
	{
		return;
	}else
	{
		if(pParent->m_pParent != NULL)
		{
			
			pDoc->raw_createElement(_bstr_t(pParent->m_strName), &xmlElement);
			if(0 == xmlElement)
			{
				wchar_t strErrorInfo[100];
				wsprintf(strErrorInfo, L"无效的参数名称:%s", pParent->m_strName);
				MessageBox(NULL,strErrorInfo , L"错误", MB_OK);
				
			}
			else
			{
				if(wcscmp(pParent->m_strText, L"") != 0)
				{
					xmlElement->put_text(_bstr_t(pParent->m_strText));
				}
				pXMLParent->appendChild(xmlElement);
			}
		}else
		{
			xmlElement = pXMLParent;
		}

	}

	PNode pChild = pParent->m_pChild;
	while(pChild)
	{
		AddNodeToXML(pChild, xmlElement, pDoc);
		pChild = pChild->m_pBrother;		
	}
}
//------------------------------------------------------------------
//描述: 获取XML文件中的节点信息
void    XMLOperator::GetXMLNodeInfo(MSXML2::IXMLDOMNodePtr pNode)
{
	if(0 == pNode)
	{
		return;
	}
	MSXML2::IXMLDOMNodePtr pOldNode = pNode;
	BSTR strNodeName;
	pNode->get_nodeName(&strNodeName);

	pNode->get_firstChild(&pNode);	
	if(pNode)
	{
		BSTR strText;
		BSTR strName;
		pNode->get_nodeName(&strName);
		if(IsLeaf(pNode))
		{
			pNode->get_text(&strText);
			if(wcscmp(strName, L"#text") != 0)
			{
				AddChild(strNodeName, strName, strText);
			
			}
		}else
		{
			if(wcscmp(strName, L"#text") != 0)
			{
				AddChild(strNodeName, strName);
			
			}
		}
	}
	MSXML2::IXMLDOMNodePtr pChild;
	while(pNode)
	{
		pNode->get_nextSibling(&pChild);
		if(pChild)
		{
			BSTR strName;
			BSTR strText;
			pChild->get_nodeName(&strName);
			if(IsLeaf(pNode))
			{
				pChild->get_text(&strText);		
				AddChild(strNodeName, strName, strText);
			}else
			{
				AddChild(strNodeName, strName);
			}
			pNode = pChild; 
		}else
		{
			break;
		}
	}
    //进行下一组计算
	pOldNode->get_firstChild(&pOldNode);
	BSTR strName;
	if(pNode)
	{
		pNode->get_nodeName(&strName);
		if(wcscmp(strName, L"#text") != 0)
		{
			GetXMLNodeInfo(pOldNode);			
		}
	}
	while(pOldNode)
	{
		pOldNode = pOldNode->nextSibling;
		if(0 == pOldNode)
		{
			break;
		}else
		{
			GetXMLNodeInfo(pOldNode);
		}
	}
}

//------------------------------------------------------------------
//描述: 
bool   XMLOperator::IsLeaf(MSXML2::IXMLDOMNodePtr pNode)
{
	MSXML2::IXMLDOMNodeListPtr pList =  pNode->GetchildNodes();
	long length;
	pList->get_length(&length);
	if(length > 1)
	{
		return false;	
	}else
	{
		MSXML2::IXMLDOMNodePtr pTempChild = pNode->GetfirstChild();
		if(!pTempChild) return true;
		if(pTempChild && !pTempChild->GetfirstChild())
		{
			return true;
		}else
		{
			return false;
		}
	}


}

