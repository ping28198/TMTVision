
#include "RelicDetect.h"

int k = 222;

RelicDetect::RelicDetect()
{
	
}

string RelicDetect::test()
{
	return "Hello I'm relic!!";
}

void RelicDetect::Calc_Keypoints_and_Descriptors(Mat input_img)
{
	int minHessian = 200;
	//建立一个surf对象，保存在cv::Ptr（智能指针）内
	Ptr<SURF> detector = SURF::create(minHessian, 4, 3, false);//第四个参数为true代表采用surf-128descriptor，效果更好，但会慢些
	//创建vector，用来特征点keypoints
	std::vector<KeyPoint> keypoints;
	//创建矩阵，用来存储描述子descriptor
	Mat descriptors;
	//detect keypoints 并 计算 descriptors。第二个参数为mask，这里为空。
	detector->detectAndCompute(input_img, Mat(), keypoints, descriptors);

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
