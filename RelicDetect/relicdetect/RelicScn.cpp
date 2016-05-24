#include "RelicScn.h"
bool RelicScn::Match_a_Obj(RelicObj obj)
{
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
	cout << "H:" << endl;
	for (int i = 0;i < H.rows;i++)
	{
		for (int j = 0;j < H.cols;j++)
		{
			cout << H.at<double>(i, j) << " ";
		}
		cout << endl;
	}
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(obj.img_gray.cols, 0);
	obj_corners[2] = cvPoint(obj.img_gray.cols, obj.img_gray.rows);
	obj_corners[3] = cvPoint(0, obj.img_gray.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);
	cout << "object area" << contourArea(obj_corners) << endl;
	cout << "scene detected area" << contourArea(scene_corners) << endl;
	this->corners = scene_corners;
	double scene_area = contourArea(scene_corners);
	if (scene_area>=10000)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

void RelicScn::Draw_Obj()
{
	Mat img_draw_obj = this->img_color;
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_draw_obj, this->corners[0], this->corners[1], Scalar(0, 255, 0), 4);
	line(img_draw_obj, this->corners[1], this->corners[2], Scalar(0, 255, 0), 4);
	line(img_draw_obj, this->corners[2], this->corners[3], Scalar(0, 255, 0), 4);
	line(img_draw_obj, this->corners[3], this->corners[0], Scalar(0, 255, 0), 4);
	//-- Show detected matches
	imshow("draw obj", img_draw_obj);
}
