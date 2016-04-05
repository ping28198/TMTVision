
#pragma once
#include "stdafx.h"
//windows头文件


//自定义头文件
#include "TmtSocket.h"//socket包含需要放在最前面
#include "CommonDefine.h"
#include "VisionStruct.h"
#include "CommonFunc.h"
#include "logger.h"
#include "CfgFileOp.h"
#include "CvImage.h"
#include "Thread.h"
#include "Detector.h"
#include "MemoryFile.h"

#include "TmtCrypt.h"
//opencv头文件
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"

//标准模板头文件
#include <map>
#include <vector>

using namespace cv;
using namespace std;
