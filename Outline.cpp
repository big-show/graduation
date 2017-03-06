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


	/********************************对图像进行二值话处理，方便进行腐蚀膨胀操作***********************/
	threshold_value tv;
	//将彩色图像转换为灰度图像，并保存

	IplImage*img_gray = cvCreateImage(cvGetSize(dst), dst->depth, 1);//灰度图像（参数：图像宽、高、像素的位深度和通道数（单通道为灰度图像））
	//计算图像的最大类间方差值用以二值化图像。
	double threshold_otsu = tv.getThreshold_otsu(img_gray);
	cout << "灰度图像的最大类间方差值--OTSU为：    " << threshold_otsu << endl;
	cvCvtColor(dst, img_gray, CV_BGR2GRAY);
	cvSaveImage("img_gray.jpg", img_gray);//保存灰度图像
	Mat img_gray_mat(img_gray, true);
	//运用canny轮廓检测算法，对轮廓进行检测
	/*Mat after_canny_image;
	Canny(img_gray_mat, after_canny_image, 125, 225);
	namedWindow("canny detect outline");
	imshow("canny detect outline", after_canny_image);*/
	//将灰度图像转换为二值图像，并保存
	IplImage*img_binary = cvCreateImage(cvGetSize(dst), img_gray->depth, 1);//二值图像
	//处理轮廓图像
	double m = 0, sd = 0;
	m = mean(img_gray_mat)[0];
	cout << "图像灰度的平均值:      " << m << endl;
	if (shadow_flag == false)
	{
		if (threshold_otsu!=0)
		cvThreshold(img_gray, img_binary, 0, 255, CV_THRESH_OTSU );//固定阈值操作（cvThreshold函数只针对单通道图像）
		else
		cvThreshold(img_gray, img_binary, 130, 255, CV_THRESH_BINARY);//固定阈值操作（cvThreshold函数只针对单通道图像）

	}
	else
	{
		cvThreshold(img_gray, img_binary, 50, 255, CV_THRESH_BINARY_INV);
	}
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
	Mat element5(3, 3, CV_8U, Scalar(1));
	//morphologyEx(img_binary_mat, opened, MORPH_OPEN, element5);
	namedWindow("Binary Image");
	imshow("Binary Image", img_binary_mat);
	///*************************************对图像进行轮廓填充**************************/
	////http://blog.csdn.net/augusdi/article/details/9011935
	Mat img_fillhold = img_binary_mat.clone();
	fillHole(img_binary_mat, img_fillhold);
	//fillHole(after_canny_image, img_fillhold);

	namedWindow("Fillhole Image");
	imshow("Fillhole Image", img_fillhold);


	/***************************************计算并显示灰度直方图*****************************************************************/

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
	//调用 calcHist 计算直方图, 结果存放在 hist 中
	calcHist(arrays, narrays, channels, mask, resultHist, dims, histSize, ranges);
	//调用一个我自己写的简单的函数用于获取一张显示直方图数据的图片,
	//输入参数为直方图数据 hist 和期望得到的图片的尺寸
	Mat histImg = histogram1D_1.showHistogram1D(resultHist
		, Size(600, 420));
	namedWindow("src gray image histogram");
	imshow("src gray image histogram", histImg);

	/***************************************先对图像进行轮廓填充再对图像进行腐蚀膨胀开闭操作操作************************************/

	Mat erodeAfterfillhole;
	cv::erode(img_fillhold, erodeAfterfillhole, Mat());
	namedWindow("Eroded Image After fillhole");
	imshow("Eroded Image After fillhole", erodeAfterfillhole);
	Mat openedAfterfillhole;
	//结构元素尺寸
	morphologyEx(img_fillhold, openedAfterfillhole, MORPH_OPEN, element5);
	//morphologyEx(img_fillhold, openedAfterfillhole, MORPH_CLOSE, element5);
	namedWindow("Opened or Closed Image After fillhole");
	imshow("Opened or Closed Image After fillhole", openedAfterfillhole);
 
	/*****************************************对二值图像进行轮廓检测***********************************************************/
	Mat after_canny_image;
	Canny(openedAfterfillhole, after_canny_image, 125, 225);
	namedWindow("canny detect outline");
	imshow("canny detect outline", after_canny_image);

	/**************************************对二值图像画出轮廓*************************************************************/

	Mat img_outline(img_binary_mat.size(), CV_8UC3, Scalar(0));
	findAndDrawOutline outline;
	img_outline = outline.drawOutline(after_canny_image);
	namedWindow("Outline of buildings");
	imshow("Outline of buildings", img_outline);

	/***************************************将轮廓覆盖到原图像上面******************************************************/
	//将src由IplImage转换为Mat
	Mat src_outline_mat(src, true);
	findAndDrawOutline src_outline;
	src_outline_mat = src_outline.drawOutlineOfOri(openedAfterfillhole, src_outline_mat);
	namedWindow("oriWithOutline");
	imshow("oriWithOutline", src_outline_mat);

	/***************************************计算并输出轮廓面积及周长并标识******************************/
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
	img_outline_logo = src_outline.drawLogo(openedAfterfillhole, img_outline, false, 0, 0);
	namedWindow("imgWihtLogo");
	imshow("imgWihtLogo", img_outline_logo);

	/**************************************运用面积以及长宽比例排除部分轮廓后的二值轮廓图像**************************/
	findAndDrawOutline imgOutlineAfterJudement;
	Mat img_outline_after_judement(img_binary_mat.size(), CV_8UC3, Scalar(0));
	img_outline_after_judement = imgOutlineAfterJudement.drawOutlineAfterJudement(openedAfterfillhole, contours, 10, 4);
	namedWindow("binary_img Outline After Judgement");
	imshow("binary_img Outline After Judgement", img_outline_after_judement);
	if (shadow_flag==true)
	imwrite("C:\\Users\\Administrator\\Desktop\\RespicS\\shadow\\oriMethodShadow.jpg", img_outline_after_judement);
	/**************************************运用面积以及长宽比例排除部分轮廓后的二值图像**************************/

	Mat binary_img_fillhold;
	fillHole(img_outline_after_judement, binary_img_fillhold);
	cvtColor(binary_img_fillhold, binary_img_fillhold, CV_BGR2GRAY);
	threshold(binary_img_fillhold, binary_img_fillhold, 0, 255, CV_THRESH_OTSU);
	//fillHole(after_canny_image, img_fillhold);

	namedWindow("Fillhole Image After Judement of Binary");
	imshow("Fillhole Image After Judement of Binary", binary_img_fillhold);
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
	cout << "灰度图像的最大类间方差值--OTSU为：    " << threshold_otsu << endl;
	cout << "图像灰度的平均值:      " << m << endl;

	/****************************************获取阴影区域的种子生长点并画出通过种子生长点找出的阴影区域*************************************************************/
	
	vector<Point> growPoint;
	growPoint=imgOutlineAfterJudementOfOri.getGrowPoint(openedAfterfillhole, 10, 4);
	vector<Point> shadowGrowPoint;
	//对生长点进行处理.得到阴影内的生长点。
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

	/**************************************通过明暗对比获得阴影通过面积及长宽比排除后得到的阴影*********************/

	if (shadow_flag == true)
	{
		Mat shadow_light_shade = img_outline_after_judement.clone();
		//进行轮廓填充
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
	/*****************************************阴影进行角点检测****************************************/
	Mat shadow_src = imread("C:\\Users\\Administrator\\Desktop\\RespicS\\building_9\\harri_test2.jpg");
	imgOutlineAfterJudementOfOri.connerHarris(50, shadow_src);
}