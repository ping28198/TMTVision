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

#include "RelicObj.h"
#include "RelicScn.h"
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
int main()
{
	Mat object_color = imread("..\\images\\postcard_object.JPG");
	Mat scene_color = imread("..\\images\\postcard_scene_1.jpg");
	//imshow("origin_object", object_color);
	Mat object_gray,scene_gray;
	cvtColor(object_color, object_gray, CV_BGR2GRAY);
	cvtColor(scene_color, scene_gray, CV_BGR2GRAY);

	RelicObj obj;
	RelicScn scene;
	obj.Load_Img(object_color);
	scene.Load_Img(scene_color);
	obj.Calc_Keypoints_and_Descriptors();
	scene.Calc_Keypoints_and_Descriptors();

	scene.Match_a_Obj(obj);
	scene.Draw_Obj();
	cout << "relicDetect" << endl;
	RelicDetect doit;
	doit.Match(obj, scene);
	////-------------------------
	//cout << "mat descriptors" << endl;
	//for (int j = 0; j < obj.descriptors.cols; j++)
	//{
	//	cout << obj.descriptors.at<int>(0, j) << endl;
	//}
	////----------------------------
	//auto descriptors_vecvec = obj.Get_Descriptors(obj.descriptors);
	//cout << "vec descriptors" << endl;
	////---------------------------
	//for (int j = 0;j < descriptors_vecvec[0].size();j++)
	//{
	//	cout << descriptors_vecvec[0][j]<<endl;
	//}


	waitKey(0);
	system("pause");
    return 0;
}

