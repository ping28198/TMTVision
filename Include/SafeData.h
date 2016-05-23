///////////////////////////////////////////////////
/** \file SafeData.h
 *  \brief Multi-thread safe data, three types thread lock
 *  \note
 *
 *    BlockedData block threads when data is already accessed;\n
 *    LockedData do not block thread,verify IsLocked() before access or make changes;\n
 *    SafeData use status logic for read/write protection.\n
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
#include <windows.h>
#include <exception>
using namespace std;

///////////////////////////////////////////////////
/** \class BlockedData :
*  \brief Use Multi-thread blocker CRITICAL_SECTION for safe protection.
*  \note
*
*   Call Lock() to EnterCriticalSection before access and DeLock() to LeaveCriticalSection after.\n
*   Any Thread is blocked until the running Thread call DeLock() to LeaveCriticalSection.\n
*
*  \author Leon Contact: towanglei@163.com
*  \version 1.0
*  \date 2016/05/20 10:08
*/
class BlockedData
{
protected:
	// Multi-thread protection blocker
	CRITICAL_SECTION m_section;
	/// Call Lock() before access
	bool Lock()
	{
		try
		{
			::EnterCriticalSection(&m_section);
		}
		catch (exception& e)
		{
			return false;
		}
		return true;
	}
	/// Call DeLock() after access
	bool DeLock()
	{
		try
		{
			::LeaveCriticalSection(&m_section);
		}
		catch (exception& e)
		{
			return false;
		}
		return true;
	}
public:
	BlockedData()
	{
		::InitializeCriticalSection(&m_section);
	}
	~BlockedData()
	{
		::DeleteCriticalSection(&m_section);
	}
	//...something else operation
};
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class LockedData : 
 *  \brief Lock data by bool flag "bool isLocked",do not block thread.
 *  \note
 *
 *   Lock data by bool flag "bool isLocked";\n 
 *   Call Lock() to assign isLocked=true before access and DeLock() after;\n
 *   Verify IsLocked() before access or make changes;\n
 *   Thread is not blocked, which can do nothing or jump to other works.\n
 *
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/20 10:08
 */
class LockedData :public BlockedData
{
protected:
	// Multi-thread protection locker
	bool isLocked = false;
	/// Call Lock() before access
	bool Lock()
	{
		try
		{
			::EnterCriticalSection(&m_section);
			isLocked = true;
			::LeaveCriticalSection(&m_section);
		}
		catch (exception& e)
		{
			return false;
		}
		return true;
	}
	/// Call DeLock() after access
	bool DeLock()
	{
		try
		{
			::EnterCriticalSection(&m_section);
			isLocked = false;
			::LeaveCriticalSection(&m_section);
		}
		catch (exception& e)
		{
			return false;
		}
		return true;
	}
public:
	bool IsLocked()
	{
		return isLocked;
	}
	//...something else operation
};
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class SafeData :
*  \brief Use status logic for read/write protection.
*  \note
*
*    If directly call EnterRead()/EnterWrite(),thread is blocked in these functong,\n
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
class SafeData :public BlockedData
{
protected:
	///// Current user thread handle
	//HANDLE m_hUser = INVALID_HANDLE_VALUE;
	/// DataStatus enum
	enum{
		DATA_NOACCESS = 0, DATA_READING = 1, DATA_WRITING = 2,
	};
	/// Status of Data
	int dataStatus = DATA_NOACCESS;
	/// Status logic table
	const bool AccessTable[2][3]
		= { {true,true,false},
			{true,false,false} };
	/// Status transfer table
	const bool TransferTable[3][3]
		= { { true,true,true },
		    { true,true,false },
		    { true,false,true } };

public:
	/// Call EnterRead() before read
	bool EnterRead(/*HANDLE hUser*/)
	{
		if (!TransferTable[1][dataStatus]) return false;
		::EnterCriticalSection(&m_section);
		dataStatus = DATA_READING;
		::LeaveCriticalSection(&m_section);
		return true;
	}
	/// Call LeaveRead() after read
	bool LeaveRead(/*HANDLE hUser*/)
	{
		if (!TransferTable[0][dataStatus]) return false;
		::EnterCriticalSection(&m_section);
		dataStatus = DATA_NOACCESS;
		::LeaveCriticalSection(&m_section);
		return true;
	}
	/// Call EnterWrite() before write
	bool EnterWrite(/*HANDLE hUser*/)
	{
		if (!TransferTable[2][dataStatus]) return false;
		::EnterCriticalSection(&m_section);
		dataStatus = DATA_WRITING;
		::LeaveCriticalSection(&m_section);
		return true;
	}
	/// Call LeaveWrite() after write
	bool LeaveWrite()
	{
		if (!TransferTable[0][dataStatus]) return false;
		::EnterCriticalSection(&m_section);
		dataStatus = DATA_WRITING;
		::LeaveCriticalSection(&m_section);
		return true;
	}

public:
	bool Readable(/*HANDLE hUser*/)
	{
		return AccessTable[0][dataStatus];
	}
	bool Writeable(/*HANDLE hUser*/)
	{
		return AccessTable[1][dataStatus];
	}
	//...something else operation
};
///////////////////////////////////////////////////