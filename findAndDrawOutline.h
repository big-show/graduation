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
};