///////////////////////////////////////////////////
/** \file SafeQueue.h
 *  \brief Multi-thread safe queue, only support one thread read or write
 *  \note
 *
 *    BlockedQueue block threads when data is already accessed;\n
 *    LockedQueue do not block thread,verify IsLocked() before access or make changes;\n
 *    SafeQueue use status logic for read/write protection.\n
 *
 *  \author Leon Contact: towanglei@163.com
 *  \copyright TMTeam
 *  \version 1.1beta
 *  \History:
 *     Leon 2016/05/20 9:20 Add read/write logic for multi-thread application.\n
 *     1.0 Leon 2016/05/20 9:20 Add lock for multi-thread application.\n
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
		Lock();
		bool rtVal = Queue::Initial(dataNum);
		DeLock();
		return rtVal;
	}

	virtual bool Unitial() override
	{
		Lock();
		bool rtVal = Queue::Unitial();
		DeLock();
		return rtVal;
	}

	virtual int SetSize(int dataNum) override
	{
		Lock();
		int rtVal = Queue::SetSize(dataNum);
		DeLock();
		return rtVal;
	}

	virtual int GetSize() override
	{
		Lock();
		int rtVal = Queue<T>::GetSize();
		DeLock();
		return rtVal;
	}

	virtual int GetLength() override
	{
		Lock();
		int rtVal = Queue<T>::GetLength();
		DeLock();
		return rtVal;
	}

	virtual void Clear() override
	{
		Lock();
		Queue<T>::Clear();
		DeLock();
	}

	virtual bool IsEmpty() override
	{
		Lock();
		bool rtVal = Queue<T>::IsEmpty();
		DeLock();
		return rtVal;
	}

	virtual bool IsFull() override
	{
		Lock();
		bool rtVal = Queue<T>::IsFull();
		DeLock();
		return rtVal;
	}

	virtual bool DelHead() override
	{
		Lock();
		bool rtVal = Queue<T>::DelHead();
		DeLock();
		return rtVal;
	}

	virtual bool AddTail(const T& data) override
	{
		Lock();
		bool rtVal = Queue<T>::AddTail(data);
		DeLock();
		return rtVal;
	}

	virtual bool ForcTail(const T& data) override
	{
		Lock();
		bool rtVal = Queue<T>::ForcTail(data);
		DeLock();
		return rtVal;
	}

	virtual bool GetTail(T& data) override
	{
		Lock();
		bool rtVal = Queue<T>::GetTail(data);
		DeLock();
		return rtVal;
	}

	virtual T* GetTail() override
	{
		Lock();
		T* rtVal = Queue<T>::GetTail();
		DeLock();
		return rtVal;
	}

	virtual bool GetHead(T& data) override
	{
		Lock();
		bool rtVal = Queue<T>::GetHead(data);
		DeLock();
		return rtVal;
	}

	virtual T* GetHead() override
	{
		Lock();
		T* rtVal = Queue<T>::GetHead();
		DeLock();
		return rtVal;
	}

	virtual bool GetData(T& data, int index) override
	{
		Lock();
		bool rtVal = Queue<T>::GetData(data, index);
		DeLock();
		return rtVal;
	}

	virtual T* GetData(int index) override
	{
		Lock();
		T* rtVal = Queue<T>::GetData(index);
		DeLock();
		return rtVal;
	}

	virtual bool GetLast(T& data, int index) override
	{
		Lock();
		bool rtVal = Queue<T>::GetLast(data, index);
		DeLock();
		return rtVal;
	}

	virtual T* GetLast(int index) override
	{
		Lock();
		T* rtVal = Queue<T>::GetLast(index);
		DeLock();
		return rtVal;
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
		Lock();
		bool rtVal = Queue<T>::Initial(dataNum);
		DeLock();
		return rtVal;
	}

	virtual bool Unitial() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		Lock();
		bool rtVal = Queue<T>::Unitial();
		DeLock();
		return rtVal;
	}

	virtual int SetSize(int dataNum) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return -1;
		}
        Lock();
        int rtVal = Queue<T>::SetSize(dataNum);
        DeLock();
        return rtVal;
	}

	virtual int GetSize() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return -1;
		}
        Lock();
        int rtVal = Queue<T>::GetSize();
        DeLock();
        return rtVal;
	}

	virtual int GetLength() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return -1;
		}
        Lock();
        int rtVal = Queue<T>::GetLength();
        DeLock();
        return rtVal;
	}

	virtual void Clear() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return;
		}
        Lock();
        Queue<T>::Clear();
        DeLock();
	}

	virtual bool IsEmpty() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		Lock();
		bool rtVal = Queue<T>::IsEmpty();
		DeLock();
		return rtVal;
	}

	virtual bool IsFull() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
        Lock();
        bool rtVal = Queue<T>::IsFull();
        DeLock();
        return rtVal;
	}

	virtual bool DelHead() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
        Lock();
        bool rtVal = Queue<T>::DelHead();
        DeLock();
        return rtVal;
	}

	virtual bool AddTail(const T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
        Lock();
        bool rtVal = Queue<T>::AddTail(data);
        DeLock();
        return rtVal;
	}

	virtual bool ForcTail(const T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
        Lock();
        bool rtVal = Queue<T>::ForcTail(data);
        DeLock();
        return rtVal;
	}

	virtual bool GetTail(T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
        Lock();
        bool rtVal = Queue<T>::GetTail(data);
        DeLock();
        return rtVal;
	}

	virtual T* GetTail() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return 0;
		}
        Lock();
        bool T* = Queue<T>::GetTail();
        DeLock();
        return rtVal;
	}

	virtual bool GetHead(T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		Lock();
		bool rtVal = Queue<T>::GetHead(data);
		DeLock();
		return rtVal;
	}

	virtual T* GetHead() override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return 0;
		}
        Lock();
        T* rtVal = Queue<T>::GetHead();
        DeLock();
        return rtVal;
	}

	virtual bool GetData(T& data, int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		Lock();
		bool rtVal = Queue<T>::GetData(data, index);
		DeLock();
		return rtVal;
	}

	virtual T* GetData(int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return 0;
		}
        Lock();
        T* rtVal = Queue<T>::GetData(index);
        DeLock();
        return rtVal;		
	}

	virtual bool GetLast(T& data, int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
        Lock();
        bool rtVal = Queue<T>::GetLast(data, index);
        DeLock();
        return rtVal;
	}

	virtual T* GetLast(int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (locked)
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
        Lock();
		T* rtVal = Queue<T>::GetLast(index);
        DeLock();
        return rtVal;
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
*////////////////////////////////////////////////////
/** \class SafeQueue :
*  \brief Use status logic for read/write protection.
*  \note
*
*    If directly call EnterRead()/EnterWrite(),thread is blocked in these function,\n
*     If call Readable()/Writeable() before Enter*() to vertify accessable,\n
*     thread is not blocked and could jump to do something else.\n
*
** Status:
*
*    Accessible in:  Do nothing  Reading  Writing
*      To read           √        √       ×
*      To write          √        ×       ×
*
*    Transferable from: Do nothing  Reading  Writing
*      To do nothing        -        √       √
*      To reading          √         -       ×
*      To writing          √        ×        -
*
*  \author Leon Contact: towanglei@163.com
*  \version 1.0
*  \date 2016/05/20 10:08
*/
template <typename T>
class SafeQueue :public Queue<T>, public SafeData
{
public:
	virtual bool Initial(int dataNum = DEFAULTNUM) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Writeable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterWrite();
		bool rtVal = Queue<T>::Initial(dataNum);
		LeaveWrite();
		return rtVal;
	}

	virtual bool Unitial() override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Writeable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterWrite();
		bool rtVal = Queue<T>::Unitial();
		LeaveWrite();
		return rtVal;
	}

	virtual int SetSize(int dataNum) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Writeable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return -1;
		}
		EnterWrite();
		int rtVal = Queue<T>::SetSize(dataNum);
		LeaveWrite();
		return rtVal;
	}

	virtual int GetSize() override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return -1;
		}
		EnterRead();
		int rtVal = Queue<T>::GetSize();
		LeaveRead();
		return rtVal;
	}

	virtual int GetLength() override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return -1;
		}
		EnterRead();
		int rtVal = Queue<T>::GetLength();
		LeaveRead();
		return rtVal;
	}

	virtual void Clear() override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Writeable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return;
		}
		EnterRead();
		Queue<T>::Clear();
		LeaveWrite();
	}

	virtual bool IsEmpty() override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterRead();
		bool rtVal = Queue<T>::IsEmpty();
		LeaveRead();
		return rtVal;
	}

	virtual bool IsFull() override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterRead();
		bool rtVal = Queue<T>::IsFull();
		LeaveRead();
		return rtVal;
	}

	virtual bool DelHead() override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Writeable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterWrite();
		bool rtVal = Queue<T>::DelHead();
		LeaveWrite();
		return rtVal;
	}

	virtual bool AddTail(const T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Writeable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterWrite();
		bool rtVal = Queue<T>::AddTail(data);
		LeaveWrite();
		return rtVal;
	}

	virtual bool ForcTail(const T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Writeable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterWrite();
		bool rtVal = Queue<T>::ForcTail(data);
		LeaveWrite();
		return rtVal;
	}

	virtual bool GetTail(T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterRead();
		bool rtVal = Queue<T>::GetTail(data);
		LeaveRead();
		return rtVal;
	}

	virtual T* GetTail() override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return 0;
		}
		EnterRead();
		bool T* = Queue<T>::GetTail();
		LeaveRead();
		return rtVal;
	}

	virtual bool GetHead(T& data) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterRead();
		bool rtVal = Queue<T>::GetHead(data);
		LeaveRead();
		return rtVal;
	}

	virtual T* GetHead() override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return 0;
		}
		EnterRead();
		T* rtVal = Queue<T>::GetHead();
		LeaveRead();
		return rtVal;
	}

	virtual bool GetData(T& data, int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterRead();
		bool rtVal = Queue<T>::GetData(data, index);
		LeaveRead();
		return rtVal;
	}

	virtual T* GetData(int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return 0;
		}
		EnterRead();
		T* rtVal = Queue<T>::GetData(index);
		LeaveRead();
		return rtVal;
	}

	virtual bool GetLast(T& data, int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterRead();
		bool rtVal = Queue<T>::GetLast(data, index);
		LeaveRead();
		return rtVal;
	}

	virtual T* GetLast(int index) override
	{
		m_ErrCode = QUE_NORMAL;
		if (!Readable())
		{
			m_ErrCode = QUE_ERR_LOCKED;
			return false;
		}
		EnterRead();
		T* rtVal = Queue<T>::GetLast(index);
		LeaveRead();
		return rtVal;
	}

};
///////////////////////////////////////////////////
