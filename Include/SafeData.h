///////////////////////////////////////////////////
/** \file SafeData.h
*  \brief Multi-thread safe data, two types thread lock
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
#include <windows.h>

///////////////////////////////////////////////////
/** \class BlockedData :
*  \brief Use CRITICAL_SECTION for multi thread safe.
*  \note
*   User operation should EnterCriticalSection before access or make changes,
*   and LeaveCriticalSection after access.
*   Thread is blocked until another Thread call LeaveCriticalSection.\n
*  \author Leon Contact: towanglei@163.com
*  \version 1.0
*  \date 2016/05/20 10:08
*/
///////////////////////////////////////////////////
class BlockedData
{
protected:
	// Multi-thread protection lock
	CRITICAL_SECTION m_section;
public:
	BlockedData()
	{
		::InitializeCriticalSection(&m_section);
	}
	~BlockedData()
	{
		::DeleteCriticalSection(&m_section);
	}
	//...something else operation should EnterCriticalSection before access or make changes,
	//and LeaveCriticalSection after access.Thread is blocked until LeaveCriticalSection is called.
};

///////////////////////////////////////////////////
/** \class LockedData : 
 *  \brief Lock data by bool flag "bool isLocked",do not block thread.
 *  \note
 *   Lock data by bool flag "bool isLocked",\n 
 *   verify isLocked=false before access or make changes.\n
 *   Thread is not blocked,which can do nothing or jump to other works.\n
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/20 10:08
 */
///////////////////////////////////////////////////
class LockedData :public BlockedData
{
protected:
	bool isLocked = false;
	void Lock()
	{
		::EnterCriticalSection(&m_section);
		isLocked = true;
		::LeaveCriticalSection(&m_section);
	}
	void DeLock()
	{
		::EnterCriticalSection(&m_section);
		isLocked = false;
		::LeaveCriticalSection(&m_section);
	}
public:
	bool IsLocked()
	{
		return isLocked;
	}
	//...something else operation should verify isLocked=false before access or make changes.
};

