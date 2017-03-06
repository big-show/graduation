
#include"highgui.h"
#include"cv.h"
#include <iostream>
#include"Outline.h"
#include <windows.h> 
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
void main()
{
	//FullScreen();
	//system("mode con cols=500 lines=500");
	IplImage* src;  //source image
	IplImage* dst;  //the dst image after meanshift
	int spatialRad = 14, colorRad = 29, maxPryLevel = 3;
	/*******************************对图像使用meanshift进行分割,查找轮廓的操作**********************************/
	outline outline;
	src = cvLoadImage("C:\\Users\\Administrator\\Desktop\\RespicS\\building_6\\building_6.jpg");   //load the picture
	bool building = false;
	//outline.opreationAboutOutline(src,building);
	//cvWaitKey(0);
	
	/**************************************************对图像阴影的提取**********************************************************/
	bool shadow = true;
	outline.opreationAboutOutline(src,shadow);
	cvWaitKey(0);

}
