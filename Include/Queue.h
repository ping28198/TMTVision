///////////////////////////////////////////////////
/** \file Queue.h
 *  \brief Simple template circle queue,not support newed data
 *
 *  \note
 *
 ** Status:
 *  
 *       Store as an array
 *           ┏━━━━━┓
 *      ┏━▶┃  Item × ┃◀━p_DataList
 *      ┃   ┃  Item × ┃
 *      ┃   ┃  Item × ┃
 *
 *     as a  ┃  Item 1  ┃      ◀━┛
 *     ring  ┃     :    ┃
 *      ┃   ┃     :    ┃
 *      ┃   ┃  Item N  ┃◀━p_Tail┏━AddTail: do not accept if IsFull()
 *      ┃   ┃  Item × ┃      ◀━┛   /ForcTail: accept if IsFull()
 *      ┗━ ┃  Item × ┃
 *           ┗━━━━━┛
 *
 *     Index ┃  Item 0  ┃◀━p_Head┏━DelHead
 *
 *  \author Leon Contact: towanglei@163.com
 *  \copyright TMTeam
 *  \version 3.0
 *  \History:
 *     Leon 2016/05/02 17:35 Fix comments\n
 *     3.0 : Leon 2016/04/02 17:35 Debug
 *     1.0 : Leon 2013/02/20 17:35 build
 */
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class Queue : 
 *  \brief Simple template circle queue,not support newed data
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/03 0:03
 */
#pragma once
template <typename T>
class Queue
{
public:
	enum {DEFAULTNUM = 32 ///< 默认数据量
	};
	/// 队列数据空间
	T* p_DataList;
	/// 队列数据数量
	int m_DataNum;
	/// 队列头指针
	int p_Head;
	/// 队列尾指针
	int p_Tail;
	/// 构造函数创建队列并初始化为空,暂不支持拷贝
	Queue()//int dataNum=DEFAULTNUM)
	{
		p_Head=0;
		p_Tail=0;
		p_DataList=0;
		m_DataNum=0;
	}
	/// 析构
	~Queue(void)
	{
		if (p_DataList!=0)
		{
			delete[] p_DataList;
			p_DataList=0;
		}
	}
	/// 初始化资源
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
	/// 卸载资源
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
	/// 设置最大存储数据量
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
	/// 获取最大存储数据量
	int GetSize()
	{
		return m_DataNum;
	}
	/// 获取有效数据数量
	int GetLength()
	{
		int num=p_Tail-p_Head;
		num=num<0?num+m_DataNum:num;
		return num;
		//return m_DataNum;
	}
	/// 清空数据
	void Clear()
	{
		p_Head=0;
		p_Tail=0;
		//m_DataNum=0;
	}
	/// 是否为空
	bool IsEmpty()
	{
		return p_Head==p_Tail;
	}
	/// 是否存满
	bool IsFull()
	{
		return p_Head-p_Tail==1||(p_Head==0&&p_Tail==m_DataNum-1);
	}
	/// 删除头数据
	bool DelHead()
	{
		if (IsEmpty()) return false;
		//p_Head
		//delete p_Head;
		p_Head++;
		if (p_Head>=m_DataNum)
		{
			p_Head-=m_DataNum;
		}
		return true;
	}
	/// 保护头数据前提下添加尾数据
	bool AddTail(const T& data)
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
	/// 强制添加尾数据
	bool ForcTail(const T& data)
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
	/// 获得尾数据
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
	/// 获得尾数据
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
	/// 获得头数据
	bool GetHead(T& data)
	{
		if (IsEmpty()) return false;
		memcpy(&data,(p_DataList+p_Head),sizeof(T));
		//data=*(p_DataList+p_Head);
		return true;
	}
	/// 获得头数据
	T* GetHead()
	{
		if (IsEmpty()) return 0;
		return (p_DataList+p_Head);
		//memcpy(&data,(p_DataList+p_Head),sizeof(T));
		//data=*(p_DataList+p_Head);
		//return true;
	}
	/// 获得正向计数数据
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
	/// 获得正向计数数据
	T* GetData(int index)
	{
		if (IsEmpty()) return 0;
		if (index>=GetLength()|| index<0) return 0;
		int pRead=p_Head+index;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		return (p_DataList+pRead);
		//memcpy(&data,(p_DataList+pRead),sizeof(T));
		//data=*(p_DataList+pRead);
		//return true;
	}
	/// 获得反向计数数据
	bool GetLast(T& data,int index)
	{
		if (IsEmpty()) return false;
		if (index>=GetLength()|| index<0) return false;
		int pRead=p_Tail-index;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		memcpy(&data,(p_DataList+pRead),sizeof(T));
		data=*(p_DataList+pRead);
		return true;
	}
	/// 获得反向计数据
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
///////////////////////////////////////////////////