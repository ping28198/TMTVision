#pragma once
#ifndef _RELIC_DETECT_LIB_H_
#define _RELIC_DETECT_LIB_H_
//#define BOOST_SYSTEM_NO_LIB
//#define BOOST_FILESYSTEM_NO_LIB

#include <stdio.h>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
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
	vector<vector<int>> Get_Descriptors(Mat descriptors);
	pair<vector<KeyPoint>, Mat> RelicDetect::calc_image_keypoints_and_descriptors(Mat input_img);
private:

	void Mat_to_VecVec(Mat inmat, vector<vector<int>> &outvecvec);
};

#endif