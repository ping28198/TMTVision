///<header_info>
//==============================================================================
#pragma once
#include "TmtSocket.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//����ͨѶSocket������
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
