///////////////////////////////////////////////////
/** \file Device.h
 *  \brief Base class of all devices,with it's server.
 *
 *  \note
 *
 ** Status:
 *
 *          json string                             
 *                │
 *                ∨     ┌─────┐
 *      DeSetting(__)─＞│DeSetting │
 *                       └─────┘
 *                           │
 *                           ∨     ┌────┐
 *      Device       Initial(&_)─＞│ Device │─＞ Read() return
 *                                  └────┘              │
 *                                       │                   ∨
 *                                       ∨              ┌────┐
 *      DeviceReader        AttachDevice(*_)             │DeData* │
 *                                                       └────┘
 *                                                            │
 *                                                            ∨
 *                           Task(){ Queue<DeData>::push_back(__) }
 *
 *
 ** Usage:
 *
 *     //File"Camera.h":\n 
 *     \n
 *     #include "Device.h"\n 
 *     \n
 *     CamSetting:public DeSetting\n 
 *     {\n 
 *     public:\n 
 *         CamSetting();\n 
 *         ~CamSetting();\n 
 *         CamSetting(const CamSetting& setting);\n 
 *         CamSetting(const MEGASTR& setting);override;\n 
 *         CamSetting& operator=(const MEGASTR& setting) override;\n 
 *         bool CopyTo(CamSetting& setting);\n 
 *         bool CopyTo(MEGASTR& setting) override;\n
 *         bool CopyFrom(CamSetting& setting);\n
 *         bool CopyFrom(MEGASTR& setting) override;\n
 *         void Clear(MEGASTR& setting) override;\n
 *         static int GetAvailable(SettingSet<MEGASTR>& deSettings);\n 
 *     }\n 
 *     \n 
 *     //template T must has deep copy = operator and be memory continuous\n 
 *     template <typename T = Mat>\n 
 *     Camera:public Device\n
 *     {\n 
 *     public:\n 
 *         Device();\n 
 *         ~Device();\n 
 *         virtual bool Initial(MEGASTR& setting) override;\n 
 *         virtual void Unitial() override;\n 
 *         virtual bool Set(MEGASTR& setting) override;\n 
 *         virtual bool ReSet(MEGASTR& setting) override;\n 
 *         virtual bool Read(DeData<T>& deData, bool update = true) override;\n 
 *         virtual bool Write(DeData<T>& deData, bool force = false) override;\n 
 *     }\n 
 *
 *
 *     //File"main.h":\n
 *     \n
 *     #include "Camera.h"\n
 *     \n
 *     main()
 *     {
 *         CamSetting camSetting(jsonstring);\n
 *         Camera camera;\n
 *         camera.Initial((DeSetting*)&camSetting);\n
 *         DeviceReader deviceReader;\n
 *         deviceReader.AttachDevice((Device*)&camera);\n
 *         //1.Active frame in thread\n
 *         deviceReader.StartServer();\n
 *         ...
 *         deviceReader.StopServer();\n
 *         //2.Or call back frame through Task() function.\n
 *         ...
 *         deviceReader.DetachDevice();\n
 *     }
 * 
 *  \author Leon Contact: towanglei@163.com
 *  \copyright TMTeam
 *  \version 2.0
 *  \History:
 *     Leon 2016/04/14 18:31 Divide DeviceServer into DeviceReader and DeviceWriter.
 *     1.0 : Leon 2016/04/29 10:31 built.\n
 */
///////////////////////////////////////////////////
#pragma once
#include "Thread.h"
#include "CommonDefine.h"
#include "SafeQueue.h"
#include "Err_seq.h"

//////////////////////////////////////////////////
/** \def DE_ERR
 *  \brief Error code of device
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/07 17:58
 */
#ifndef DE_ERR
#define DE_OK        TMT_NORMAL
#define DE_ERR       ERR
#endif
//////////////////////////////////////////////////

//////////////////////////////////////////////////
/** \struct SettingSet
 *  \brief 
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/10 12:38
 */
