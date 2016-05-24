#include "RelicObj.h"
void RelicObj::Load_Img(InputArray img)
{
	this->img_color = img.getMat();
	cvtColor(img,this->img_gray,CV_BGR2GRAY);
	this->img_height = this->img_gray.rows;
	this->img_width = this->img_gray.cols;
}