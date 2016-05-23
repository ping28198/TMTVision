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
 *     Index ┃  Item 0  ┃◀━p_Head┏━DelHead
 *     as a  ┃  Item 1  ┃      ◀━┛
 *     ring  ┃     :    ┃
 *      ┃   ┃     :    ┃
 *      ┃   ┃  Item N  ┃◀━p_Tail┏━AddTail: do not accept if IsFull()
 *      ┃   ┃  Item × ┃      ◀━┛   /ForcTail: accept if IsFull()
 *      ┗━ ┃  Item × ┃
 *           ┗━━━━━┛
 *
 *
 *  \author Leon Contact: towanglei@163.com
 *  \copyright TMTeam
 *  \version 4.1beta
 *  \History:
 *     Leon 2016/05/17 17:00 Add Err code and try-catch block.\n
 *     4.0beta : Leon 2016/05/17 17:00 Priority use T::operator= if has(memcopy if not),when deep copy data in.\n
 *     3.1 : Leon 2016/05/02 17:35 Fix comments\n
 *     3.0 : Leon 2016/04/02 17:35 Debug
 *     1.0 : Leon 2013/02/20 17:35 build
 */
///////////////////////////////////////////////////

#pragma once
#include <exception>
using namespace std;
#ifndef QUE_ERR
#define QUE_NORMAL     (int)0
#define QUE_ERR        (int)-1000
#define QUE_ERR_MEMOUT (int)QUE_ERR-1
#define QUE_ERR_LOCKED (int)QUE_ERR-2
#endif
///////////////////////////////////////////////////
/** \fn  CopyData
 *  \brief Priority use T::operator= if has(memcopy if not),when deep copy data in.
 *  \param[in] T& src
 *  \param[out] T& dst
 *  \return bool
 */
template <typename T>
bool CopyData(T& dst,const T& src)
{
	__if_exists(T::operator=)
	{
		dst = src;
	}
	__if_not_exists(T::operator=)
	{
		memcpy_s(&dst, sizeof(T), &src, sizeof(T));
	}
	return true;
    return false;
}
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class Queue : 
 *  \brief Simple template circle queue,not support newed data
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/03 0:03
 */