#ifndef SETTINGSET
#define SETTINGSET
template <typename T>
struct SettingSet
{ 
	enum {SETTINGSETSIZE = 128///< Setting Data max size
	};
	T SettingData[SETTINGSETSIZE];///< Setting data
	int SettingNum = 0; ///< Valid setting data number
}; 
#endif
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class DeSetting
 *  \brief Base struct to store setting para.
 *  \note
 *   Decode para from joson string\n
 *   Read and store by method in derived class.\n
 *  \author Leon Contact: towanglei@163.com
 */
#ifndef TMTV_MEGASTRLEN
#define TMTV_MEGASTRLEN	    4096//字符串最大长度
typedef char	MEGASTR[TMTV_MEGASTRLEN];
#endif
#ifndef DESETTING
#define DESETTING
class DeSetting
{
public:
	SHORTSTR deviceName = "";///< Device name
	PATHSTR devicePath = ""; ///< Device path
	NetIP deviceMAC = "";///< MAC address of Device
	NetIP deviceIP = "";///< IP address of host,not support multi-net
	bool readUpdate = true; ///< If Read(...,bool update) in task
	bool writeForce = false; ///< If Write(...,bool force) in task
	//int serverDataSize = 8; ///< Data list size
	//long serverTimes = -1; ///< Thread run times
	//long serverFrameTime = 0; ///< Thread wait time
	//bool ServerIncludeTaskTime = true; ///< If task time included in wait times

	/// Default constructor get PC name and physical net
	DeSetting();
	/** \fn  ~DeSetting
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~DeSetting();

	/// Deep copy constructor
	DeSetting(const DeSetting& setting);
	/// Deep copy constructor
	DeSetting(const MEGASTR& setting);
	/// Deep copy operator=
	DeSetting& operator=(const DeSetting& setting);
	/// Deep copy operator=
	virtual DeSetting& operator=(const MEGASTR& setting);
	/// Deep copy to para
	bool CopyTo(DeSetting& setting);
	/// Deep copy to para
	virtual bool CopyTo(MEGASTR& setting);
	/// Deep copy from para
	bool CopyFrom(const DeSetting& setting);
	/// Deep copy from para
	virtual bool CopyFrom(const MEGASTR& setting);
	/// Clear para
	virtual void Clear();
	/// Compare operator==
	bool operator== (const DeSetting& deSetting)
	{
		if (deviceMAC[0]==0 || deSetting.deviceMAC[0]==0)
		{
			return false;
		}
		return strcmp(deviceMAC, deSetting.deviceMAC);
	};

	/** \fn  GetAvailable
	 *  \brief Get available device list,just get host setting in base class of DeSetting
	 *  \param[in] NetIP hostIP = "" IP of the machine will be checked 
	 *  \param[out] DeSetting* deSettings, int deNums
	 *  \return static MEGASTR as json format
	 */
	static int GetAvailable(SettingSet<MEGASTR>& deSettings);
};
#endif
///////////////////////////////////////////////////

//////////////////////////////////////////////////
/** \class DeData
*  \brief Base class to store template IO data, template must be memory continues
*  \note
*   Inlined methods for better performents;\n
*   Implemente copy constructor and = operator;\n
*   Recode create time in, use GetLife() to avoid dead data;\n
*   Use dataTimeStr in compare of operator ==, to avoid duplicate data.\n
*  \author Leon Contact: towanglei@163.com
*  \version 1.0
*  \date 2016/05/02 11:38
*/
template <typename T>
class DeData
{
public:
	T data;///< Data body, must be memory continues
	SYSTEMTIME createTime;
	SYSTEMTIME updateTime;
	TINYSTR dataTimeStr = "2000-01-01 00:00:00:00";///< Data time.
	int proceessTag = 0;///< Tag of Data processed.
	
