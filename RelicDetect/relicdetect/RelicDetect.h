
#ifndef _RELIC_DETECT_LIB_H_
#define _RELIC_DETECT_LIB_H_

#include <stdio.h>
#include <string>
#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

class RelicDetect
{
private:
	void Mat_to_VecVec(Mat inmat, vector<vector<int>> &outvecvec);
public:
	RelicDetect();
	string test();
	void Load_Img(InputArray img);
	void Calc_Keypoints_and_Descriptors();
	bool RelicDetect::Match(RelicDetect obj, RelicDetect scn);
	double Get_min_Dist(vector<DMatch> matches);
	vector<DMatch> Get_Good_Matches(vector<DMatch> matches);
public:
	Mat img_color;
	Mat img_gray;
	Mat descriptors;
	vector<KeyPoint> keypoints;



};

#endif