#ifndef RELIC
#define RELIC
#endif 
#include "RelicScn.hpp"
bool RelicScn::Match_an_Obj(RelicObj obj)
{
	string message;

	FlannBasedMatcher matcher;
	vector<DMatch> matches;

	matcher.match(obj.descriptors, this->descriptors, matches);
	vector<DMatch> good_matches = Get_Good_Matches(matches);

	//-- Localize the object
	std::vector<Point2f> obj_points;
	std::vector<Point2f> scn_points;
	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj_points.push_back(obj.keypoints[good_matches[i].queryIdx].pt);
		scn_points.push_back(this->keypoints[good_matches[i].trainIdx].pt);
	}
	Mat H = cv::findHomography(obj_points, scn_points, RANSAC);
	//cout << "H:" << endl;
	//for (int i = 0;i < H.rows;i++)
	//{
	//	for (int j = 0;j < H.cols;j++)
	//	{
	//		cout << H.at<double>(i, j) << " ";
	//	}
	//	cout << endl;
	//}
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(obj.img_width, 0);
	obj_corners[2] = cvPoint(obj.img_width, obj.img_height);
	obj_corners[3] = cvPoint(0, obj.img_height);

	std::vector<Point2f> possible_obj_corners(4);
	perspectiveTransform(obj_corners, possible_obj_corners, H);
	BOOST_LOG_TRIVIAL(info) << "ԭʼĿ�������С�����أ��� " << contourArea(obj_corners);
	BOOST_LOG_TRIVIAL(info) << "��⵽�������С�����أ��� " << contourArea(possible_obj_corners);
	this->corners = possible_obj_corners;
	double possible_target_area = contourArea(possible_obj_corners);
	double whole_scene_area = this->img_gray.rows*this->img_gray.cols;
	BOOST_LOG_TRIVIAL(info) << "����ͼ���С�����أ��� " << whole_scene_area;
	double ratio = possible_target_area / whole_scene_area;
	BOOST_LOG_TRIVIAL(info) << "��⵽��Ŀ��ռȫͼ������ " << ratio;
	if (ratio>0.10)
	{
		BOOST_LOG_TRIVIAL(info) << "�ɹ���⵽Ŀ�����壡";
		return true;
	} 
	else
	{
		BOOST_LOG_TRIVIAL(info) << "δ�ܼ�⵽Ŀ�����壡";
		return false;
	}
}

void RelicScn::Draw_Obj()
{
	Mat img_draw_obj = this->img_color;
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_draw_obj, this->corners[0], this->corners[1], Scalar(0, 255, 0), 2);
	line(img_draw_obj, this->corners[1], this->corners[2], Scalar(0, 255, 0), 2);
	line(img_draw_obj, this->corners[2], this->corners[3], Scalar(0, 255, 0), 2);
	line(img_draw_obj, this->corners[3], this->corners[0], Scalar(0, 255, 0), 2);
	//-- Show detected matches
	imshow("draw obj", img_draw_obj);
}
