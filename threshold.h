#ifndef  _TH_D_
#define _TH_D_
#include <iostream>
#include"highgui.h"
#include"cv.h"
using namespace cv;
class threshold_value
{
public:
	double getThreshold_otsu(IplImage *image);
};
#endif