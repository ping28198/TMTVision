// Console Demo.cpp : 定义控制台应用程序的入口点。
//
#pragma once
#include "stdafx.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"

#include "RelicDetect.h"
#include "RelicObj.h"
#include "RelicScn.h"
#include "JsonCPPHeader\json.h"
#include "RelicHelper.h"

#include "opencv_serialization.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
void relic_origin()
{
	Mat object_color = imread("..\\images\\owl_object.JPG");
	Mat scene_color = imread("..\\images\\blur_scene_3.jpg");
	//imshow("origin", object_color);
	//// create and open a character archive for output
	//std::ofstream ofs("test.txt");

	//// save data to archive
	//{
	//	boost::archive::text_oarchive oa(ofs);
	//	// write class instance to archive
	//	oa << object_color;
	//	
	//	//oa.end_preamble();
	//}
	//ofs.close();
	//Mat loaded_s;
	//{
	//	// archive and stream closed when destructors are called
	//	std::ifstream ifs("test.txt");
	//	boost::archive::text_iarchive ia(ifs);
	//	// read class state from archive

	//	ia >> loaded_s;
	//}
	//
	//	imshow("loaded",loaded_s);
	//	waitKey(0);
	//

	RelicObj obj;
	RelicScn scene;
	obj.Load_Img(object_color);
	scene.Load_Img(scene_color);
	cout << "blur measure:" << RelicDetect::Image_Blurred(scene.img_gray, 200) << endl;;
	cout << "blur lapm: " << RelicDetect::Image_Blurred_LAPM(scene.img_gray) << endl;
	cout << "blur lapv: " << RelicDetect::Image_Blurred_LAPV(scene.img_gray) << endl;
	cout << "blur teng: " << RelicDetect::Image_Blurred_TENG(scene.img_gray, 3) << endl;
	obj.Calc_Keypoints_and_Descriptors();
	scene.Calc_Keypoints_and_Descriptors();

	scene.Match_an_Obj(obj);
	scene.Draw_Obj();
	cout << "relicDetect" << endl;

	//ofstream ofs("keypoints_se.txt");
	//boost::archive::text_oarchive oa(ofs);
	//// write class instance to archive
	//stringstream oss;
	//boost::archive::text_oarchive oa2(oss);
	//oa << obj.keypoints;
	//oa2 << obj.keypoints;
	//ofs.close();
	//string str_kps;
	//str_kps = oss.str();
	//ifstream ifs("keypoints_se.txt");
	//boost::archive::text_iarchive ia(ifs);
	//vector<KeyPoint> read_kps;
	//ia >> read_kps;
	//CV_8UC1;

	/////////////////////////////////////////////////////
	//RelicDetect doit;
	//doit.Match(obj, scene);
	////-------------------------
	cout << "mat descriptors" << endl;
	for (int j = 0; j < obj.descriptors.cols; j++)
	{
		cout << obj.descriptors.at<float>(0, j) << endl;
	}
	//cout << obj.descriptors;
	////----------------------------
	//auto descriptors_vecvec = obj.Get_Descriptors(obj.descriptors);
	//cout << "vec descriptors" << endl;
	////---------------------------
	//for (int j = 0;j < descriptors_vecvec[0].size();j++)
	//{
	//	cout << descriptors_vecvec[0][j]<<endl;
	//}
	Json::Value root;string test;
	auto str = obj.Convert_to_Json(obj);
	fstream a_file("obj_json.json", ios::out);
	if (a_file.is_open())
	{
		a_file << str;
		a_file.close();
	}
	fstream in_file("obj_json.json", ios::in);
	if (in_file.is_open())
	{
		stringstream buffer;
		buffer << in_file.rdbuf();
		string json_str(buffer.str());
		RelicObj obj2;
		obj2.Parse_from_Json(json_str);
	}

	waitKey(0);
}

void relic_from_file()
{
	Mat object_color = imread("..\\images\\postcard_object.JPG");
	Mat scene_color = imread("..\\images\\postcard_scene_1.jpg");

	//RelicObj obj;
	RelicScn scene;
	//obj.Load_Img(object_color);
	scene.Load_Img(scene_color);
	//calc 
	//obj.Calc_Keypoints_and_Descriptors();
	scene.Calc_Keypoints_and_Descriptors();

	//scene.Match_an_Obj(obj);
	//scene.Draw_Obj();
	//cout << "relicDetect" << endl;

	/////////////////////////////////////////////////////
	//RelicDetect doit;
	//doit.Match(obj, scene);
	////-------------------------

	//cout << obj.descriptors;
	////----------------------------
	//auto descriptors_vecvec = obj.Get_Descriptors(obj.descriptors);
	//cout << "vec descriptors" << endl;
	////---------------------------
	//for (int j = 0;j < descriptors_vecvec[0].size();j++)
	//{
	//	cout << descriptors_vecvec[0][j]<<endl;
	//}
	//Json::Value root;string test;
	//auto str = obj.Convert_to_Json(obj);

	//fstream a_file("obj_json.json", ios::out);
	//if (a_file.is_open())
	//{
	//	a_file << str;
	//	a_file.close();
	//}
	fstream in_file("obj_json.json", ios::in);
	RelicObj obj2;
	if (in_file.is_open())
	{
		stringstream buffer;
		buffer << in_file.rdbuf();
		string json_str(buffer.str());
		
		obj2.Parse_from_Json(json_str);
	}
	//RelicDetect judge;
	//obj2.Load_Img(object_color);
	//obj2.img_color = imread("..\\images\\postcard_object.JPG", IMREAD_COLOR);
	//judge.Match(obj2, scene);
	scene.Match_an_Obj(obj2);
	scene.Draw_Obj();
	waitKey(0);
}
int main()
{
	relic_from_file();
	system("pause");
    return 0;
}

