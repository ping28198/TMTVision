
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
	//����һ��surf���󣬱�����cv::Ptr������ָ�룩��
	Ptr<SURF> detector = SURF::create(minHessian, 4, 3, false);//���ĸ�����Ϊtrue�������surf-128descriptor��Ч�����ã�������Щ
	//����vector������������keypoints
	std::vector<KeyPoint> keypoints;
	//�������������洢������descriptor
	Mat descriptors;
	//detect keypoints �� ���� descriptors���ڶ�������Ϊmask������Ϊ�ա�
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
