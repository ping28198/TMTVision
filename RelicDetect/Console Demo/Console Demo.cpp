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

#include "RelicObj.h"
#include "RelicScn.h"
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
int main()
{
	Mat object_color = imread("..\\images\\owl_object.JPG");
	Mat scene_color = imread("..\\images\\blur_scene_3.jpg");

	RelicObj obj;
	RelicScn scene;
	obj.Load_Img(object_color);
	scene.Load_Img(scene_color);
	cout << "blur measure:" << RelicDetect::Image_Blurred(scene.img_gray,200) << endl;;
	cout << "blur lapm: " << RelicDetect::Image_Blurred_LAPM(scene.img_gray) << endl;
	cout << "blur lapv: " << RelicDetect::Image_Blurred_LAPV(scene.img_gray) << endl;
	cout << "blur teng: " << RelicDetect::Image_Blurred_TENG(scene.img_gray, 3) << endl;
	obj.Calc_Keypoints_and_Descriptors();
	scene.Calc_Keypoints_and_Descriptors();

	scene.Match_an_Obj(obj);
	scene.Draw_Obj();
	cout << "relicDetect" << endl;
	//RelicDetect doit;
	//doit.Match(obj, scene);
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

