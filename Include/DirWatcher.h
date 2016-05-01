///<proj_info>
//==============================================================================
// 项目名 : 智能监控
// 文件名 : DirWatcher.h
// 作  者 : 王磊
// 用  途 : DirWatcher文件夹监控工具类,实现同步监控
//          DirWatchServer文件夹监控线程类,实现异步监控
// 版  权 : 霍比特人
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     王磊        2016.3.20  整合旧版本
//2.0     王磊        2016.3.29  ReadDirectoryChangesW监控二级文件夹,并返回
//                               最新文件信息,采用同步方式通知文件夹变化
//2.1     王磊        2016.4.3   拆分为DirWatcher工具类和DirWatchServer线程类
//                               分别实现同步监控和异步监控
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
//添加所需的头文件
#pragma once
#include "Thread.h"
#include "CommonDefine.h"
#include "Queue.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//功能描述:文件夹监控类, 同步监控
//         监视文件夹改变, 给出改变内容索引
class DirWatcher
{
public:
	//监控的路径
	PATHWSTR m_path;
	//监控的路径句柄
	HANDLE m_hDir;
	DWORD m_action;
	enum { NOTIFYSIZE=2048 };
	//DWORD m_notify[NOTIFYSIZE];
	DirWatcher()
	{
		m_hDir = 0;
		//初始化文件名队列
		m_path[0]=0;
		m_action = 0x00000000;
	}
	~DirWatcher()
	{
		FreePath();
	}
	//创建文件夹读取句柄, 初始化ReadDirectoryChangesW相关参数
	//#define FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001   
	//#define FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002   
	//#define FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004   
	//#define FILE_NOTIFY_CHANGE_SIZE         0x00000008   
	//#define FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010   
	//#define FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020   
	//#define FILE_NOTIFY_CHANGE_CREATION     0x00000040   
	//#define FILE_NOTIFY_CHANGE_SECURITY     0x00000100   
	//#define FILE_ACTION_ADDED                   0x00000001   
	//#define FILE_ACTION_REMOVED                 0x00000002   
	//#define FILE_ACTION_MODIFIED                0x00000003   
	//#define FILE_ACTION_RENAMED_OLD_NAME        0x00000004   
	//#define FILE_ACTION_RENAMED_NEW_NAME        0x00000005 
	virtual bool RegPath(PATHWSTR path,DWORD action= FILE_NOTIFY_CHANGE_LAST_WRITE);
	//执行前强制停止线程, 停止监控文件夹
	virtual void FreePath();
	//调用ReadDirectoryChangesW用于等待监控消息
	bool Watch(PATHWSTR& fileName, TINYWSTR& fileTime,DWORD& action);
//
//public:
//	//调试函数,显示对象信息
//	//method=0 xml 完整格式输出, 带换行
//	//1 最短格式输出,只输出队列数量
//	//2 更新格式输出,输出队列最新元素
//	//字背景颜色范围: 40--49        字颜色: 30--39
//	//	40 : 黑                           30 : 黑
//	//	41 : 红                           31 : 红
//	//	42 : 绿                           32 : 绿
//	//	43 : 黄                           33 : 黄
//	//	44 : 蓝                           34 : 蓝
//	//	45 : 紫                           35 : 紫
//	//	46 : 深绿                         36 : 深绿
//	//	47 : 白色                         37 : 白色
//	void ToString(MEGAWSTR &string, int method = 0, int color = 32);
};
//==============================================================================
///</class_info>

///<algorithm_info>
//==============================================================================
//功能描述: 文件夹监控线程类, 异步监控(线程内同步,线程外异步):
// 
//         Store as an array
//        ┏━━━━━━━┓
//   ┏━▶┃  FileItem × ┃◀━p_DataList
//   ┃   ┃  FileItem × ┃
//   ┃   ┃  FileItem × ┃             Parent somewhere ◀━━━┓
//  Index ┃  FileItem 0  ┃◀━ p_Head┏━DelHead()              ┃
//  as a  ┃  FileItem 1  ┃       ◀━┛     ┗━━━━━━━━━┛
//  ring  ┃       :      ┃
//   ┃   ┃       :      ┃ 
//   ┃   ┃       :      ┃              DirWatchServer::Task()◀┓    
//   ┃   ┃  FileItem N  ┃◀━ p_Tail┏━ ForcTail()            ┃
//   ┃   ┃  FileItem × ┃       ◀━┛     ┗━━━━━━━━━┛
//   ┗━ ┃  FileItem × ┃
//        ┗━━━━━━━┛
//
//==============================================================================
///</algorithm_info>

///<datastruct_info>
//==============================================================================
#ifndef FILEITEM
#define  FILEITEM
struct FileItem
{
	PATHWSTR m_fileName;
	TINYWSTR m_fileTime;
	DWORD m_fileAction;
	bool m_fileProcessed;
};
#endif
//==============================================================================
///</datastruct_info>

///<class_info>
//==============================================================================
//功能描述:文件夹监控线程类, 异步监控(线程内同步,线程外异步), 
//         通过队列才线程内外交换数据, 
//         线程内不断将最新变动的文件强制推入循环队列尾, 保持文件队列最新, 
//         线程外(父线程)不断处理队列头文件, 处理完删除队列头.
class DirWatchServer :
	public Thread, public DirWatcher
{
public:
	//文件名队列, 用于保存最近的历史文件, 
    //处理线程将从头部读取执行释放
	//监控线程将从尾部添加
	enum { QUEUESIZE = 64 };
	Queue<FileItem> m_fileNameQueue;
	void *p_Parent;//父类指针
	DirWatchServer(void *pParent,HANDLE  hParent = 0):p_Parent(pParent)
	{
		//初始化文件名队列
		m_fileNameQueue.Initial(QUEUESIZE);
	}
	~DirWatchServer()
	{
		//卸载文件名队列
		m_fileNameQueue.Unitial();
	}
	//创建文件夹读取句柄, 初始化ReadDirectoryChangesW相关参数
	//执行前强制停止线程, 需要用Create()再次启动,具有线程保护
	bool RegPath(PATHWSTR path, DWORD action = FILE_NOTIFY_CHANGE_LAST_WRITE);
	//执行前强制停止线程, 停止监控文件夹,具有线程保护
	void FreePath();
	//先扫描注册文件夹所有文件, 文件名推入队列中保留最近的文件
	//再重载启动函数,内部添加FindFirstChangeNotification用于启动监控
	void Create(int times = -1, long waiteTime = 0, bool includeTaskTime = true);
	//重载启动函数
	void Destroy();
	//供重载的操作函数,主函数会重复执行该函数,内部调用Watch()函数,具有线程保护
	void Task();
//
//public:
//	//调试函数,显示对象信息
//	//method=0 xml 完整格式输出, 带换行
//	//1 最短格式输出,只输出队列数量
//	//2 更新格式输出,输出队列最新元素
//	//字背景颜色范围: 40--49        字颜色: 30--39
//	//	40 : 黑                           30 : 黑
//	//	41 : 红                           31 : 红
//	//	42 : 绿                           32 : 绿
//	//	43 : 黄                           33 : 黄
//	//	44 : 蓝                           34 : 蓝
//	//	45 : 紫                           35 : 紫
//	//	46 : 深绿                         36 : 深绿
//	//	47 : 白色                         37 : 白色
//	void ToString(MEGAWSTR &string,int method=0,int color= 32);
};
//==============================================================================
///</class_info>
