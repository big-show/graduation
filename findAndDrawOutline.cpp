#include <iostream>
#include"highgui.h"
#include"cv.h"
#include"findAndDrawOutline.h"
findAndDrawOutline::findAndDrawOutline()
{
}
//查询轮廓并绘制轮廓
Mat findAndDrawOutline::drawOutline(Mat image)
{
	Mat result(image.size(), CV_8UC3, Scalar(0));
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	drawContours(result, contours,-1,Scalar(191.0,19.0,206.0),1);
	return result;
}
//在原图上绘制图像并且对比,outline是二值图像的提供轮廓查找，src是原图像
Mat findAndDrawOutline::drawOutlineOfOri(Mat outline,Mat src)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//轮廓数据存储在contours里
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
//计算轮廓的面积
double findAndDrawOutline::computeArea(vector<Point> outline)
{
	double area = contourArea(outline);
	return area;
}
//计算轮廓的周长
double findAndDrawOutline::computeLength(vector<Point> outline)
{
	double len = arcLength(outline, 1);
	return len;
}
//计算轮廓的近似长宽比
double* findAndDrawOutline::computeLenghtWidthRatio(double area, double perimete)
{
	double len = 0, width = 0, ratio = 0;
	int tempArea = (int)area;
	int tempPerimeter = (int)perimete;
	double *result = new double[3];
	double  minPerimeter = 0.98*tempPerimeter;
	double maxPerimeter = 1.02*tempPerimeter;
	double minArea = 0.98*tempArea;
	double maxArea = 1.02*tempArea;
	double temp_Perimeter = 2 * (len + width);
	double temp_Area = len*width;
	for (len; len < tempPerimeter / 2;)
	{
		if (temp_Perimeter >= minPerimeter&&temp_Perimeter <= maxPerimeter&&temp_Area >= minArea&&temp_Area <= maxArea)
		{
			break;
			
		}
		else
		{
			len = len + 0.1;
			width = perimete / 2 - len;
			temp_Perimeter = 2 * (len + width);
			temp_Area = len*width;
		}
	}
	if (len > width)
		ratio = len / width;
	else
		ratio = width / len;
	result[0] = len;
	result[1] = width;
	result[2] = ratio;
	return result;
}
//进行面积或长宽比例判断后将轮廓排除后的结果
//image 是二值图像，outline是存储轮廓。
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
	std::cout <<"平均面积"<< mean << std::endl;
	Mat img_after_judement(image.size(), CV_8UC3, Scalar(0));
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		double temparea = contourArea(contours[i]);
		double tempPerimeter = computeLength(contours[i]);
		double tempLenWidthRatio = computeLenghtWidthRatio(temparea, tempPerimeter)[2];
		if (temparea >= mean/4&&tempLenWidthRatio<25)
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
//进行用面积排除，以及对长宽率处理后在原图画出图像
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
	std::cout << "平均面积" << mean << std::endl;
	//Mat img_after_judement(image.size(), CV_8UC3, Scalar(0));
	//findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	for (int i = 0; i < contours.size(); i++)
	{
		double temparea = contourArea(contours[i]);
		double tempPerimeter = computeLength(contours[i]);
		double tempLenWidthRatio = computeLenghtWidthRatio(temparea, tempPerimeter)[2];
		if (temparea >= mean / minratio  && tempLenWidthRatio<25)
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
//在二值图像上标出数字记号loge,flag决定是否进行通过面积以及长宽比进行处理。
Mat findAndDrawOutline::drawLogo(Mat image, Mat outline_img, bool flag ,int maxratio, int minratio)
{
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 2;// 相当于写字的线条    


	// 初始化字体   
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);//初始化字体，准备写到图片上的   
	// cvPoint 为起笔的x，y坐标   

	vector<vector<Point>> contours;
	//轮廓数据存储在contours里
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat img_outline_logo = outline_img.clone();
	if (flag == false)
	{
		for (int i = 0; i < contours.size(); i++)
		{
			std::stringstream sstr;
			sstr << i;
			string str = sstr.str();
			putText(img_outline_logo, str, (contours[i][1]), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 254, 0), 1, 1);//在图片中输出字符  
			double area = computeArea(contours[i]);
			double preimeter = computeLength(contours[i]);
			double *result = new double[3];
			result = computeLenghtWidthRatio(area, preimeter);

			std::cout << "第" << i << "个建筑物的面积，周长，长，宽，长宽比分别为    " << area <<"       "<<preimeter<<"   "<<result[0]<<"    "<<result[1]<<"    "<<result[2]<<  std::endl;
			
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
			double tempPerimeter = computeLength(contours[i]);
			double tempLenWidthRatio = computeLenghtWidthRatio(temparea, tempPerimeter)[2];
			if (temparea >= mean / minratio  && tempLenWidthRatio<=25)
			{
				std::stringstream sstr;
				sstr << index;
				string str = sstr.str();
				putText(img_outline_logo, str, (contours[i][1]), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 254, 0), 1, 1);//在图片中输出字符  
				double area = computeArea(contours[i]);
				double preimeter = computeLength(contours[i]);
				double *result = new double[3];
				result = computeLenghtWidthRatio(area, preimeter);
				std::cout << "第" << index << "个建筑物的面积，周长，长，宽，长宽比分别为" << area << "   " << preimeter << "   " << result[0] << "    " << result[1] << "    " << result[2] << std::endl;
				index++;

			}
		}
	}
	return img_outline_logo;
}