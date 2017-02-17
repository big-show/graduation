
#include"highgui.h"
#include"cv.h"
#include"findAndDrawOutline.h"
#include <iostream>

using namespace cv;
using namespace std;


IplImage* src;  //source image
IplImage* dst;  //the dst image after meanshift
int spatialRad = 14, colorRad = 29, maxPryLevel = 3;


void on_Meanshift(int)  //the callback function
{

	//cout<<"spatialRad="<<spatialRad<<endl;   //for test
	//cout<<"   colorRad="<<colorRad<<endl;
	//cout<<"        maxPryLevel="<<maxPryLevel<<endl;
	cvPyrMeanShiftFiltering(src, dst, spatialRad, colorRad, maxPryLevel);  //segmentation use meanshift
	cvShowImage("dst", dst);   //show the segmented image

}

//������ˮ�㷨��ͼ��Ŀն�������䡣
//http://bbs.csdn.net/topics/391542633?page=1
void fillHole(const Mat srcBw, Mat &dstBw)
{
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//��չͼ��
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//�ü���չ��ͼ��
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}
void main()
{
	/*******************************��ͼ��ʹ��meanshift���зָ�**********************************/

	src = cvLoadImage("C:\\Users\\Administrator\\Desktop\\RespicS\\building_2\\building_2.jpg");   //load the picture
	CvSize size;
	size.width = src->width;
	size.height = src->height;
	dst = cvCreateImage(size, src->depth, 3);  //set the size of the dst image
	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("src", src);
	cvPyrMeanShiftFiltering(src, dst, spatialRad, colorRad, maxPryLevel);

	//create the trackbar
	cvCreateTrackbar("spatialRad", "dst", &spatialRad, 50, on_Meanshift);
	cvCreateTrackbar("colorRad", "dst", &colorRad, 60, on_Meanshift);
	cvCreateTrackbar("maxPryLevel", "dst", &maxPryLevel, 5, on_Meanshift);

	 cvShowImage("dst", dst);

	
	 /********************************��ͼ����ж�ֵ������������и�ʴ���Ͳ���***********************/
	
	 //����ɫͼ��ת��Ϊ�Ҷ�ͼ�񣬲�����
	IplImage*img_gray = cvCreateImage(cvGetSize(dst), dst->depth, 1);//�Ҷ�ͼ�񣨲�����ͼ����ߡ����ص�λ��Ⱥ�ͨ��������ͨ��Ϊ�Ҷ�ͼ�񣩣�
	cvCvtColor(dst, img_gray, CV_BGR2GRAY);
	cvSaveImage("img_gray.jpg", img_gray);//����Ҷ�ͼ��


	//���Ҷ�ͼ��ת��Ϊ��ֵͼ�񣬲�����
	IplImage*img_binary = cvCreateImage(cvGetSize(dst), img_gray->depth, 1);//��ֵͼ��
	cvThreshold(img_gray, img_binary, 130, 255, CV_THRESH_BINARY);//�̶���ֵ������cvThreshold����ֻ��Ե�ͨ��ͼ��
	cvSave("img_binary", img_binary);//ʹ��cvSaveImage�����ֵͼ�񱨴�


	//�������ڣ�������ʾͼ��
	cvNamedWindow("img_color", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("img_gray", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("img_binary", CV_WINDOW_AUTOSIZE);

	cvShowImage("img_color", dst);//��ʾ��ɫͼ��Ͷ�Ӧ�ĻҶ�ͼ�񡢶�ֵͼ��
	//cvShowImage("img_gray", img_gray);
	cvShowImage("img_binary", img_binary); 
	//��IplImageת��ΪMat
	Mat img_binary_mat(img_binary, true);
	Mat element5(5, 5, CV_8U, Scalar(1));
	//morphologyEx(img_binary_mat, opened, MORPH_OPEN, element5);
	//namedWindow("Opened Image");
	//imshow("Opened Image", opened);
	///*************************************��ͼ������������**************************/
	////http://blog.csdn.net/augusdi/article/details/9011935
	Mat img_fillhold = img_binary_mat.clone();
	fillHole(img_binary_mat, img_fillhold);
	namedWindow("Fillhole Image");
	imshow("Fillhole Image", img_fillhold);



	/***************************************�ȶ�ͼ�������������ٶ�ͼ����и�ʴ���Ϳ��ղ�������************************************/

	Mat erodeAfterfillhole;
	cv::erode(img_fillhold, erodeAfterfillhole, Mat());
	namedWindow("Eroded Image After fillhole");
	imshow("Eroded Image After fillhole", erodeAfterfillhole);
	Mat openedAfterfillhole;
	//�ṹԪ�سߴ�
	morphologyEx(img_fillhold, openedAfterfillhole, MORPH_OPEN, element5);
	namedWindow("Opened Image After fillhole");
	imshow("Opened Image After fillhole", openedAfterfillhole);


	/**************************************�Զ�ֵͼ�񻭳�����*************************************************************/
	
	Mat img_outline(img_binary_mat.size(), CV_8UC3, Scalar(0));
	findAndDrawOutline outline;
	img_outline = outline.drawOutline(openedAfterfillhole);
	namedWindow("Outline of buildings");
	imshow("Outline of buildings", img_outline);

	/***************************************���������ǵ�ԭͼ������******************************************************/
	//��src��IplImageת��ΪMat
	Mat src_outline_mat(src, true);
	findAndDrawOutline src_outline;
	src_outline_mat = src_outline.drawOutlineOfOri(openedAfterfillhole, src_outline_mat);
	namedWindow("oriWithOutline");
	imshow("oriWithOutline", src_outline_mat);
	
	/***************************************���㲢����������******************************/
	//���������б�ʶ
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 2;// �൱��д�ֵ�����    
	
	
	// ��ʼ������   
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);//��ʼ�����壬׼��д��ͼƬ�ϵ�   
	// cvPoint Ϊ��ʵ�x��y����   
	
	vector<vector<Point>> contours;
	//�������ݴ洢��contours��
	findContours(openedAfterfillhole, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat img_outline_logo = img_outline.clone();
	for (int i = 0; i < contours.size(); i++)
	{
		stringstream sstr;
		sstr << i;
		string str = sstr.str();
		putText(img_outline_logo, str, (contours[i][1]), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 254, 0), 1, 1);//��ͼƬ������ַ�  
		double area=outline.computeArea(contours[i]);
		cout << "��" << i << "������������Ϊ" << area << endl;
	}
	namedWindow("imgWihtLogo");
	imshow("imgWihtLogo", img_outline_logo);
	cvWaitKey(0);
} 
