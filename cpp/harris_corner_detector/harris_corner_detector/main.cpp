
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat src, src_gray;
int thresh = 150;

void draw_harris_corner(int, void*) {
	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros(src.size(), CV_32FC1);

	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;
	cornerHarris(src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());

	convertScaleAbs(dst_norm, dst_norm_scaled);

	for (int j = 0; j < dst_norm.rows; j++) {
		for (int i = 0; i < dst_norm.cols; i++) {
			if ((int)dst_norm.at<float>(j, i) > thresh) {
				circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
			}
		}
	}

	imshow("Corners detected", dst_norm_scaled);
}

int main() {
	String imageFile = "..\\..\\..\\test_data\\railway.jpg";
	src = imread(imageFile);

	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	imshow("src", src);
	createTrackbar("Threshold: ", "src", &thresh, 255, draw_harris_corner);
	//imshow("src", src);

	draw_harris_corner(0, 0);

	waitKey(0);
	return 0;
}