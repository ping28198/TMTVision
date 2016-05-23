
#pragma once
#include "stdafx.h"
//windows头文件


//自定义头文件

#include "TmtSocket.h"//socket包含需要放在最前面
#include "CipherCode.h"//软件注册常用函数
#include "CommonDefine.h"
#include "VisionStruct.h"
#include "CommonFunc.h"
#include "logger.h"//写日志

#include "CvImage.h"
#include "Thread.h"//线程类
#include "Detector.h"//算法头文件
#include "MemoryFile.h"//文件读写
#include "Queue.h"
#include "DirWatcher.h"
#include "USBCamWatcher.h"
#include "TmtCrypt.h"//字符串加密，界面
#include "XMLOperator.h"//xml读写
#include "JsonCPPHeader/json.h"//json格式
#include "TmtConfig.h"//ini配置文件格式
#include "CfgFileOp.h"//ini配置文件格式
#include "ObjToString.h"
#include "SendServer.h"
#include "ReceiveServer.h"
//opencv头文件
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"

//标准模板头文件
#include <map>
#include <vector>

using namespace cv;
using namespace std;
