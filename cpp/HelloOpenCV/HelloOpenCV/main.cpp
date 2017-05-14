
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void conn(Mat src) {
	Mat dst;
	int labels = connectedComponents(src, dst, 8, CV_32S);

	Mat out(src.rows, src.cols, CV_8U, Scalar(0));


	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {

			int label = dst.at<int>(row, col);

			if (label < 5)
				out.at<uchar>(row, col) = 255;
		}
	}

	imshow("out", out);

	cout << labels << endl;
}


int main() {
	Mat src = imread("C:\\Users\\User\\Desktop\\ttt.png", IMREAD_GRAYSCALE);
	Mat dst;

	imshow("src", src);

	//dst = src;
	blur(src, dst, Size(3, 3));

	imshow("blur", dst);

	threshold(dst, dst, 110, 255, THRESH_BINARY_INV);
	
	imshow("threshold", dst);

	Mat tmp = dst.clone();


	//Mat element = getStructuringElement(MORPH_ELLIPSE, Size(11, 11));
	//morphologyEx(tmp, tmp, MORPH_OPEN, element);


	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(11, 11));
	erode(dst, dst, element);
	imshow("erode", dst);


	element = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
	dilate(dst, dst, element);
	imshow("dilate", dst);


	//conn(dst);


	
	Mat edge;
	
	GaussianBlur(dst, dst, Size(19, 19), 0, 0);
	Canny(dst, edge, 50, 150, 3);

	imshow("Canny", edge);
	
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(edge, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);


	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);
		cout << area << endl;

		//if (area < 10000 && area > 100)
			drawContours(edge, contours, i, Scalar(128), 2, 8, hierarchy);

	}
	imshow("drawContours", edge);

	waitKey(0);
	return 0;
}