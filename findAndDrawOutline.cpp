#include <iostream>
#include"highgui.h"
#include"cv.h"
#include"findAndDrawOutline.h"
findAndDrawOutline::findAndDrawOutline()
{
	area = 0.0;
}
//��ѯ��������������
Mat findAndDrawOutline::drawOutline(Mat image)
{
	Mat result(image.size(), CV_8UC3, Scalar(0));
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	drawContours(result, contours,-1,Scalar(191.0,19.0,206.0),1);
	return result;
}
//��ԭͼ�ϻ���ͼ���ҶԱ�,outline�Ƕ�ֵͼ����ṩ�������ң�src��ԭͼ��
Mat findAndDrawOutline::drawOutlineOfOri(Mat outline,Mat src)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//�������ݴ洢��contours��
	findContours(outline, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = 0; j < contours[i].size(); j++)
		{
			src.at<Vec3b>(contours[i][j])[0] = 191;
			src.at<Vec3b>(contours[i][j])[1] = 19;
			src.at<Vec3b>(contours[i][j])[2] = 206;
		}
	}
	return src;
}