	/// Default constructor
	DeData();
	/** \fn  ~DeData
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~DeData();

	/// Deep copy constructor
	DeData(const DeData<T>& dataIn);
	/// Deep copy operator=
	DeData<T>& operator=(const DeData<T>& dataIn);
	/// Deep copy to para
	virtual bool CopyTo(DeData<T>& dataIn);
	/// Deep copy from para
	virtual bool CopyFrom(const DeData<T>& dataIn);

	/** \fn  operator==
	*  \brief Compare operator==, to avoid duplicate data, must override
	*  \param[in] DeData& dataIn = (DeData&)dataObject
	*  \return bool
	*/
	virtual bool operator== (const DeData& dataIn);

	/// Get life of the object, to avoid dead data
	double GetLife();
};
///////////////////////////////////////////////////

//////////////////////////////////////////////////
/** \struct DeviceCallbackFun
 *  \brief Callback Function called by Device
 *  \author Leon Contact: towanglei@163.com
 */
#ifndef DEVICECALLBACKFUN
#define DEVICECALLBACKFUN
typedef void(__stdcall* DeviceCallbackFun)(void* pPara);
#endif
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class Device 
 *  \brief Base class of all devices.
 *  \note
 *   Inlined methods for better performents;\n
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/04/29 10:34
 */
template <typename T>
class Device
{
public:
    /// Handle of device
	HANDLE m_hDevice = INVALID_HANDLE_VALUE;
	 /** \enum Status
	 *  \brief Status of device.
	 *  \author Leon Contact: towanglei@163.com
	 */
	enum DESTATUS 
	{
		DE_INVALID = 0, ///< Invalid device e.g. not exist
		DE_UNITIALED, ///< Available device but not initialed 
		DE_INITIALED ///< Initialed device
	};
	/// Status of device
	DESTATUS m_DeStatus;
	/// Setting of device
	DeSetting* p_DeSetting = 0;
	/// Data of device
	DeData m_DeData;

private:
	DeviceCallbackFun callBackFun = 0;
	void* callPara = 0;

public:
	/// Default constructor
	Device();
	/** \fn  ~Device
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~Device();
	/** \fn  Initial
	*  \brief Initial device
	*  \param[in] DeSetting* pSetting = new DeSetting(MEGASTR& setting)
	*  \return bool
	*/
	virtual bool Initial(MEGASTR& setting);
	/** \fn  Unitial
	 *  \brief Unitial device
	 */
	virtual void Unitial();
	/** \fn  Set
	 *  \brief Set device para without stop
	 *  \param[in] DeSetting* pSetting = new DeSetting(MEGASTR& setting)
	 *  \return bool
	 */
	virtual bool Set(MEGASTR& setting);
	/** \fn  ReSet
	 *  \brief ReSet device, stop and restart
	 *  \param[in] DeSetting* pSetting = new DeSetting(MEGASTR& setting)
	 *  \return bool
	 */
	virtual bool ReSet(MEGASTR& setting);

	/** \fn  Read
	*  \brief Read form buffer of device, or process deData and return.
	*  \note
	*    If update buffer from physical device before read.\n
	*    DeData<T>& deData is deep copied from buffer/physical device.\n
	*  \param[out/in] DeData<T>& deData, T must be memory continues
	*  \param[in] bool update if Read then update buffer from physical device.
	*  \return False if failed.
	*/
	virtual bool Read(DeData<T>& deData, bool update = true);
	/** \fn  operator>>
	*  \brief Read form buffer of device, or process deData and return.
	*  \note
	*    If update buffer from physical device before read.\n
	*    DeData<T>& deData is deep copied from buffer/physical device.\n
	*  \param[out/in] DeData<T>& deData, T must be memory continues
	*  \param[in] bool update if Read then update buffer from physical device.
	*  \return False if failed.
	*/
	virtual bool operator>>(DeData<T>& deData);
	/** \fn  Write
	*  \brief Write data into buffer of device,if force push buffer into physical device after write.
	*  \note
	*    DeData<T>& deData is deep copied into buffer/physical device.\n
	*  \param[in] DeData<T>& deData, T must be memory continues
	*  \param[in] bool force if force to write physical device.
	*  \return False if failed.
	*/
	virtual bool Write(const DeData<T>& deData, bool force = false);
	/** \fn  operator<<
	*  \brief Write data into buffer of device,if force push buffer into physical device after write.
	*  \note
	*    DeData<T>& deData is deep copied into buffer/physical device.\n
	*  \param[in] DeData<T>& deData, T must be memory continues
	*  \param[in] bool force if force to write physical device.
	*  \return False if failed.
	*/
	virtual bool operator<<(const DeData<T>& deData);
};
///////////////////////////////////////////////////


