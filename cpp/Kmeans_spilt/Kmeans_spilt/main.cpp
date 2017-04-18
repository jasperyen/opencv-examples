
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

	String imageFile = "..\\..\\..\\test_data\\lena.png";

	src = imread(imageFile);

	imshow("input", src);
	int width = src.cols;
	int height = src.rows;
	int dims = src.channels();

	int sampleCount = width*height;
	int clusterCount = 3;
	Mat points(sampleCount, dims, CV_32F, Scalar(10));
	Mat labels;
	Mat centers(clusterCount, 1, points.type());

	int index = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			index = row*width + col;
			Vec3b rgb = src.at<Vec3b>(row, col);
			points.at<float>(index, 0) = static_cast<int>(rgb[0]);
			points.at<float>(index, 1) = static_cast<int>(rgb[1]);
			points.at<float>(index, 2) = static_cast<int>(rgb[2]);
		}
	}

	TermCriteria criteria = TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1);
	kmeans(points, clusterCount, labels, criteria, 3, KMEANS_PP_CENTERS, centers);

	Mat result = Mat::zeros(src.size(), CV_8UC3);
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			index = row*width + col;
			int label = labels.at<int>(index, 0);
			
			switch (label) {
				case 0 :
					result.at<Vec3b>(row, col)[0] = 255;
					result.at<Vec3b>(row, col)[1] = 0;
					result.at<Vec3b>(row, col)[2] = 0;
					break;

				case 1 :
					result.at<Vec3b>(row, col)[0] = 0;
					result.at<Vec3b>(row, col)[1] = 255;
					result.at<Vec3b>(row, col)[2] = 0;
					break;

				case 2 :
					result.at<Vec3b>(row, col)[0] = 0;
					result.at<Vec3b>(row, col)[1] = 0;
					result.at<Vec3b>(row, col)[2] = 255;
					break;
			}
			
		}
	}

	imshow("kmeans-demo", result);

	waitKey(0);

	return 0;
}