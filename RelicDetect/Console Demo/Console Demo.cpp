// Console Demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "RelicDetect.h"
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
int main()
{
	Mat object_color = imread("..\\images\\postcard_object.JPG");
	imshow("origin_object", object_color);
	Mat object_gray;
	cvtColor(object_color, object_gray, CV_BGR2GRAY);
	RelicDetect a;
	auto feature = a.calc_image_keypoints_and_descriptors(object_gray);
	//-------------------------
	cout << "mat descriptors" << endl;
	for (int j = 0; j < feature.second.cols; j++)
	{
		cout << feature.second.at<int>(0, j) << endl;
	}
	//----------------------------
	auto descriptors_vecvec = a.Get_Descriptors(feature.second);
	cout << "vec descriptors" << endl;
	//---------------------------

		for (int j = 0;j < descriptors_vecvec[0].size();j++)
		{
			cout << descriptors_vecvec[0][j]<<endl;
		}

	auto x = a.test();
	cout << x << endl;
	waitKey(0);
	system("pause");
    return 0;
}

