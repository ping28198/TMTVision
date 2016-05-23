#pragma once
#ifndef _RELIC_DETECT_LIB_H_
#define _RELIC_DETECT_LIB_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
void fun(int a);

extern int k;

class RelicDetect
{
public:
	RelicDetect();
	string test();
};

#endif