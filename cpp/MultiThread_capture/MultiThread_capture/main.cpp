
#include <stdio.h>
#include <iostream>
#include <queue>
#include <thread>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

queue<Mat> frame_queue;
VideoCapture capture;


void captureThread() {
	Mat frame;

	double sec = 0;
	int frame_sum = 0;
	while (true) {
		double t = (double)getTickCount();

		capture >> frame;

		sec += ((double)getTickCount() - t) / getTickFrequency();
		frame_sum++;
		if (sec > 1.0) {
			cout << "Frame rate : " << frame_sum / sec << endl;
			sec = 0;
			frame_sum = 0;
		}

		if (frame.empty())
			break;

		frame_queue.push(frame);
	}

}

int main() {

	capture.open(0);

	if (!capture.isOpened())
		return -1;
	
	thread cap_thread(captureThread);

	while (true) {
		if (frame_queue.empty())
			continue;

		Mat frame = frame_queue.front();
		frame_queue.pop();

		imshow("frame", frame);
		waitKey(1);
	}

	return 0;
}