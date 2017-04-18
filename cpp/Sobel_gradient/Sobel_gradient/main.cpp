#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;


int main() {
	Mat src;

	String imageFile = "..\\..\\..\\test_data\\railway.jpg";

	src = imread(imageFile, IMREAD_GRAYSCALE);
	imshow("input", src);


	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y, dst;

	Sobel(src, grad_x, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	Sobel(src, grad_y, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT);

	imshow("grad_x", grad_x);
	imshow("grad_y", grad_y);

	Mat angle;
	int degree = 90;
	int range = 1;

	phase(grad_x, grad_y, angle, true);

	Mat grad_deg(angle.rows, angle.cols, CV_8UC1, Scalar(0));

	int max = 0;
	for (int i = 0; i < angle.rows; i++) {
		float *ap = angle.ptr<float>(i);
		uchar *dp = grad_deg.ptr<uchar>(i);
		for (int j = 0; j < angle.cols; j++) {
			if ( ap[j] < degree + range && ap[j] > degree - range )
				dp[j] = 255;

			if (ap[j] < degree + range + 180 && ap[j] > degree - range + 180)
				dp[j] = 255;
		}
	}
	
	imshow("grad_deg", grad_deg);

	waitKey(0);

	return 0;
}