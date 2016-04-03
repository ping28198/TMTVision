///<proj_info>
//==============================================================================
// 项目名 ：智能监控
// 文件名 ：Queue.h
// 作  者 ：王磊
// 用  途 ：简单循环队列模板类
// 版  权 ：霍比特人
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     王磊        2013.2.20   创建
//2.0     王磊        2016.4.2   整合旧版本
//==============================================================================
///</ver_info>

///<algorithm_info>
//==============================================================================
//功能描述: 简单循环队列模板类
//         
//         Store as an array
//        ┏━━━━━┓
//   ┏━▶┃  Item × ┃◀━p_DataList
//   ┃   ┃  Item × ┃
//   ┃   ┃  Item × ┃
//  Index ┃  Item 0  ┃◀━ p_Head┏━DelHead()
//  as a  ┃  Item 1  ┃       ◀━┛
//  ring  ┃     :    ┃
//   ┃   ┃     :    ┃
//   ┃   ┃  Item N  ┃◀━ p_Tail┏━AddTail(): do not accept if IsFull()
//   ┃   ┃  Item × ┃       ◀━┛   /ForcTail(): accept if IsFull()
//   ┗━ ┃  Item × ┃
//        ┗━━━━━┛
//
//==============================================================================
///</algorithm_info>

///<class_info>
//==============================================================================
//功能描述:简单循环队列模板类
//         队列保存数据(非指针),添加调用数据时采用赋值操作
//         支持无赋值运算的数据类型
//         由于不包含复杂调用,所有方法写在头文件中
#pragma once
template <typename T>
class Queue
{
public:
	enum {DEFAULTNUM = 32};
	//队列数据
	T* p_DataList;
	int m_DataNum;
	//队列头尾
	int p_Head;
	int p_Tail;
	//构造函数创建队列并初始化为空,暂不支持拷贝
	Queue()//int dataNum=DEFAULTNUM)
	{
		p_Head=0;
		p_Tail=0;
		p_DataList=0;
		m_DataNum=0;
	}
	~Queue(void)
	{
		if (p_DataList!=0)
		{
			delete[] p_DataList;
			p_DataList=0;
		}
	}
	void Initial(int dataNum = DEFAULTNUM)
	{
		if (p_DataList != 0)
		{
			delete[] p_DataList;
			p_DataList = 0;
		}
		p_Head = 0;
		p_Tail = 0;
		if (dataNum<1) dataNum = DEFAULTNUM;
		p_DataList = new T[dataNum];
		m_DataNum = dataNum;
	}
	void Unitial()
	{
		if (p_DataList != 0)
		{
			delete[] p_DataList;
			p_DataList = 0;
		}
		p_Head = 0;
		p_Tail = 0;
		m_DataNum = 0;
	}
	//方法
	int SetSize(int dataNum)
	{
		if (p_DataList != 0)
		{
			delete[] p_DataList;
			p_DataList = 0;
		}
		p_Head = 0;
		p_Tail = 0;
		if (dataNum<1) dataNum = DEFAULTNUM;
		p_DataList = new T[dataNum];
		m_DataNum = dataNum;
		return m_DataNum;
	}
	int GetSize()
	{
		return m_DataNum;
	}
	int GetLength()
	{
		int num=p_Tail-p_Head;
		num=num<0?num+m_DataNum:num;
		return num;
		//return m_DataNum;
	}
	void Clear()
	{
		p_Head=0;
		p_Tail=0;
		//m_DataNum=0;
	}
	bool IsEmpty()
	{
		return p_Head==p_Tail;
	}
	bool IsFull()
	{
		return p_Head-p_Tail==1||(p_Head==0&&p_Tail==m_DataNum-1);
	}

	//删除头数据
	bool DelHead()
	{
		if (IsEmpty()) return false;
		p_Head++;
		if (p_Head>=m_DataNum)
		{
			p_Head-=m_DataNum;
		}
		return true;
	}
	//保护头数据前提下添加尾数据
	bool AddTail(T data)
	{
		if (IsFull()) return false;
		memcpy((p_DataList+p_Tail),&data,sizeof(T));
		//*(p_DataList+p_Tail)=data;
		p_Tail++;
		if (p_Tail>=m_DataNum)
		{
			p_Tail-=m_DataNum;
		}
		return true;
	}
	//强制添加尾数据
	bool ForcTail(T data)
	{
		if (IsFull()) DelHead();
		memcpy((p_DataList+p_Tail),&data,sizeof(T));
		//*(p_DataList+p_Tail)=data;
		p_Tail++;
		if (p_Tail>=m_DataNum)
		{
			p_Tail-=m_DataNum;
		}
		return true;
	}
	//获得尾数据
	bool GetTail(T& data)
	{
		if (IsEmpty()) return false;
		if (p_Tail>0)
		{
			memcpy(&data,(p_DataList+p_Tail-1),sizeof(T));
			//data=*(p_DataList+p_Tail-1);
		}
		else memcpy(&data,(p_DataList+m_DataNum-1),sizeof(T));
//data=*(p_DataList+m_DataNum-1);
		return true;
	}
	//获得尾数据
	T* GetTail()
	{
		if (IsEmpty()) return 0;
		if (p_Tail>0)
		{
			//memcpy(&data,(p_DataList+p_Tail-1),sizeof(T));
			//data=*(p_DataList+p_Tail-1);
			return (p_DataList+p_Tail-1);
		}
		else return (p_DataList+m_DataNum-1);
		//memcpy(&data,(p_DataList+m_DataNum-1),sizeof(T));
		//data=*(p_DataList+m_DataNum-1);
	}
	//获得头数据
	bool GetHead(T& data)
	{
		if (IsEmpty()) return false;
		memcpy(&data,(p_DataList+p_Head),sizeof(T));
		//data=*(p_DataList+p_Head);
		return true;
	}
	//获得头数据
	T* GetHead()
	{
		if (IsEmpty()) return 0;
		return (p_DataList+p_Head);
		//memcpy(&data,(p_DataList+p_Head),sizeof(T));
		//data=*(p_DataList+p_Head);
		//return true;
	}
	//获得数据
	bool GetData(T& data,int index)
	{
		if (IsEmpty()) return false;
		if (index>=GetLength()|| index<0) return false;
		int pRead=p_Head+index-1;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		memcpy(&data,(p_DataList+pRead),sizeof(T));
		//data=*(p_DataList+pRead);
		return true;
	}
	//获得数据
	T* GetData(int index)
	{
		if (IsEmpty()) return 0;
		if (index>=GetLength()|| index<0) return 0;
		int pRead=p_Head+index-1;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		return (p_DataList+pRead);
		//memcpy(&data,(p_DataList+pRead),sizeof(T));
		//data=*(p_DataList+pRead);
		//return true;
	}
	//获得数据
	bool GetLast(T& data,int index)
	{
		if (IsEmpty()) return false;
		if (index>=GetLength()|| index<0) return false;
		int pRead=p_Tail-index-1;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		memcpy(&data,(p_DataList+pRead),sizeof(T));
		data=*(p_DataList+pRead);
		return true;
	}
	//获得数据
	T* GetLast(int index)
	{
		if (IsEmpty()) return 0;
		if (index>=GetLength()|| index<0) return 0;
		int pRead=p_Tail-index-1;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		return (p_DataList+pRead);
		//memcpy(&data,(p_DataList+pRead),sizeof(T));
		//data=*(p_DataList+pRead);
		//return true;
	}
};
//==============================================================================
///</class_info>