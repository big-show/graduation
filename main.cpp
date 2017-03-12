
#include"highgui.h"
#include"cv.h"
#include <iostream>
#include"Outline.h"
#include <windows.h> 
#include<stdio.h>
using namespace cv;
using namespace std;


//设置控制台全屏显示
void FullScreen(){

	HWND hwnd = GetForegroundWindow();
	int x = GetSystemMetrics(SM_CXSCREEN)/2 + 300, y = GetSystemMetrics(SM_CYSCREEN)/2 + 300;
	char setting[30];
	sprintf(setting, "mode con:cols=%d lines=%d", x, y);//设置控制台行数列数
	system(setting);

	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, x + 300, y + 300, NULL);//置顶


	MoveWindow(hwnd, -10, -40, x + 300, y + 300, 1);//移动
	printf("\n\n");
}
//后台服务器端程序
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
//		/*******************************对图像使用meanshift进行分割,查找轮廓的操作**********************************/
//		outline outline;
//		src = cvLoadImage(".\\1.jpg");//load the picture
//		Mat gg(src, true);
//		printf("%s", "true do");
//		imwrite("gogogo.jpg", gg);
//		bool building = false;
//		outline.opreationAboutOutline(src, building);
//		/**************************************************对图像阴影的提取**********************************************************/
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
		/*******************************对图像使用meanshift进行分割,查找轮廓的操作**********************************/
		outline outline;
		src = cvLoadImage("C:\\Users\\Administrator\\Desktop\\RespicS\\web\\1.jpg");//load the picture
		Mat gg(src, true);
		printf("%s", "true do");
		imwrite("gogogo.jpg", gg);
		bool building = false;
		outline.opreationAboutOutline(src, building);
		/**************************************************对图像阴影的提取**********************************************************/
		bool shadow = true;
		printf("%s", "done");
		//outline.opreationAboutOutline(src,shadow);
		cvWaitKey(0);
	

}
