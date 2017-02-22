#ifndef  _OU_E_
#define _OU_E_
#include"highgui.h"
#include"cv.h"
#include"findAndDrawOutline.h"
#include <iostream>
#include"Histogram1D.h"
#include"threshold.h"
using namespace cv;
using namespace std;
class outline
{
public:
	void opreationAboutOutline(IplImage *image,bool flag);
};
#endif