
#include"highgui.h"
#include"cv.h"
#include <iostream>
#include"Outline.h"
#include <windows.h> 
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
void main()
{
	//FullScreen();
	//system("mode con cols=500 lines=500");
	IplImage* src;  //source image
	IplImage* dst;  //the dst image after meanshift
	int spatialRad = 14, colorRad = 29, maxPryLevel = 3;
	/*******************************��ͼ��ʹ��meanshift���зָ�,���������Ĳ���**********************************/
	outline outline;
	src = cvLoadImage("C:\\Users\\Administrator\\Desktop\\RespicS\\building_1\\building_1.jpg");   //load the picture
	bool building = false;
	outline.opreationAboutOutline(src,building);
	//cvWaitKey(0);
	
	/**************************************************��ͼ����Ӱ����ȡ**********************************************************/
	bool shadow = true;
	//outline.opreationAboutOutline(src,shadow);
	cvWaitKey(0);

}
