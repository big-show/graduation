
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

//运用漫水算法对图像的空洞进行填充。
//http://bbs.csdn.net/topics/391542633?page=1
void fillHole(const Mat srcBw, Mat &dstBw)
{
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//裁剪延展的图像
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}
void main()
{
	/*******************************对图像使用meanshift进行分割**********************************/

	src = cvLoadImage("C:\\Users\\Administrator\\Desktop\\RespicS\\building_3\\building_3.jpg");   //load the picture
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

	
	 /********************************对图像进行二值话处理，方便进行腐蚀膨胀操作***********************/
	
	 //将彩色图像转换为灰度图像，并保存
	IplImage*img_gray = cvCreateImage(cvGetSize(dst), dst->depth, 1);//灰度图像（参数：图像宽、高、像素的位深度和通道数（单通道为灰度图像））
	cvCvtColor(dst, img_gray, CV_BGR2GRAY);
	cvSaveImage("img_gray.jpg", img_gray);//保存灰度图像


	//将灰度图像转换为二值图像，并保存
	IplImage*img_binary = cvCreateImage(cvGetSize(dst), img_gray->depth, 1);//二值图像
	cvThreshold(img_gray, img_binary, 130, 255, CV_THRESH_BINARY);//固定阈值操作（cvThreshold函数只针对单通道图像）
	cvSave("img_binary", img_binary);//使用cvSaveImage保存二值图像报错？


	//创建窗口（用于显示图像）
	//cvNamedWindow("img_color", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("img_gray", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("img_binary", CV_WINDOW_AUTOSIZE);

	//cvShowImage("img_color", dst);//显示彩色图像和对应的灰度图像、二值图像
	//cvShowImage("img_gray", img_gray);
	//cvShowImage("img_binary", img_binary); 
	//将IplImage转换为Mat
	Mat img_binary_mat(img_binary, true);
	Mat element5(5, 5, CV_8U, Scalar(1));
	//morphologyEx(img_binary_mat, opened, MORPH_OPEN, element5);
	namedWindow("Binary Image");
	imshow("Binary Image", img_binary_mat);
	///*************************************对图像进行轮廓填充**************************/
	////http://blog.csdn.net/augusdi/article/details/9011935
	Mat img_fillhold = img_binary_mat.clone();
	fillHole(img_binary_mat, img_fillhold);
	namedWindow("Fillhole Image");
	imshow("Fillhole Image", img_fillhold);



	/***************************************先对图像进行轮廓填充再对图像进行腐蚀膨胀开闭操作操作************************************/

	Mat erodeAfterfillhole;
	cv::erode(img_fillhold, erodeAfterfillhole, Mat());
	namedWindow("Eroded Image After fillhole");
	imshow("Eroded Image After fillhole", erodeAfterfillhole);
	Mat openedAfterfillhole;
	//结构元素尺寸
	morphologyEx(img_fillhold, openedAfterfillhole, MORPH_OPEN, element5);
	namedWindow("Opened Image After fillhole");
	imshow("Opened Image After fillhole", openedAfterfillhole);


	/**************************************对二值图像画出轮廓*************************************************************/
	
	Mat img_outline(img_binary_mat.size(), CV_8UC3, Scalar(0));
	findAndDrawOutline outline;
	img_outline = outline.drawOutline(openedAfterfillhole);
	namedWindow("Outline of buildings");
	imshow("Outline of buildings", img_outline);

	/***************************************将轮廓覆盖到原图像上面******************************************************/
	//将src由IplImage转换为Mat
	Mat src_outline_mat(src, true);
	findAndDrawOutline src_outline;
	src_outline_mat = src_outline.drawOutlineOfOri(openedAfterfillhole, src_outline_mat);
	namedWindow("oriWithOutline");
	imshow("oriWithOutline", src_outline_mat);
	
	/***************************************计算并输出轮廓面积并标识******************************/
	//对轮廓进行标识
	//CvFont font;
	//double hScale = 1;
	//double vScale = 1;
	//int lineWidth = 2;// 相当于写字的线条    
	//
	//
	//// 初始化字体   
	//cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);//初始化字体，准备写到图片上的   
	//// cvPoint 为起笔的x，y坐标   
	//
	vector<vector<Point>> contours;
	////轮廓数据存储在contours里
	findContours(openedAfterfillhole, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat img_outline_logo = img_outline.clone();
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	stringstream sstr;
	//	sstr << i;
	//	string str = sstr.str();
	//	putText(img_outline_logo, str, (contours[i][1]), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 254, 0), 1, 1);//在图片中输出字符  
	//	double area=outline.computeArea(contours[i]);
	//	cout << "第" << i << "个建筑物的面积为" << area << endl;
	//}
	img_outline_logo = src_outline.drawLogo(openedAfterfillhole, img_outline,false,0,0);
	namedWindow("imgWihtLogo");
	imshow("imgWihtLogo", img_outline_logo);

	/**************************************运用面积以及长宽比例排除部分轮廓后的二值轮廓图像**************************/
	findAndDrawOutline imgOutlineAfterJudement;
	Mat img_outline_after_judement(img_binary_mat.size(), CV_8UC3, Scalar(0));
	img_outline_after_judement = imgOutlineAfterJudement.drawOutlineAfterJudement(openedAfterfillhole, contours, 10, 4);
	namedWindow("binary_img Outline After Judgement");
	imshow("binary_img Outline After Judgement", img_outline_after_judement);
	
	/**************************************运用面积以及长宽比例排除部分轮廓后的原图轮廓图像**************************/
	
	Mat src_outline_mat_1(src, true);
	findAndDrawOutline imgOutlineAfterJudementOfOri;
	Mat img_outline_after_judement_of_ori(img_binary_mat.size(), CV_8UC3, Scalar(0));
	img_outline_after_judement_of_ori = imgOutlineAfterJudementOfOri.drawOutlineAfterJudementOfOri(src_outline_mat_1, contours, 10, 4);
	//namedWindow("Ori Outline After Judgement ");
	imshow("Ori Outline After Judgement", img_outline_after_judement_of_ori);

	/***************************************运用面积及长款比例排除部分轮廓后计算并输出轮廓面积并标识******************************/
	
	Mat img_outline_logo_after_judement = img_outline_after_judement.clone();
	img_outline_logo_after_judement = src_outline.drawLogo(openedAfterfillhole, img_outline_after_judement, true, 10, 4);
	namedWindow("img With Logo After Judement");
	imshow("img With Logo After Judement", img_outline_logo_after_judement);
	cvWaitKey(0);


} 
