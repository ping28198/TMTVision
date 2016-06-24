#ifndef RELIC
#define RELIC
#endif 
#include "RelicDetect.hpp"


RelicDetect::RelicDetect()
{
	
}

string RelicDetect::test()
{
	return "Hello I'm relic!!";
}
void RelicDetect::Load_Img(InputArray img)
{
	RelicDetect::img_color = img.getMat();
	cvtColor(img, RelicDetect::img_gray, CV_BGR2GRAY);
}

void RelicDetect::Calc_Keypoints_and_Descriptors()
{
	int minHessian = 200;
	//建立一个surf对象，保存在cv::Ptr（智能指针）内
	Ptr<SURF> detector = SURF::create(minHessian, 4, 3, false);//第四个参数为true代表采用surf-128descriptor，效果更好，但会慢些
	//创建vector，用来特征点keypoints
	std::vector<KeyPoint> keypoints;
	//创建矩阵，用来存储描述子descriptor
	Mat descriptors;
	//detect keypoints 并 计算 descriptors。第二个参数为mask，这里为空。
	detector->detectAndCompute(RelicDetect::img_gray, Mat(), keypoints, descriptors);

	RelicDetect::descriptors = descriptors;
	RelicDetect::keypoints = keypoints;

	//equal to above
	//detector->detect(input_img, keypoints);
	//detector->compute(input_img, keypoints, descriptors);
}

//vector<vector<int>> RelicDetect::Get_Descriptors(Mat img)
//{
//	pair<vector<KeyPoint>, Mat> feature = calc_image_keypoints_and_descriptors(img);
//	Mat descriptors = feature.second;
//	vector<vector<int>> descriptors_vecvec;
//	Mat_to_VecVec(descriptors, descriptors_vecvec);
//	return descriptors_vecvec;
//}
void RelicDetect::Mat_to_VecVec(Mat inmat, vector<vector<int>> &outvecvec)
{
	int rows = inmat.rows;
	int cols = inmat.cols;
	outvecvec.resize(rows);
	for (int i = 0;i < rows;i++)
	{
		outvecvec[i].resize(cols);
	}
	for (int row = 0;row < rows;row++)
	{
		for (int col = 0;col < cols;col++)
		{
			outvecvec[row][col] = inmat.at<int>(row, col);
		}
	}
}

bool RelicDetect::Match(RelicDetect obj,RelicDetect scn)
{
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	
	matcher.match(obj.descriptors, scn.descriptors, matches);
	double max_dist = 0; double min_dist = 100;
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < obj.descriptors.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);
	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;
	for (int i = 0; i < obj.descriptors.rows; i++)
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
	cout << "-- dist per match" << total_min_dist / (double)good_matches.size() << endl;
	Mat img_matches;
	drawMatches(obj.img_color, obj.keypoints, scn.img_color, scn.keypoints,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	//imshow("matches", img_matches);
	//-- Localize the object
	std::vector<Point2f> obj_points;
	std::vector<Point2f> scn_points;
	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj_points.push_back(obj.keypoints[good_matches[i].queryIdx].pt);
		scn_points.push_back(scn.keypoints[good_matches[i].trainIdx].pt);
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
	obj_corners[1] = cvPoint(obj.img_color.cols, 0);
	obj_corners[2] = cvPoint(obj.img_color.cols, obj.img_color.rows);
	obj_corners[3] = cvPoint(0, obj.img_color.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);
	cout << "object area" << contourArea(obj_corners) << endl;
	cout << "scene detected area" << contourArea(scene_corners) << endl;
	auto scene_area = contourArea(scene_corners);
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(obj.img_color.cols, 0), scene_corners[1] + Point2f(obj.img_color.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(obj.img_color.cols, 0), scene_corners[2] + Point2f(obj.img_color.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(obj.img_color.cols, 0), scene_corners[3] + Point2f(obj.img_color.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(obj.img_color.cols, 0), scene_corners[0] + Point2f(obj.img_color.cols, 0), Scalar(0, 255, 0), 4);
	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);
	waitKey(0);
	if (scene_area>1000)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

double RelicDetect::Get_min_Dist(vector<DMatch> matches)
{
	double max_dist = 0; double min_dist = 100;
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < matches.size(); i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	return min_dist;
}
vector<DMatch> RelicDetect::Get_Good_Matches(vector<DMatch> matches)
{
	double min_dist = Get_min_Dist(matches);
	std::vector< DMatch > good_matches;
	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance <= 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}
	return good_matches;
}

bool RelicDetect::Image_Blurred(Mat img, int blur_threshold)
{
	cv::Mat lap;
	cv::Laplacian(img, lap, CV_64F);

	cv::Scalar mu, sigma;
	cv::meanStdDev(lap, mu, sigma);

	double focusMeasure = sigma.val[0] * sigma.val[0];
	if (focusMeasure>=blur_threshold)
	{
		return false;
	} 
	else
	{// only focus measure less than blur_threshold is considered blurry
		return true;
	}
}

double RelicDetect::Image_Blurred_LAPM(Mat img)
{
	cv::Mat M = (Mat_<double>(3, 1) << -1, 2, -1);
	cv::Mat G = cv::getGaussianKernel(3, -1, CV_64F);

	cv::Mat Lx;
	cv::sepFilter2D(img, Lx, CV_64F, M, G);

	cv::Mat Ly;
	cv::sepFilter2D(img, Ly, CV_64F, G, M);

	cv::Mat FM = cv::abs(Lx) + cv::abs(Ly);

	double focusMeasure = cv::mean(FM).val[0];
	return focusMeasure;
}
double RelicDetect::Image_Blurred_LAPV(Mat img)
{
	cv::Mat lap;
	cv::Laplacian(img, lap, CV_64F);

	cv::Scalar mu, sigma;
	cv::meanStdDev(lap, mu, sigma);

	double focusMeasure = sigma.val[0] * sigma.val[0];
	return focusMeasure;
}
double RelicDetect::Image_Blurred_TENG(Mat img,int ksize)
{
	cv::Mat Gx, Gy;
	cv::Sobel(img, Gx, CV_64F, 1, 0, ksize);
	cv::Sobel(img, Gy, CV_64F, 0, 1, ksize);

	cv::Mat FM = Gx.mul(Gx) + Gy.mul(Gy);

	double focusMeasure = cv::mean(FM).val[0];
	return focusMeasure;
}
