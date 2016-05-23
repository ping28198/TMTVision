
#ifndef _RELIC_DETECT_LIB_H_
#define _RELIC_DETECT_LIB_H_

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

extern int k;

class RelicDetect
{
public:
	RelicDetect();
	string test();
	void Calc_Keypoints_and_Descriptors(Mat input_img);
	//vector<vector<int>> Get_Descriptors(Mat descriptors);
	Mat descriptors;
	vector<KeyPoint> keypoints;
private:
	void Mat_to_VecVec(Mat inmat, vector<vector<int>> &outvecvec);
};

#endif