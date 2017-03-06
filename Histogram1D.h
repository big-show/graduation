#ifndef  _HI_D_
#define _HI_D_
#include <iostream>
#include"highgui.h"
#include"cv.h"
using namespace cv;
class Histogram1D
{
public:
	Mat showHistogram1D(Mat image, Size imgSize);
};
#endif