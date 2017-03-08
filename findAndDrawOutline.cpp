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
//�����������ܳ�
double findAndDrawOutline::computeLength(vector<Point> outline)
{
	double len = arcLength(outline, 1);
	return len;
}
//���������Ľ��Ƴ����
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
		double tempPerimeter = computeLength(contours[i]);
		double tempLenWidthRatio = computeLenghtWidthRatio(temparea, tempPerimeter)[2];
		if (temparea >= mean/4&&tempLenWidthRatio<30)
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
		double tempPerimeter = computeLength(contours[i]);
		double tempLenWidthRatio = computeLenghtWidthRatio(temparea, tempPerimeter)[2];
		if (temparea >= mean / minratio  && tempLenWidthRatio<30)
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
//�ڶ�ֵͼ���ϱ�����ּǺ�loge,flag�����Ƿ����ͨ������Լ�����Ƚ��д���
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
			double preimeter = computeLength(contours[i]);
			double *result = new double[3];
			result = computeLenghtWidthRatio(area, preimeter);
			if (result[0]>result[1])
			std::cout << "��" << i << "��������������ܳ�������������ȷֱ�Ϊ    " << area <<"       "<<preimeter<<"   "<<result[0]<<"    "<<result[1]<<"    "<<result[2]<<  std::endl;
			else
			std::cout << "��" << i << "��������������ܳ�������������ȷֱ�Ϊ    " << area << "       " << preimeter << "   " << result[1] << "    " << result[0] << "    " << result[2] << std::endl;

			
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
			if (temparea >= mean / minratio  && tempLenWidthRatio<=30)
			{
				std::stringstream sstr;
				sstr << index;
				string str = sstr.str();
				putText(img_outline_logo, str, (contours[i][1]), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 254, 0), 1, 1);//��ͼƬ������ַ�  
				//std::cout << "��" << index << "�����ӵ������" << contours[i][1].x << "," << contours[i][1].y << std::endl;
				double area = computeArea(contours[i]);
				double preimeter = computeLength(contours[i]);
				double *result = new double[3];
				result = computeLenghtWidthRatio(area, preimeter);
				if (result[0]>result[1])
					std::cout << "��" << index << "�����������Ϊ    " << area + preimeter << "       " << preimeter << "   " << result[0] << "    " << result[1] << "    " << result[2] << std::endl;
				else
					std::cout << "��" << index << "�����������Ϊ    " << area + preimeter << "       " << preimeter << "   " << result[1] << "    " << result[0] << "    " << result[2] << std::endl;
				index++;

			}
		}
	}
	return img_outline_logo;
}
//������������������
vector<Point> findAndDrawOutline::getGrowPoint(Mat image, int maxratio, int minratio)
{
	vector<vector<Point>> contours;
	//�������ݴ洢��contours��
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	vector<Point> growPoint;
	double sum = 0.0;
	int number = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		sum += contourArea(contours[i]);
		number++;
	}

	double mean = sum / (double)number;
	int index = 1;
	for (int i = 0; i < contours.size(); i++)
	{
		double temparea = contourArea(contours[i]);
		double tempPerimeter = computeLength(contours[i]);
		double tempLenWidthRatio = computeLenghtWidthRatio(temparea, tempPerimeter)[2];
		if (temparea >= mean / minratio  && tempLenWidthRatio <= 30)
		{
			growPoint.push_back(contours[i][1]);
			std::cout << "��" << index << "�����ӵ������" << contours[i][1].x << "," << contours[i][1].y << std::endl;
			index++;

		}
	}

	return growPoint;
}
//���ݽ��������ӵ����Ӱ�������������õ���ͼ��
Mat findAndDrawOutline::drawShadowOfGrowPoint(Mat src,vector< Point> pt, int th)
{
	Mat matDst = Mat::zeros(src.size(), CV_8UC1);   //����һ���հ��������Ϊ��ɫ  
	for (int i = 0; i < pt.size(); i++)
	{
		Point ptGrowing;                      //��������λ��  
		int nGrowLable = 0;                             //����Ƿ�������  
		int nSrcValue = 0;                              //�������Ҷ�ֵ  
		int nCurValue = 0;                              //��ǰ������Ҷ�ֵ  
		//��������˳������  
		int DIR[8][2] = { { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 } };
		Vector<Point> vcGrowPt;                     //������ջ  
		vcGrowPt.push_back(pt[i]);                         //��������ѹ��ջ��  
		matDst.at<uchar>(pt[i].y, pt[i].x) = 255;               //���������  
		nSrcValue = src.at<uchar>(pt[i].y, pt[i].x);            //��¼������ĻҶ�ֵ  

		while (!vcGrowPt.empty())                       //����ջ��Ϊ��������  
		{
			Point tempPt;
			tempPt = vcGrowPt.back();                       //ȡ��һ��������  
			vcGrowPt.pop_back();

			//�ֱ�԰˸������ϵĵ��������  
			for (int i = 0; i < 9; ++i)
			{
				ptGrowing.x = tempPt.x + DIR[i][0];
				ptGrowing.y = tempPt.y + DIR[i][1];
				//����Ƿ��Ǳ�Ե��  
				if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
					continue;

				nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);      //��ǰ��������ĻҶ�ֵ  

				if (nGrowLable == 0)                    //�����ǵ㻹û�б�����  
				{
					nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
					if (abs(nSrcValue - nCurValue) < th )                 //����ֵ��Χ��������  
					{
						matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;     //���Ϊ��ɫ  
						vcGrowPt.push_back(ptGrowing);                  //����һ��������ѹ��ջ��  
					}
				}
			}
		}
	}
	return matDst.clone();
}
//�Ľ���ͨ���ǵ��ⷨ������Ӱ�ĸ߶ȡ�
void findAndDrawOutline::connerHarris(int th,Mat src)
{
	int maxCorners =30;
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	Mat src_gray;
	Mat copy = src.clone();
	cvtColor(src, src_gray, CV_BGR2GRAY);
	/// Copy the source image  
	Mat cormat;
	/// Apply corner detection :Determines strong corners on an image.  
	goodFeaturesToTrack(src_gray,
		corners,
		maxCorners,
		qualityLevel,
		minDistance,
		Mat(),
		blockSize,
		useHarrisDetector,
		k);
	std::cout << corners << std::endl;
	/// Draw corners detected  
	for (int i = 0; i < corners.size(); i++){
		circle(copy, corners[i], 2, Scalar(65,254,254), 2, 8, 0);
		circle(src, corners[i], 4, Scalar(0, 255, 0), 2, 8, 0);
	}
	//����ƽ��yֵ����
	int midHeight = 0;
	int sumHeight = 0;
	for (int i = 0; i < corners.size(); i++)
	{
		sumHeight += corners[i].y;
	}
	midHeight = sumHeight / corners.size();
	std::cout << midHeight << std::endl;
	//�洢�����ϱ߽��Լ��±߽�ĵ�Ԫ��.
	vector<Point2f> topArr;
	vector<Point2f> bottomArr;
	for (int j = 0; j < corners.size(); j++)
	{
		if (corners[j].y < midHeight)
			topArr.push_back(corners[j]);
		else
			bottomArr.push_back(corners[j]);
	}
	/*std::cout << "�ϱ߽�Ԫ��" << topArr << std::endl;
	std::cout << "�±߽�Ԫ��" << bottomArr << std::endl;*/
	//��������Ԫ��,�ҵ��±�Ԫ�غ��������С��5�����صĵ��������
	int index = 0;
	double sumLine = 0;
	double meanLine = 0;
	for (int i = 0; i < topArr.size(); i++)
	{
		for (int j = 0; j < bottomArr.size(); j++)
		{
			if (abs(topArr[i].x - bottomArr[j].x) <= 3)
			{
				index++;
				line(copy, topArr[i], bottomArr[j], cv::Scalar(0, 0, 255));
				int tempX = topArr[i].x - bottomArr[j].x;
				int tempY = topArr[i].y - bottomArr[j].y;
				sumLine += sqrt(tempX*tempX + tempY*tempY);
				std::cout << "��" << index << "��ǵ��е�һ����" << "������ x=    " << topArr[i].x << "  ������ y=  " << topArr[i].y << std::endl;
				std::cout << "��" << index << "��ǵ��еڶ�����" << "������ x=    " << bottomArr[j].x << "  ������ y=  " << bottomArr[j].y << std::endl;
				std::cout << "��" << index << "��ǵ�֮������߳���Ϊ" << sqrt(tempX*tempX + tempY*tempY) << std::endl;
				//ɾ��ƥ����ĵײ��ǵ㣻
				bottomArr[j].x = 10000;
				break;
			}

		}
	}
		meanLine = sumLine / index;
		std::cout << "ͨ���ǵ㷨�������Ӱ��ƽ������Ϊ   " << meanLine << std::endl;
		std::cout << "ʵ����Ӱ����" << meanLine*0.61 <<std:: endl;
		std::cout << "���������߶�Ϊ" << meanLine*0.61*1.2975 << std::endl;
		std::cout << "ʵ�ʸ߶������Ϊ" << std::abs(22.56 - meanLine*0.61*1.2975) << std::endl;
		std::cout << "�����׼ȷ��Ϊ" << (double)(1 - std::abs(22.56 - meanLine*0.61*1.2975) / 22.56) <<std::endl;
	
	/// չʾͼ�� 
	imshow("corners_window", copy);
	imshow("source_window", src);
}
//δ�Ľ���ͨ���ǵ��ⷨ������Ӱ�ĸ߶ȡ�
void findAndDrawOutline::connerHarris_f(int th, Mat src)
{
	int maxCorners = 30;
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	Mat src_gray;
	Mat copy = src.clone();
	cvtColor(src, src_gray, CV_BGR2GRAY);
	/// Copy the source image  
	Mat cormat;
	/// Apply corner detection :Determines strong corners on an image.  
	goodFeaturesToTrack(src_gray,
		corners,
		maxCorners,
		qualityLevel,
		minDistance,
		Mat(),
		blockSize,
		useHarrisDetector,
		k);
	std::cout << corners << std::endl;
	/// Draw corners detected  
	for (int i = 0; i < corners.size(); i++){
		circle(copy, corners[i], 2, Scalar(65, 254, 254), 2, 8, 0);
		circle(src, corners[i], 4, Scalar(0, 255, 0), 2, 8, 0);
	}
	//����ƽ��yֵ����
	int midHeight = 0;
	int sumHeight = 0;
	for (int i = 0; i < corners.size(); i++)
	{
		sumHeight += corners[i].y;
	}
	midHeight = sumHeight / corners.size();
	std::cout << midHeight << std::endl;
	//�洢�����ϱ߽��Լ��±߽�ĵ�Ԫ��.
	vector<Point2f> topArr;
	vector<Point2f> bottomArr;
	for (int j = 0; j < corners.size(); j++)
	{
		if (corners[j].y < midHeight)
			topArr.push_back(corners[j]);
		else
			bottomArr.push_back(corners[j]);
	}
	/*std::cout << "�ϱ߽�Ԫ��" << topArr << std::endl;
	std::cout << "�±߽�Ԫ��" << bottomArr << std::endl;*/
	//��������Ԫ��,�ҵ��±�Ԫ�غ��������С��5�����صĵ��������
	int index = 0;
	double sumLine = 0;
	double meanLine = 0;
	for (int i = 0; i < topArr.size(); i++)
	{
		for (int j = 0; j < bottomArr.size(); j++)
		{
			if (abs(topArr[i].x - bottomArr[j].x) <= 3)
			{
				index++;
				line(copy, topArr[i], bottomArr[j], cv::Scalar(0, 0, 255));
				int tempX = topArr[i].x - bottomArr[j].x;
				int tempY = topArr[i].y - bottomArr[j].y;
				sumLine += sqrt(tempX*tempX + tempY*tempY);
				std::cout << "��" << index << "��ǵ��е�һ����" << "������ x=    " << topArr[i].x << "  ������ y=  " << topArr[i].y << std::endl;
				std::cout << "��" << index << "��ǵ��еڶ�����" << "������ x=    " << bottomArr[j].x << "  ������ y=  " << bottomArr[j].y << std::endl;
				std::cout << "��" << index << "��ǵ�֮������߳���Ϊ" << sqrt(tempX*tempX + tempY*tempY) << std::endl;
				//ɾ��ƥ����ĵײ��ǵ㣻
				//bottomArr[j].x = 10000;
				break;
			}

		}
	}
	meanLine = sumLine / index;
	std::cout << "ͨ���ǵ㷨�������Ӱ��ƽ������Ϊ   " << meanLine << std::endl;
	std::cout << "ʵ����Ӱ����" << meanLine*0.61 << std::endl;
	std::cout << "���������߶�Ϊ" << meanLine*0.61*1.2975 << std::endl;
	std::cout << "ʵ�ʸ߶������Ϊ" << std::abs(22.56 - meanLine*0.61*1.2975) << std::endl;
	std::cout << "�����׼ȷ��Ϊ" << (double)(1 - std::abs(22.56 - meanLine*0.61*1.2975) / 22.56) << std::endl;

	/// չʾͼ�� 
	imshow("corners_window_f", copy);
	imshow("source_window_f", src);
}