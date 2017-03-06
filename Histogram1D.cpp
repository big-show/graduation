#include <iostream>
#include"highgui.h"
#include"cv.h"
#include"Histogram1D.h"
using namespace cv;
Mat Histogram1D::showHistogram1D(Mat hist, Size imgSize)
{
	Mat histImg(imgSize, CV_8UC3);
	int Padding = 10;
	int W = imgSize.width - 2 * Padding;
	int H = imgSize.height - 2 * Padding;
	double _max;
	minMaxLoc(hist, NULL, &_max);
	double Per = (double)H / _max;
	const Point Orig(Padding, imgSize.height - Padding);
	int bin = W / (hist.rows + 2);

	//������
	for (int i = 1; i <= hist.rows; i++)
	{
		Point pBottom(Orig.x + i * bin, Orig.y);
		Point pTop(pBottom.x, pBottom.y - Per * hist.at<float>(i - 1));
		line(histImg, pBottom, pTop, Scalar(255, 0, 0), bin);
	}
	double bin_u = (double)histImg.rows / _max;
	//�� 3 �����߱�������
	line(histImg, Point(Orig.x + bin, Orig.y - H), Point(Orig.x + hist.rows *  bin, Orig.y - H), Scalar(0, 0, 255), 1);
	line(histImg, Point(Orig.x + bin, Orig.y), Point(Orig.x + bin, Orig.y - H), Scalar(0, 0, 255), 1);
	line(histImg, Point(Orig.x + hist.rows * bin, Orig.y), Point(Orig.x + hist.rows *  bin, Orig.y - H), Scalar(0, 0, 255), 1);
	/*drawArrow(histImg, Orig, Orig + Point(W, 0), 10, 30, Scalar::all(0), 2);
	drawArrow(histImg, Orig, Orig - Point(0, H), 10, 30, Scalar::all(0), 2);*/
	//���̶�
	char string[12];//���ת����ʮ��������ת����ʮ���ƺ��λ��������12λ�������������Լ��趨
	//��������̶ȣ����ظ�����
	int kedu = 0;
	for (int i = 1; kedu<_max; i++)
	{
		kedu = i*_max / 10;//�˴�ѡ��10���̶�
		_itoa(kedu, string, 10);//��һ������ת��Ϊ�ַ�����������е�10ָʮ����
		//��ͼ������ʾ�ı��ַ���
		putText(histImg, string, Point(0, histImg.rows - kedu*bin_u), 1, 1, Scalar(0));//��Ӧ�ɰ��е�cvPutText����
	}
	//��������̶ȣ����ػҶ�ֵ��
	kedu = 0;
	for (int i = 1; kedu<256; i++)
	{
		kedu = i * 20;//�˴�ѡ����Ϊ20
		_itoa(kedu, string, 10);//��һ������ת��Ϊ�ַ���
		//��ͼ������ʾ�ı��ַ���
		putText(histImg, string, cvPoint(kedu*(histImg.cols / 256), histImg.rows), 1, 1, Scalar(0));
	}
	return histImg;
}