// Console Demo.cpp : �������̨Ӧ�ó������ڵ㡣
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
	RelicDetect a;
	auto x = a.test();
	cout << x << endl;
	waitKey(0);
	system("pause");
    return 0;
}

