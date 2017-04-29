
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main() {

	int blurSize = 12;

	Mat image = imread("..\\..\\..\\test_data\\bridge.jpg");
	int channels = image.channels();
	imshow("src", image);


	Mat dst_ptr = image.clone();

	//	pointer vision
	double t = (double)getTickCount();
	for (int i = 0; i < dst_ptr.rows; i += blurSize) {
		for (int j = 0; j < channels * dst_ptr.cols; j += blurSize * channels) {
			for (int c = 0; c < channels; c++) {
				int sum = 0;
				int count = 0;

				for (int ii = i; ii < blurSize + i && ii < dst_ptr.rows; ii++) {
					uchar* p = dst_ptr.ptr<uchar>(ii);
					for (int jj = j + c; jj < blurSize * channels + j + c && jj < dst_ptr.cols * channels; jj += channels) {
						sum += p[jj];
						count++;
					}
				}
				int avg = sum / count;

				for (int ii = i; ii < blurSize + i && ii < dst_ptr.rows; ii++) {
					uchar* p = dst_ptr.ptr<uchar>(ii);
					for (int jj = j + c; jj < blurSize * channels + j + c && jj < dst_ptr.cols * channels; jj += channels) {
						p[jj] = avg;
					}
				}
			}
		}
	}
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "Times passed in pointer vision " << t << endl;
	imshow("dst_ptr", dst_ptr);


	Mat dst_fly = image.clone();

	//	On-The-Fly vision
	t = (double)getTickCount();
	for (int i = 0; i < dst_fly.rows; i += blurSize) {
		for (int j = 0; j < dst_fly.cols; j += blurSize) {
			for (int c = 0; c < channels; c++) {
				int sum = 0;
				int count = 0;

				for (int ii = i; ii < blurSize + i && ii < dst_fly.rows; ii++) {
					for (int jj = j; jj < blurSize + j && jj < dst_fly.cols; jj++) {
						sum += dst_fly.at<Vec3b>(ii, jj)[c];
						count++;
					}
				}
				int avg = sum / count;

				for (int ii = i; ii < blurSize + i && ii < dst_fly.rows; ii++) {
					for (int jj = j; jj < blurSize + j && jj < dst_fly.cols; jj++) {
						dst_fly.at<Vec3b>(ii, jj)[c] = avg;
					}
				}
			}
		}
	}
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "Times passed in On-The-Fly vision: " << t << endl;
	imshow("dst_fly", dst_fly);


	waitKey(0);

	return 0;
}