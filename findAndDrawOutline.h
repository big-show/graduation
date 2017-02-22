#ifndef  _FI_E_
#define _FI_E_
#include <iostream>
#include"highgui.h"
#include"cv.h"
using namespace cv;
class findAndDrawOutline
{
public:
	findAndDrawOutline();
	Mat drawOutline( Mat image);
	Mat drawOutlineOfOri(Mat outline,Mat src);
	double computeArea(vector<Point> outline);
	Mat drawOutlineAfterJudement(Mat image, vector<vector<Point>> contours, int maxratio, int minratio);
	Mat drawOutlineAfterJudementOfOri(Mat image, vector<vector<Point>> contours, int maxratio, int minratio);
	Mat drawLogo(Mat image, Mat outline_logo, bool flag, int maxratio, int minratio);
};
#endif