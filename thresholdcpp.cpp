#include <iostream>
#include"highgui.h"
#include"cv.h"
#include"threshold.h"
using namespace cv;

double threshold_value::getThreshold_otsu(IplImage *src)
{
	int hist_size = 256;    //ֱ��ͼ�ߴ�      
	int hist_height = 256;
	float range[] = { 0, 255 };  //�Ҷȼ��ķ�Χ      
	float* ranges[] = { range };
	//����һάֱ��ͼ��ͳ��ͼ����[0 255]���صľ��ȷֲ�      
	CvHistogram* gray_hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	//����Ҷ�ͼ���һάֱ��ͼ      
	cvCalcHist(&src, gray_hist, 0, 0);
	//��һ��ֱ��ͼ      
	cvNormalizeHist(gray_hist, 1.0);

	int Width = src->width;
	int Height = src->height;
	int threshold = 0;
	double delta = 0;
	double U_t = 0;
	for (int m = 0; m<256; m++)
	{
		U_t += cvQueryHistValue_1D(gray_hist, m)*m;
	}


	double u = 0, w = 0;
	for (int k = 0; k<256; k++)
	{

		u += cvQueryHistValue_1D(gray_hist, k)*k;   //    
		w += cvQueryHistValue_1D(gray_hist, k);      //�Ҷȴ�����ֵk�����صĸ���    

		double t = U_t * w - u;
		double delta_tmp = t * t / (w * (1 - w));

		if (delta_tmp > delta)
		{
			delta = delta_tmp;
			threshold = k;
		}
	}

	cvReleaseHist(&gray_hist);
	return threshold;
}