///////////////////////////////////////////////////
/** \class ServerSetting
*  \brief class to store Server setting para.
*  \note
*   Decode para from joson string\n
*  \author Leon Contact: towanglei@163.com
*/
#ifndef TMTV_MEGASTRLEN
#define TMTV_MEGASTRLEN	    4096//字符串最大长度
typedef char	MEGASTR[TMTV_MEGASTRLEN];
#endif
#ifndef SEVERSETTING
#define SEVERSETTING
class ServerSetting:public DeSetting
{
public:
	int serverDataSize = 8; ///< Data list size
	long serverTimes = -1; ///< Thread run times
	long serverFrameTime = 0; ///< Thread wait time
	bool serverQueueBlock = false; ///< If Thread block until queue released by other object
	//bool serverReadUpdate = true; ///< If Read(...,bool update) in task
	//bool serverWriteForce = false; ///< If Write(...,bool force) in task
	//bool serverForceQueue = true; ///< If ForceTail() when Queue is full
	//bool ServerIncludeTaskTime = true; ///< If task time included in wait times

	/// Default constructor
	ServerSetting();
	/** \fn  ~DeSetting
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~ServerSetting();

	/// Deep copy constructor
	ServerSetting(const ServerSetting& setting);
	/// Deep copy constructor
	ServerSetting(const MEGASTR& setting);
	/// Deep copy operator=
	ServerSetting& operator=(const ServerSetting& setting);
	/// Deep copy operator=
	virtual ServerSetting& operator=(const MEGASTR& setting);
	/// Deep copy to para
	bool CopyTo(ServerSetting& setting);
	/// Deep copy to para
	virtual bool CopyTo(MEGASTR& setting);
	/// Deep copy from para
	bool CopyFrom(const ServerSetting& setting);
	/// Deep copy from para
	virtual bool CopyFrom(const MEGASTR& setting);
};
#endif
///////////////////////////////////////////////////


///////////////////////////////////////////////////
/** \class DeviceServer : public Thread, include Device
 *  \brief Base server class of all devices.
 *  \note
 *   Start server to run the added device.\n
 *   The task can pop_back results from deque and write into device.\n
 *   The task can read the device, and push_front results in deque.\n
 *   DeviceServer is also a valid device, can Read()/Write() from/into deque.\n
 *   See DeviceServer A as a device, B.AttachDevice((Device*)&A) can connect A with B.\n
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/04/29 23:09
 */
template <typename T>
class DeviceServer : public Thread , public Device<T>
{
public:// Just for other DeviceServer use
	/** \fn  Read
	*  \brief Read (GetHead) form Queue, or process deData and return.
	*  \note
	*    if update then DelHead after read.\n
	*    DeData<T>& deData is deep copied from Queue item.\n
	*  \param[out/in] DeData<T>& deData, T must has = operation
	*  \param[in] bool update if DelHead after read.
	*  \return False if failed.
	*/
	virtual bool Read(DeData<T>& deData, bool update = true);
	/** \fn  Write
	*  \brief Write (ForceTail/AddTail) into Queue,if force use ForceTail().
	*  \note
	*    DeData<T>& deData is deep copied into Queue item.\n
	*  \param[in] DeData<T>& deData, T must has = operation
	*  \param[in] bool force if force to write physical device.
	*  \return False if failed.
	*/
	virtual bool Write(const DeData<T>& deData, bool force = false);

public:
	Device<T>* p_Device; ///< Device object pointer
	SafeQueue <DeData<T>> m_DataList; ///< Data list deque push front pop back, iterate front to back

