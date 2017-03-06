#include"highgui.h"
#include"cv.h"
#include"findAndDrawOutline.h"
#include <iostream>

#include"Outline.h"
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
	//http://blog.csdn.net/dcrmg/article/details/52705087
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
void outline::opreationAboutOutline(IplImage *src,bool shadow_flag)
{
	//IplImage* src;  //source image
	IplImage* dst;  //the dst image after meanshift
	int spatialRad = 14, colorRad = 29, maxPryLevel = 3;
	CvSize size;
	size.width = src->width;
	size.height = src->height;
	dst = cvCreateImage(size, src->depth, 3);  //set the size of the dst image
	//cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("src", src);
	cvPyrMeanShiftFiltering(src, dst, spatialRad, colorRad, maxPryLevel);

	//create the trackbar
	cvCreateTrackbar("spatialRad", "dst", &spatialRad, 50, on_Meanshift);
	cvCreateTrackbar("colorRad", "dst", &colorRad, 60, on_Meanshift);
	cvCreateTrackbar("maxPryLevel", "dst", &maxPryLevel, 5, on_Meanshift);

	cvShowImage("dst", dst);


	/********************************��ͼ����ж�ֵ������������и�ʴ���Ͳ���***********************/
	threshold_value tv;
	//����ɫͼ��ת��Ϊ�Ҷ�ͼ�񣬲�����

	IplImage*img_gray = cvCreateImage(cvGetSize(dst), dst->depth, 1);//�Ҷ�ͼ�񣨲�����ͼ����ߡ����ص�λ��Ⱥ�ͨ��������ͨ��Ϊ�Ҷ�ͼ�񣩣�
	//����ͼ��������䷽��ֵ���Զ�ֵ��ͼ��
	double threshold_otsu = tv.getThreshold_otsu(img_gray);
	cout << "�Ҷ�ͼ��������䷽��ֵ--OTSUΪ��    " << threshold_otsu << endl;
	cvCvtColor(dst, img_gray, CV_BGR2GRAY);
	cvSaveImage("img_gray.jpg", img_gray);//����Ҷ�ͼ��
	Mat img_gray_mat(img_gray, true);
	//����canny��������㷨�����������м��
	/*Mat after_canny_image;
	Canny(img_gray_mat, after_canny_image, 125, 225);
	namedWindow("canny detect outline");
	imshow("canny detect outline", after_canny_image);*/
	//���Ҷ�ͼ��ת��Ϊ��ֵͼ�񣬲�����
	IplImage*img_binary = cvCreateImage(cvGetSize(dst), img_gray->depth, 1);//��ֵͼ��
	//��������ͼ��
	double m = 0, sd = 0;
	m = mean(img_gray_mat)[0];
	cout << "ͼ��Ҷȵ�ƽ��ֵ:      " << m << endl;
	if (shadow_flag == false)
	{
		if (threshold_otsu!=0)
		cvThreshold(img_gray, img_binary, 0, 255, CV_THRESH_OTSU );//�̶���ֵ������cvThreshold����ֻ��Ե�ͨ��ͼ��
		else
		cvThreshold(img_gray, img_binary, 130, 255, CV_THRESH_BINARY);//�̶���ֵ������cvThreshold����ֻ��Ե�ͨ��ͼ��

	}
	else
	{
		cvThreshold(img_gray, img_binary, 50, 255, CV_THRESH_BINARY_INV);
	}
	cvSave("img_binary", img_binary);//ʹ��cvSaveImage�����ֵͼ�񱨴�

	
	//�������ڣ�������ʾͼ��
	//cvNamedWindow("img_color", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("img_gray", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("img_binary", CV_WINDOW_AUTOSIZE);

	//cvShowImage("img_color", dst);//��ʾ��ɫͼ��Ͷ�Ӧ�ĻҶ�ͼ�񡢶�ֵͼ��
	//cvShowImage("img_gray", img_gray);
	//cvShowImage("img_binary", img_binary); 
	//��IplImageת��ΪMat
	Mat img_binary_mat(img_binary, true);
	Mat element5(3, 3, CV_8U, Scalar(1));
	//morphologyEx(img_binary_mat, opened, MORPH_OPEN, element5);
	namedWindow("Binary Image");
	imshow("Binary Image", img_binary_mat);
	///*************************************��ͼ������������**************************/
	////http://blog.csdn.net/augusdi/article/details/9011935
	Mat img_fillhold = img_binary_mat.clone();
	fillHole(img_binary_mat, img_fillhold);
	//fillHole(after_canny_image, img_fillhold);

	namedWindow("Fillhole Image");
	imshow("Fillhole Image", img_fillhold);


	/***************************************���㲢��ʾ�Ҷ�ֱ��ͼ*****************************************************************/

	Histogram1D histogram1D_1;
	Mat hist(img_gray, true);
	imshow("gray image", hist);
	Mat* arrays = &hist;
	int narrays = 1;
	int channels[] = { 0 };
	InputArray mask = noArray();
	Mat resultHist;
	int dims = 1;
	int histSize[] = { 256 };
	float hranges[] = { 0.0, 255.0 };
	const float *ranges[] = { hranges };
	//���� calcHist ����ֱ��ͼ, �������� hist ��
	calcHist(arrays, narrays, channels, mask, resultHist, dims, histSize, ranges);
	//����һ�����Լ�д�ļ򵥵ĺ������ڻ�ȡһ����ʾֱ��ͼ���ݵ�ͼƬ,
	//�������Ϊֱ��ͼ���� hist �������õ���ͼƬ�ĳߴ�
	Mat histImg = histogram1D_1.showHistogram1D(resultHist
		, Size(600, 420));
	namedWindow("src gray image histogram");
	imshow("src gray image histogram", histImg);

	/***************************************�ȶ�ͼ�������������ٶ�ͼ����и�ʴ���Ϳ��ղ�������************************************/

	Mat erodeAfterfillhole;
	cv::erode(img_fillhold, erodeAfterfillhole, Mat());
	namedWindow("Eroded Image After fillhole");
	imshow("Eroded Image After fillhole", erodeAfterfillhole);
	Mat openedAfterfillhole;
	//�ṹԪ�سߴ�
	morphologyEx(img_fillhold, openedAfterfillhole, MORPH_OPEN, element5);
	//morphologyEx(img_fillhold, openedAfterfillhole, MORPH_CLOSE, element5);
	namedWindow("Opened or Closed Image After fillhole");
	imshow("Opened or Closed Image After fillhole", openedAfterfillhole);
 
	/*****************************************�Զ�ֵͼ������������***********************************************************/
	Mat after_canny_image;
	Canny(openedAfterfillhole, after_canny_image, 125, 225);
	namedWindow("canny detect outline");
	imshow("canny detect outline", after_canny_image);

	/**************************************�Զ�ֵͼ�񻭳�����*************************************************************/

	Mat img_outline(img_binary_mat.size(), CV_8UC3, Scalar(0));
	findAndDrawOutline outline;
	img_outline = outline.drawOutline(after_canny_image);
	namedWindow("Outline of buildings");
	imshow("Outline of buildings", img_outline);

	/***************************************���������ǵ�ԭͼ������******************************************************/
	//��src��IplImageת��ΪMat
	Mat src_outline_mat(src, true);
	findAndDrawOutline src_outline;
	src_outline_mat = src_outline.drawOutlineOfOri(openedAfterfillhole, src_outline_mat);
	namedWindow("oriWithOutline");
	imshow("oriWithOutline", src_outline_mat);

	/***************************************���㲢�������������ܳ�����ʶ******************************/
	//���������б�ʶ
	//CvFont font;
	//double hScale = 1;
	//double vScale = 1;
	//int lineWidth = 2;// �൱��д�ֵ�����    
	//
	//
	//// ��ʼ������   
	//cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);//��ʼ�����壬׼��д��ͼƬ�ϵ�   
	//// cvPoint Ϊ��ʵ�x��y����   
	//
	vector<vector<Point>> contours;
	////�������ݴ洢��contours��
	findContours(openedAfterfillhole, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat img_outline_logo = img_outline.clone();
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	stringstream sstr;
	//	sstr << i;
	//	string str = sstr.str();
	//	putText(img_outline_logo, str, (contours[i][1]), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 254, 0), 1, 1);//��ͼƬ������ַ�  
	//	double area=outline.computeArea(contours[i]);
	//	cout << "��" << i << "������������Ϊ" << area << endl;
	//}
	img_outline_logo = src_outline.drawLogo(openedAfterfillhole, img_outline, false, 0, 0);
	namedWindow("imgWihtLogo");
	imshow("imgWihtLogo", img_outline_logo);

	/**************************************��������Լ���������ų�����������Ķ�ֵ����ͼ��**************************/
	findAndDrawOutline imgOutlineAfterJudement;
	Mat img_outline_after_judement(img_binary_mat.size(), CV_8UC3, Scalar(0));
	img_outline_after_judement = imgOutlineAfterJudement.drawOutlineAfterJudement(openedAfterfillhole, contours, 10, 4);
	namedWindow("binary_img Outline After Judgement");
	imshow("binary_img Outline After Judgement", img_outline_after_judement);
	if (shadow_flag==true)
	imwrite("C:\\Users\\Administrator\\Desktop\\RespicS\\shadow\\oriMethodShadow.jpg", img_outline_after_judement);
	/**************************************��������Լ���������ų�����������Ķ�ֵͼ��**************************/

	Mat binary_img_fillhold;
	fillHole(img_outline_after_judement, binary_img_fillhold);
	cvtColor(binary_img_fillhold, binary_img_fillhold, CV_BGR2GRAY);
	threshold(binary_img_fillhold, binary_img_fillhold, 0, 255, CV_THRESH_OTSU);
	//fillHole(after_canny_image, img_fillhold);

	namedWindow("Fillhole Image After Judement of Binary");
	imshow("Fillhole Image After Judement of Binary", binary_img_fillhold);
	/**************************************��������Լ���������ų������������ԭͼ����ͼ��**************************/

	Mat src_outline_mat_1(src, true);
	findAndDrawOutline imgOutlineAfterJudementOfOri;
	Mat img_outline_after_judement_of_ori(img_binary_mat.size(), CV_8UC3, Scalar(0));
	img_outline_after_judement_of_ori = imgOutlineAfterJudementOfOri.drawOutlineAfterJudementOfOri(src_outline_mat_1, contours, 10, 4);
	//namedWindow("Ori Outline After Judgement ");
	imshow("Ori Outline After Judgement", img_outline_after_judement_of_ori);

	/***************************************�����������������ų�������������㲢��������������ʶ******************************/

	Mat img_outline_logo_after_judement = img_outline_after_judement.clone();
	img_outline_logo_after_judement = src_outline.drawLogo(openedAfterfillhole, img_outline_after_judement, true, 10, 4);
	namedWindow("img With Logo After Judement");
	imshow("img With Logo After Judement", img_outline_logo_after_judement);
	cout << "�Ҷ�ͼ��������䷽��ֵ--OTSUΪ��    " << threshold_otsu << endl;
	cout << "ͼ��Ҷȵ�ƽ��ֵ:      " << m << endl;

	/****************************************��ȡ��Ӱ��������������㲢����ͨ�������������ҳ�����Ӱ����*************************************************************/
	
	vector<Point> growPoint;
	growPoint=imgOutlineAfterJudementOfOri.getGrowPoint(openedAfterfillhole, 10, 4);
	vector<Point> shadowGrowPoint;
	//����������д���.�õ���Ӱ�ڵ������㡣
	for (int i = 0; i < growPoint.size(); i++)
	{
		Point tempPoint = growPoint[i];
		Point resPoint;
		resPoint.x = tempPoint.x;
		if (tempPoint.y - 3 >= 0)
			resPoint.y = tempPoint.y - 3;
		shadowGrowPoint.push_back(resPoint);
	}
	Mat growPointShadow;
	growPointShadow = imgOutlineAfterJudementOfOri.drawShadowOfGrowPoint(img_gray_mat, shadowGrowPoint, 20);
	namedWindow("shadow Of GrowPoint");
	imshow("shadow Of GrowPoint",growPointShadow);
	if (shadow_flag==false)
	imwrite("C:\\Users\\Administrator\\Desktop\\RespicS\\shadow\\growPoingShadow.jpg", growPointShadow);
	int i=0;
	int b = 1;

	/**************************************ͨ�������ԱȻ����Ӱͨ�������������ų���õ�����Ӱ*********************/

	if (shadow_flag == true)
	{
		Mat shadow_light_shade = img_outline_after_judement.clone();
		//�����������
		fillHole(img_outline_after_judement, shadow_light_shade);
		Mat shadow_gray(shadow_light_shade.size(),shadow_light_shade.type());
		cvtColor(shadow_light_shade, shadow_gray, CV_BGR2GRAY);
		Mat dst_shadow;
		//dst_shadow.create(shadow_light_shade.size(), shadow_light_shade.type());
		threshold(shadow_gray, dst_shadow, 230, 255, CV_THRESH_BINARY);
		namedWindow("dst_shadow");
		imshow("shadow_light_shade", dst_shadow);
		imwrite("C:\\Users\\Administrator\\Desktop\\RespicS\\shadow\\shadow_light_shade.jpg", dst_shadow);


	}
}