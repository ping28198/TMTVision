#pragma once

#include <stdio.h>
#include <string>
#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"

//#include "JsonCPPHeader\json.h"
#include "..\jsoncpp\json\json.h"
#include "..\Console Demo\opencv_serialization.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

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
		bool Match(RelicDetect obj, RelicDetect scn);
		double Get_min_Dist(vector<DMatch> matches);
		vector<DMatch> Get_Good_Matches(vector<DMatch> matches);
		// test for blur
		static double Image_Blurred_LAPM(Mat img);
		static bool Image_Blurred(Mat img, int blur_threshold);
		static double Image_Blurred_LAPV(Mat img);
		static double RelicDetect::Image_Blurred_TENG(Mat img, int ksize);
	public:

		Mat img_color;
		Mat img_gray;
		Mat descriptors;
		vector<KeyPoint> keypoints;
		string text;
	};

