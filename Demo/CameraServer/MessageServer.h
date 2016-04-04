///<header_info>
//==============================================================================
#pragma once
#include "TmtSocket.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//网络通讯Socket方法类
class MessageServer :public TmtSocketServer
{
public:
	MessageServer();
	~MessageServer();
public:
	void ServerProcess(int revLen);
};
//==============================================================================
///</class_info>
