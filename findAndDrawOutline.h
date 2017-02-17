#include <iostream>
#include"highgui.h"
#include"cv.h"
using namespace cv;
class findAndDrawOutline
{
	double area;
public:
	findAndDrawOutline();
	Mat drawOutline( Mat image);
	Mat drawOutlineOfOri(Mat outline,Mat src);
};