
#include"highgui.h"
#include"cv.h"
#include"shadow.h"
#include <iostream>
#include"Outline.h"
using namespace cv;
using namespace std;




void main()
{
	IplImage* src;  //source image
	IplImage* dst;  //the dst image after meanshift
	int spatialRad = 14, colorRad = 29, maxPryLevel = 3;
	/*******************************��ͼ��ʹ��meanshift���зָ�,���������Ĳ���**********************************/
	outline outline;
	src = cvLoadImage("C:\\Users\\Administrator\\Desktop\\RespicS\\building_7\\building_7.jpg");   //load the picture
	bool building = false;
	outline.opreationAboutOutline(src,building);
	//cvWaitKey(0);
	
	/**************************************************��ͼ����Ӱ����ȡ**********************************************************/
	bool shadow = true;
	//outline.opreationAboutOutline(src,shadow);
	cvWaitKey(0);

}
