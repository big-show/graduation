
#include"highgui.h"
#include"cv.h"
#include <iostream>
#include"Outline.h"
#include <windows.h> 
#include<stdio.h>
using namespace cv;
using namespace std;


//���ÿ���̨ȫ����ʾ
void FullScreen(){

	HWND hwnd = GetForegroundWindow();
	int x = GetSystemMetrics(SM_CXSCREEN)/2 + 300, y = GetSystemMetrics(SM_CYSCREEN)/2 + 300;
	char setting[30];
	sprintf(setting, "mode con:cols=%d lines=%d", x, y);//���ÿ���̨��������
	system(setting);

	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, x + 300, y + 300, NULL);//�ö�


	MoveWindow(hwnd, -10, -40, x + 300, y + 300, 1);//�ƶ�
	printf("\n\n");
}
Mat prewitt(Mat imageP)
{
	//cvtColor(imageP, imageP, CV_RGB2GRAY);
	float prewittx[9] =
	{
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1
	};
	float prewitty[9] =
	{
		1, 1, 1,
		0, 0, 0,
		-1, -1, -1
	};
	Mat px = Mat(3, 3, CV_32F, prewittx);
	cout << px << endl;
	Mat py = Mat(3, 3, CV_32F, prewitty);
	cout << py << endl;
	Mat dstx = Mat(imageP.size(), imageP.type(), imageP.channels());
	Mat dsty = Mat(imageP.size(), imageP.type(), imageP.channels());
	Mat dst = Mat(imageP.size(), imageP.type(), imageP.channels());
	filter2D(imageP, dstx, imageP.depth(), px);
	filter2D(imageP, dsty, imageP.depth(), py);
	float tempx, tempy, temp;
	for (int i = 0; i<imageP.rows; i++)
	{
		for (int j = 0; j<imageP.cols; j++)
		{
			tempx = dstx.at<uchar>(i, j);
			tempy = dsty.at<uchar>(i, j);
			temp = sqrt(tempx*tempx + tempy*tempy);
			dst.at<uchar>(i, j) = temp;
		}
	}
	return dst;
}
Mat roberts(cv::Mat srcImage)
{
	cv::Mat dstImage = srcImage.clone();
	int nRows = dstImage.rows;
	int nCols = dstImage.cols;
	for (int i = 0; i < nRows - 1; i++)
	{
		for (int j = 0; j < nCols - 1; j++)
		{
			int t1 = (srcImage.at<uchar>(i, j) -
				srcImage.at<uchar>(i + 1, j + 1)) *
				(srcImage.at<uchar>(i, j) -
				srcImage.at<uchar>(i + 1, j + 1));
			int t2 = (srcImage.at<uchar>(i + 1, j) -
				srcImage.at<uchar>(i, j + 1)) *
				(srcImage.at<uchar>(i + 1, j) -
				srcImage.at<uchar>(i, j + 1));
			dstImage.at<uchar>(i, j) = (uchar)sqrt(t1 + t2);

		}
	}
	return dstImage;
}
//��̨�������˳���
//int main(int argc, char *argv[])
//{
//	
//	IplImage* src;  //source image
//	IplImage* dst;  //the dst image after meanshift
//	int spatialRad = 14, colorRad = 29, maxPryLevel = 3;
//	int imageName;
//	int k = 0;
//	if (argc > 1)
//	{
//		imageName = atol(argv[1]);
//		k = atol(argv[2]);
//		printf("%d\n", imageName);
//		printf("%d\n", k);
//		getchar();
//		vector<string> result;
//		/*******************************��ͼ��ʹ��meanshift���зָ�,���������Ĳ���**********************************/
//		outline outline;
//		src = cvLoadImage(".\\1.jpg");//load the picture
//		Mat gg(src, true);
//		printf("%s", "true do");
//		imwrite("gogogo.jpg", gg);
//		bool building = false;
//		outline.opreationAboutOutline(src, building);
//		/**************************************************��ͼ����Ӱ����ȡ**********************************************************/
//		bool shadow = true;
//		printf("%s", "done");
//		//outline.opreationAboutOutline(src,shadow);
//		//cvWaitKey(0);
//	}
//	else
//	{
//		cout << "enter k an file" << endl;
//		getchar();
//	}
//	
//	return 0;
//
//}
void main()
{

	IplImage* src;  //source image
	IplImage* dst;  //the dst image after meanshift
	int spatialRad = 14, colorRad = 29, maxPryLevel = 3;
	int imageName;
	int k = 0;
	
		vector<string> result;
		/*******************************��ͼ��ʹ��meanshift���зָ�,���������Ĳ���**********************************/
		//outline outline;
		//src = cvLoadImage("C:\\Users\\Administrator\\Desktop\\RespicS\\web\\1.jpg");//load the picture
		//Mat gg(src, true);
		//printf("%s", "true do");
		//imwrite("gogogo.jpg", gg);
		//bool building = false;
		//outline.opreationAboutOutline(src, building);
		///**************************************************��ͼ����Ӱ����ȡ**********************************************************/
		//bool shadow = true;
		//printf("%s", "done");
		//outline.opreationAboutOutline(src,shadow);
		/*****************************soble���*******/
		Mat src_gray = imread("C:\\Users\\Administrator\\Desktop\\RespicS\\building_2\\building_2_erode.jpg", 0);
		namedWindow("src");
		imshow("src", src_gray);
		Mat grad_x, grad_y;
		Mat abs_grad_x, abs_grad_y;
		int scale = 1;
		int delta = 0;
		int ddepth = CV_16S;
		Mat grad;
		/// �� X�����ݶ�
		//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
		Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_x, abs_grad_x);

		/// ��Y�����ݶ�
		//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
		Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
		convertScaleAbs(grad_y, abs_grad_y);

		/// �ϲ��ݶ�(����)
		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
		namedWindow("sobel");
		imshow("sobel", grad);
		/**************************************prewitt���*******************************/
		Mat dst_prewitt = prewitt(src_gray);
		namedWindow("prewitt");
		imshow("prewitt", dst_prewitt);
		/**************************************robert���*******************************/
		Mat dst_roberts = roberts(src_gray);
		namedWindow("roberts");
		imshow("roberts", dst_roberts);
		/************************************laplace���***************************/
		Mat dst_lac, abs_dst_laplace;
		int kernel_size = 3;
		Laplacian(src_gray, dst_lac, CV_16S, 3, 1, 0, BORDER_DEFAULT);		
		convertScaleAbs(dst_lac, abs_dst_laplace);
		namedWindow("result_laplacian");
		imshow("result_laplacian", abs_dst_laplace);
		/************************************canny���***************************/
		Mat dst_canny;
		Canny(src_gray, dst_canny, 125, 225);
		namedWindow("canny");
		imshow("canny", dst_canny);
		cvWaitKey(0);

}
