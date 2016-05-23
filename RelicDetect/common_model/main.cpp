// common_model.cpp : 定义控制台应用程序的入口点。
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
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
/* @function main */
int main(int argc, char** argv)
{
	Mat img_object_color = imread("F:\\Tdevelop\\TMT\\TMTVision\\RelicDetect\\images\\postcard_object.JPG");
	Mat img_object;
	cvtColor(img_object_color, img_object, CV_BGR2GRAY);
	Mat img_scene_color = imread("F:\\Tdevelop\\TMT\\TMTVision\\RelicDetect\\images\\postcard_scene_1.JPG");
	Mat img_scene;
	cvtColor(img_scene_color, img_scene, CV_BGR2GRAY);
	
	if (!img_object.data || !img_scene.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; 
		system("pause");
		return -1;
	}
	//-- Step 1: Detect the keypoints and extract descriptors using SURF
	//设置Hessian矩阵的最小值，只有大于这个值的特征点才会被保留
	int minHessian = 200;
	//建立一个surf对象，保存在cv::Ptr（智能指针）内
	Ptr<SURF> detector = SURF::create(minHessian,4,3,false);
	//创建两个vector，分别用来存储目标物object和环境scene的特征点keypoints
	std::vector<KeyPoint> keypoints_object, keypoints_scene,kp_o;
	//创建两个矩阵，用来存储描述子descriptor
	Mat descriptors_object, descriptors_scene,d_o;
	//detect keypoints 并 计算 descriptors。第二个参数为mask，这里为空。
	//detector->detectAndCompute(img_object, Mat(), keypoints_object, descriptors_object);
	detector->detectAndCompute(img_scene, Mat(), keypoints_scene, descriptors_scene);

	detector->detect(img_object, keypoints_object);
	detector->compute(img_object, keypoints_object, descriptors_object);
	//-- Step 2: Matching descriptor vectors using FLANN matcher

	//for (int j = 0; j < descriptors_object.cols; j++)
	//{
	//	cout<<descriptors_object.at<int>(0, j)<<endl;
	//}
	
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);
	double max_dist = 0; double min_dist = 100;
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);
	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance <= 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}
	max_dist = 0;min_dist = 100;double total_min_dist = 0;
	for (int i = 0; i < good_matches.size(); i++)
	{
		double dist = good_matches[i].distance;
		total_min_dist += dist;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;

	}
	printf("-- good matches Max dist : %f \n", max_dist);
	printf("-- good matches Min dist : %f \n", min_dist);
	printf("-- good matches total Min dist : %f \n", total_min_dist);
	cout << "-- good matches size " << good_matches.size() << endl;
	cout << "-- dist per match" << total_min_dist/(double)good_matches.size() << endl;
	Mat img_matches;
	drawMatches(img_object_color, keypoints_object, img_scene_color, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	//imshow("matches", img_matches);
	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;
	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}
	Mat H = findHomography(obj, scene, RANSAC);
	cout << "H:" << endl;
	for (int i = 0;i < H.rows;i++)
	{
		for (int j = 0;j < H.cols;j++)
		{
			cout << H.at<double>(i, j)<<" ";
		}
		cout << endl;
	}
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); 
	obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); 
	obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);
	cout << "object area" << contourArea(obj_corners) << endl;
	cout << "scene detected area" << contourArea(scene_corners) << endl;
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);
	waitKey(0);
	return 0;
}
