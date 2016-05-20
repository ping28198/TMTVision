///////////////////////////////////////////////////
/** \file SafeQueue.h
 *  \brief Multi-thread safe queue, only support one thread read or write
 *  \note
 *    Any type object is unsafe when is being written by more than one threads;\n
 *    basic type object is safe in reading, when it is in writting by only one thread;\n
 *    but queue is still unsafe in reading, when another thread is writting the queue;\n
 *  \author Leon Contact: towanglei@163.com
 *  \copyright TMTeam
 *  \version 1.0
 *  \History:
 *     Leon 2016/05/20 9:20 Add lock for multi-thread application.\n
 */
///////////////////////////////////////////////////

#pragma once
#include "Queue.h"
#include "SafeData.h"

///////////////////////////////////////////////////
/** \class BlockedQueue :
*  \brief Multi-thread safe queue, only support one thread read or write
*  \note
*    Any type object is unsafe when is being written by more than one threads;\n
*    basic type object is safe in reading, when it is in writting by only one thread;\n
*    but queue is still unsafe in reading, when another thread is writting the queue;\n
*  \author Leon Contact: towanglei@163.com
*  \version 1.0
*  \date 2016/05/03 0:03
*/
template <typename T>
class BlockedQueue :public Queue<T>, public BlockedData
{
public:
	virtual bool Initial(int dataNum = DEFAULTNUM) override
	{
		EnterCriticalSection(&m_section);
			return Queue::Initial(dataNum);
		LeaveCriticalSection(&m_section);
	}

	virtual bool Unitial() override
	{
		EnterCriticalSection(&m_section);
			return Queue::Unitial();
		LeaveCriticalSection(&m_section);
	}

	virtual int SetSize(int dataNum) override
	{
		EnterCriticalSection(&m_section);
			return Queue::SetSize(dataNum);
		LeaveCriticalSection(&m_section);
	}

	virtual int GetSize() override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetSize();
		LeaveCriticalSection(&m_section);
	}

	virtual int GetLength() override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetLength();
		LeaveCriticalSection(&m_section);
	}

	virtual void Clear() override
	{
		EnterCriticalSection(&m_section);
			return Queue::Clear();
		LeaveCriticalSection(&m_section);
	}

	virtual bool IsEmpty() override
	{
		EnterCriticalSection(&m_section);
			return Queue::IsEmpty();
		LeaveCriticalSection(&m_section);
	}

	virtual bool IsFull() override
	{
		EnterCriticalSection(&m_section);
			return Queue::IsFull();
		LeaveCriticalSection(&m_section);
	}

	virtual bool DelHead() override
	{
		EnterCriticalSection(&m_section);
			return Queue::DelHead();
		LeaveCriticalSection(&m_section);
	}

	virtual bool AddTail(const T& data) override
	{
		EnterCriticalSection(&m_section);
			return Queue::AddTail(data);
		LeaveCriticalSection(&m_section);
	}

	virtual bool ForcTail(const T& data) override
	{
		EnterCriticalSection(&m_section);
			return Queue::ForcTail(data);
		LeaveCriticalSection(&m_section);
	}

	virtual bool GetTail(T& data) override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetTail(data);
		LeaveCriticalSection(&m_section);
	}

	virtual T* GetTail() override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetTail();
		LeaveCriticalSection(&m_section);
	}

	virtual bool GetHead(T& data) override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetHead(data);
		LeaveCriticalSection(&m_section);
	}

	virtual T* GetHead() override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetHead();
		LeaveCriticalSection(&m_section);
	}

	virtual bool GetData(T& data, int index) override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetData(data, index);
		LeaveCriticalSection(&m_section);
	}

	virtual T* GetData(int index) override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetData(index);
		LeaveCriticalSection(&m_section);
	}

	virtual bool GetLast(T& data, int index) override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetLast(data, index);
		LeaveCriticalSection(&m_section);
	}

	virtual T* GetLast(int index) override
	{
		EnterCriticalSection(&m_section);
			return Queue::GetLast(index);
		LeaveCriticalSection(&m_section);
	}

};
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class LockedQueue :
*  \brief Multi-thread safe queue, only support one thread read or write
*  \note
*   Lock queue by bool flag "bool isLocked",\n
*   verify isLocked=false before access or make changes.\n
*   Thread is not blocked,which can do nothing or jump to other works.\n
*  \author Leon Contact: towanglei@163.com
*  \version 1.0
*  \date 2016/05/03 0:03
*/
template <typename T>
class LockedQueue :public Queue<T>, public LockedData
{
public:
	virtual bool Initial(int dataNum = DEFAULTNUM) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::Initial(dataNum);
	}

	virtual bool Unitial() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::Unitial();
	}

	virtual int SetSize(int dataNum) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return -1;
		}
		return Queue::SetSize(dataNum);
	}

	virtual int GetSize() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return -1;
		}
		return Queue::GetSize();
	}

	virtual int GetLength() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return -1;
		}
		return Queue::GetLength();
	}

	virtual void Clear() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return;
		}
		return Queue::Clear();
	}

	virtual bool IsEmpty() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::IsEmpty();
	}

	virtual bool IsFull() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::IsFull();
	}

	virtual bool DelHead() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::DelHead();
	}

	virtual bool AddTail(const T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::AddTail(data);
	}

	virtual bool ForcTail(const T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::ForcTail(data);
	}

	virtual bool GetTail(T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::GetTail(data);
	}

	virtual T* GetTail() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return 0;
		}
		return Queue::GetTail();
	}

	virtual bool GetHead(T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::GetHead(data);
	}

	virtual T* GetHead() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return 0;
		}
		return Queue::GetHead();
	}

	virtual bool GetData(T& data, int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::GetData(data, index);
	}

	virtual T* GetData(int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return 0;
		}
		return Queue::GetData(index);
	}

	virtual bool GetLast(T& data, int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::GetLast(data, index);
	}

	virtual T* GetLast(int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		return Queue::GetLast(index);
	}

};
///////////////////////////////////////////////////
