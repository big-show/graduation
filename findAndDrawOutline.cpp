#include <iostream>
#include"highgui.h"
#include"cv.h"
#include"findAndDrawOutline.h"
findAndDrawOutline::findAndDrawOutline()
{
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
//�������������
double findAndDrawOutline::computeArea(vector<Point> outline)
{
	double area = contourArea(outline);
	return area;
}
//��������򳤿�����жϺ������ų���Ľ��
//image �Ƕ�ֵͼ��outline�Ǵ洢������
Mat findAndDrawOutline::drawOutlineAfterJudement(Mat image, vector<vector<Point>> contours, int maxratio,int minratio)
{
	double sum = 0.0;
	int number = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		sum += contourArea(contours[i]);
		number++;
	}
	double mean = sum / (double)number;
	std::cout <<"ƽ�����"<< mean << std::endl;
	Mat img_after_judement(image.size(), CV_8UC3, Scalar(0));
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		double temparea = contourArea(contours[i]);
		if (temparea >= mean/4&&temparea<=maxratio*mean)
		{
			for (int j = 0; j < contours[i].size(); j++)
			{
				img_after_judement.at<Vec3b>(contours[i][j])[0] = 191;
				img_after_judement.at<Vec3b>(contours[i][j])[1] = 19;
				img_after_judement.at<Vec3b>(contours[i][j])[2] = 206;
			}
		}
	}
	return img_after_judement;

}
//����������ų����Լ��Գ����ʴ������ԭͼ����ͼ��
Mat findAndDrawOutline::drawOutlineAfterJudementOfOri(Mat image, vector<vector<Point>> contours, int maxratio, int minratio)
{
	double sum = 0.0;
	int number = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		sum += contourArea(contours[i]);
		number++;
	}
	double mean = sum / (double)number;
	std::cout << "ƽ�����" << mean << std::endl;
	//Mat img_after_judement(image.size(), CV_8UC3, Scalar(0));
	//findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		double temparea = contourArea(contours[i]);
		if (temparea >= mean / minratio  && temparea <= maxratio*mean)
		{
			for (int j = 0; j < contours[i].size(); j++)
			{
				image.at<Vec3b>(contours[i][j])[0] = 191;
				image.at<Vec3b>(contours[i][j])[1] = 19;
				image.at<Vec3b>(contours[i][j])[2] = 206;
			}
		}
		else
			std::cout << "out of area" << std::endl;
	}
	return image;
}
//�ڶ�ֵͼ���ϱ�����ּǺ�loge
Mat findAndDrawOutline::drawLogo(Mat image, Mat outline_img, bool flag ,int maxratio, int minratio)
{
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 2;// �൱��д�ֵ�����    


	// ��ʼ������   
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);//��ʼ�����壬׼��д��ͼƬ�ϵ�   
	// cvPoint Ϊ��ʵ�x��y����   

	vector<vector<Point>> contours;
	//�������ݴ洢��contours��
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat img_outline_logo = outline_img.clone();
	if (flag == false)
	{
		for (int i = 0; i < contours.size(); i++)
		{
			std::stringstream sstr;
			sstr << i;
			string str = sstr.str();
			putText(img_outline_logo, str, (contours[i][1]), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 254, 0), 1, 1);//��ͼƬ������ַ�  
			double area = computeArea(contours[i]);
			std::cout << "��" << i << "������������Ϊ" << area << std::endl;
		}
	}
	else
	{
		double sum = 0.0;
		int number = 0;
		for (int i = 0; i < contours.size(); i++)
		{
			sum += contourArea(contours[i]);
			number++;
		}

		double mean = sum / (double)number;
		int index = 1;
		for (int i = 0; i < contours.size();i++)
		{
			double temparea = contourArea(contours[i]);
			if (temparea >= mean / minratio  && temparea <= maxratio*mean)
			{
				std::stringstream sstr;
				sstr << index;
				string str = sstr.str();
				putText(img_outline_logo, str, (contours[i][1]), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 254, 0), 1, 1);//��ͼƬ������ַ�  
				index++;
				double area = computeArea(contours[i]);
				std::cout <<"ͨ����������ȴ����"<< "��" << index << "������������Ϊ" << area << std::endl;
			}
		}
	}
	return img_outline_logo;
}