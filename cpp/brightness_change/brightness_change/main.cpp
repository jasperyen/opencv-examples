
#include <stdio.h>
#include <iostream>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


void drawHistImg(Mat &src, Mat &dst) {
	int histSize = 256;
	float histMaxValue = 0;
	for (int i = 0; i<histSize; i++) {
		float tempValue = src.at<float>(i);
		if (histMaxValue < tempValue) {;
			histMaxValue = tempValue;
		}
	}

	//histMaxValue = 5000;
	//cout << histMaxValue;

	float scale = (1 * 256) / histMaxValue;
	for (int i = 0; i<histSize; i++) {
		int intensity = static_cast<int>(src.at<float>(i)*scale);
		line(dst, Point(i, 255), Point(i, 255 - intensity), Scalar(0));
	}
}

void getHist(Mat &src, Mat &dst) {
	int histSize = 256;
	float range[] = { 0, 255 };
	const float* histRange = { range };
	Mat histImg;
	calcHist(&src, 1, 0, Mat(), histImg, 1, &histSize, &histRange);

	//把直方圖秀在一個256*256大的影像上
	dst = Mat( Size(256, 256), CV_8UC1, Scalar(255) );
	
	drawHistImg(histImg, dst);
}


int main() {

	double alpah = 1;
	int beta = 0;
	double gamma = 1;


	Mat image = imread("..\\..\\..\\test_data\\lena.png");
	Mat grayimg, histimg;
	cvtColor(image, grayimg, CV_BGR2GRAY);

	getHist(grayimg, histimg);
	imshow("src", image);
	imshow("src histimg", histimg);

	//	直方圖均等化
	equalizeHist(grayimg, grayimg);
	getHist(grayimg, histimg);
	imshow("histimg after equalize", histimg);

	int channels = image.channels();

	for (int row = 0; row < image.rows; row++) {
		uchar* p = image.ptr<uchar>(row);
		for (int channel = 0; channel < channels; channel++)
			for (int col = channel; col < image.cols * channels; col += channels) {
				
				//	調整 alpah, beta, gamma 值
				double tmp = pow(p[col] / 255.0, gamma) * 255;
				tmp = tmp * alpah + beta;


				if (tmp < 0)
					p[col] = 0;
				else if (tmp > 255)
					p[col] = 255;
				else
					p[col] = tmp;
			}
	}

	imshow("after change", image);
	cvtColor(image, grayimg, CV_BGR2GRAY);
	getHist(grayimg, histimg);
	imshow("histimg after change", histimg);

	waitKey(0);

	return 0;
}