template <typename T>
class Queue
{
private:
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
	/// 错误码
	int m_ErrCode = QUE_NORMAL;
public:
	int GetErrCode() 
	{
		return m_ErrCode
	}
public:
	/// 构造函数创建队列并初始化为空,暂不支持拷贝
	Queue()//int dataNum=DEFAULTNUM)
	{
		p_Head=0;
		p_Tail=0;
		p_DataList=0;
		m_DataNum=0;
	}
	/// 析构
	virtual ~Queue(void)
	{
		try
		{
			if (p_DataList!=0)
			{
				delete[] p_DataList;
				p_DataList=0;
			}
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
		}
	}
	/// 初始化资源
	virtual bool Initial(int dataNum = DEFAULTNUM)
	{
		m_ErrCode = QUE_NORMAL;
		try
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
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		return true;
	}
	/// 卸载资源
	virtual bool Unitial()
	{
		m_ErrCode = QUE_NORMAL;
		try
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
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		return true;
	}
	/// 设置最大存储数据量
	virtual int SetSize(int dataNum)
	{
		m_ErrCode = QUE_NORMAL;
		try
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
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return QUE_ERR_MEMOUT;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return QUE_ERR_MEMOUT;
		}
		return m_DataNum;
	}
	/// 获取最大存储数据量
	virtual int GetSize()
	{
		return m_DataNum;
	}
	/// 获取有效数据数量
	virtual int GetLength()
	{
		int num=p_Tail-p_Head;
		num=num<0?num+m_DataNum:num;
		return num;
		//return m_DataNum;
	}
	/// 清空数据
	virtual void Clear()
	{
		p_Head=0;
		p_Tail=0;
		//m_DataNum=0;
	}
	/// 是否为空
	virtual bool IsEmpty()
	{
		return p_Head==p_Tail;
	}
	/// 是否存满
	virtual bool IsFull()
	{
		return (p_Head-p_Tail==1||(p_Head==0&&p_Tail==m_DataNum-1));
	}
	/// 删除头数据
	virtual bool DelHead()
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsEmpty()) return false;
			//p_Head
			//delete p_Head;
			p_Head++;
			if (p_Head>=m_DataNum)
			{
				p_Head-=m_DataNum;
			}
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		return true;
	}
	/// 保护头数据前提下添加尾数据
	virtual bool AddTail(const T& data)
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsFull()) return false;
			//Priority use T::operator= if has(memcopy if not),when deep copy data in.
			CopyData(*(p_DataList + p_Tail), data);
			//memcpy((p_DataList+p_Tail),&data,sizeof(T));
			//*(p_DataList+p_Tail)=data;
			p_Tail++;
			if (p_Tail>=m_DataNum)
			{
				p_Tail-=m_DataNum;
			}
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		return true;
	}
	/// 强制添加尾数据
	virtual bool ForcTail(const T& data)
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsFull()) DelHead();
			//Priority use T::operator= if has(memcopy if not),when deep copy data in.
			CopyData(*(p_DataList + p_Tail), data);
			//memcpy((p_DataList+p_Tail),&data,sizeof(T));
			//*(p_DataList+p_Tail)=data;
			p_Tail++;
			if (p_Tail>=m_DataNum)
			{
				p_Tail-=m_DataNum;
			}
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		return true;
	}
	/// 获得尾数据
	virtual bool GetTail(T& data)
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsEmpty()) return false;
			if (p_Tail>0)
			{
				//Priority use T::operator= if has(memcopy if not),when deep copy data in.
				return CopyData(data, *(p_DataList + p_Tail-1));
				//memcpy(&data,(p_DataList+p_Tail-1),sizeof(T));
				//data=*(p_DataList+p_Tail-1);
			}
			else
			{
				//Priority use T::operator= if has(memcopy if not),when deep copy data in.
				return CopyData(data, *(p_DataList + m_DataNum - 1));
				//memcpy(&data, (p_DataList + m_DataNum - 1), sizeof(T));
				//data=*(p_DataList+m_DataNum-1);
			}
			//return true;
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
	}
	/// 获得尾数据
	virtual T* GetTail()
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsEmpty()) return 0;
			if (p_Tail > 0)
			{
				//memcpy(&data,(p_DataList+p_Tail-1),sizeof(T));
				//data=*(p_DataList+p_Tail-1);
				return (p_DataList + p_Tail - 1);
			}
			else return (p_DataList + m_DataNum - 1);
			//memcpy(&data,(p_DataList+m_DataNum-1),sizeof(T));
			//data=*(p_DataList+m_DataNum-1);
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return 0;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return 0;
		}
	}
	/// 获得头数据
	virtual bool GetHead(T& data)
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsEmpty()) return false;
			//Priority use T::operator= if has(memcopy if not),when deep copy data in.
			return CopyData(data, *(p_DataList + p_Head));
			//memcpy(&data,(p_DataList+p_Head),sizeof(T));
			//data=*(p_DataList+p_Head);
			//return true;
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
	}
	/// 获得头数据
	virtual T* GetHead()
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsEmpty()) return 0;
			return (p_DataList + p_Head);
			//memcpy(&data,(p_DataList+p_Head),sizeof(T));
			//data=*(p_DataList+p_Head);
			//return true;
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return 0;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return 0;
		}
	}
	/// 获得正向计数数据
	virtual bool GetData(T& data,int index)
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsEmpty()) return false;
			if (index >= GetLength() || index < 0) return false;
			int pRead = p_Head + index;
			if (pRead >= m_DataNum) pRead -= m_DataNum;
			if (pRead < 0) pRead += m_DataNum;
			//Priority use T::operator= if has(memcopy if not),when deep copy data in.
			return CopyData(data, *(p_DataList + pRead));
			//memcpy(&data,(p_DataList+pRead),sizeof(T));
			//data=*(p_DataList+pRead);
			//return true;
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
	}
	/// 获得正向计数数据
	virtual T* GetData(int index)
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsEmpty()) return 0;
			if (index >= GetLength() || index < 0) return 0;
			int pRead = p_Head + index;
			if (pRead >= m_DataNum) pRead -= m_DataNum;
			if (pRead < 0) pRead += m_DataNum;
			return (p_DataList + pRead);
			//memcpy(&data,(p_DataList+pRead),sizeof(T));
			//data=*(p_DataList+pRead);
			//return true;
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return 0;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return 0;
		}
	}
	/// 获得反向计数数据
	virtual bool GetLast(T& data,int index)
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsEmpty()) return false;
			if (index >= GetLength() || index < 0) return false;
			int pRead = p_Tail - index - 1;
			if (pRead >= m_DataNum) pRead -= m_DataNum;
			if (pRead < 0) pRead += m_DataNum;
			//Priority use T::operator= if has(memcopy if not),when deep copy data in.
			return CopyData(data, *(p_DataList + pRead));
			//memcpy(&data,(p_DataList+pRead),sizeof(T));
			//data=*(p_DataList+pRead);
			//return true;
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return false;
		}
	}
	/// 获得反向计数据
	virtual T* GetLast(int index)
	{
		m_ErrCode = QUE_NORMAL;
		try
		{
			if (IsEmpty()) return 0;
			if (index >= GetLength() || index < 0) return 0;
			int pRead = p_Tail - index - 1;
			if (pRead >= m_DataNum) pRead -= m_DataNum;
			if (pRead < 0) pRead += m_DataNum;
			return (p_DataList + pRead);
			//memcpy(&data,(p_DataList+pRead),sizeof(T));
			//data=*(p_DataList+pRead);
			//return true;
		}
		catch (int& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return 0;
		}
		catch (exception& e)
		{
			m_ErrCode = QUE_ERR_MEMOUT;
			return 0;
		}
	}
};
///////////////////////////////////////////////////