	/// Actual frame time(ms), decided by driver or server, -1 means unused
	double GetFrameTime()
	{
		return MAX((double)m_waitTime, m_taskTime);
	}
	/// Actual sample time(ms), decided by driver or server, -1 means unused
	double GetSampleTime()
	{
		return m_taskTime;
	}
	/// Actual sample number
	long GetSampleNum()
	{
		return m_DataList.size();
	}

	/// Set expected frame time(ms)
	void SetFrameTime(long frameTime)
	{
		m_waitTime = frameTime;
	}
	/// Set expected sample time(ms)
	virtual void SetSampleTime(long sampleTime) {}

public:
	/// Default constructor
	DeviceServer(HANDLE  hParent = 0);
	/// Default destructor
	~DeviceServer();

	/** \fn  Initial
	*  \brief Initial DeviceServer
	*  \param[in] DeSetting* pSetting = new ServerSetting(MEGASTR& setting)
	*  \return bool
	*/
	virtual bool Initial(MEGASTR& setting);
	/** \fn  Unitial
	*  \brief Unitial DeviceServer
	*/
	virtual void Unitial();
	/** \fn  Set
	*  \brief Set running server
	*  \param[in] DeSetting* pSetting = (DeSetting*)&setting;
	*  \return bool
	*/
	virtual bool Set(MEGASTR& setting);
	/** \fn  ReSet
	*  \brief Stop and reset server,need StartServer()
	*  \param[in] DeSetting* pSetting = (DeSetting*)&setting;
	*  \return bool
	*/
	virtual bool ReSet(MEGASTR& setting);
	/** \fn  AttachDevice
	*  \brief Add device to server
	*  \param[in] Device* pDevice = (Device*)&deviceObj;
	*  \return bool
	*/
	virtual bool AttachDevice(Device<T>* pDevice);
	/** \fn  DetachDevice
	*  \brief Delete device from server,
	*         but device object is not delete in function
	*  \return void
	*/
	virtual void DetachDevice();
	/** \fn  SetDevice
	*  \brief Set device in running server
	*  \param[in] DeSetting* pSetting = (DeSetting*)&setting;
	*  \return bool
	*/
	virtual bool SetDevice(MEGASTR& setting);
	/** \fn  ReSetDevice
	*  \brief Stop and reset device in server,need StartServer()
	*  \param[in] DeSetting* pSetting = (DeSetting*)&setting;
	*  \return bool
	*/
	virtual bool ReSetDevice(MEGASTR& setting);
	/** \fn  StartServer
	*  \brief Start server to run the added device
	*  \return bool
	*/
	virtual bool StartServer();
	/** \fn  StopServer
	*  \brief Stop server without delete anything
	*  \return bool
	*/
	virtual bool StopServer();

protected:
	/** \fn  Task
	*  \brief The task running the device
	*  \return
	*/
	virtual void Task() = 0;
};
///////////////////////////////////////////////////


///////////////////////////////////////////////////
/** \class DeviceReader : public DeviceServer
 *  \brief Base read server class of all devices.
 *  \note
 *   The task read the device, and push_front results in deque.\n
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/04/29 23:09
 */
template <typename T>
class DeviceReader : public DeviceServer<T>
{
protected:
	/** \fn  Task
	 *  \brief The task read the device, and push_front results in deque.
	 *  \return 
	 */
	virtual void Task();
};
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class DeviceWriter : public DeviceServer
*  \brief Base write server class of all devices.
*  \note
*   The task pop_back results from deque and write into device.\n
*  \author Leon Contact: towanglei@163.com
*  \version 1.0
*  \date 2016/04/29 23:09
*/
template <typename T>
class DeviceWriter : public DeviceServer<T>
{
protected:
	/** \fn  Task
	*  \brief The task pop_back results from deque and write into device.
	*  \return
	*/
	virtual void Task();
};
///////////////////////////////////////////////////