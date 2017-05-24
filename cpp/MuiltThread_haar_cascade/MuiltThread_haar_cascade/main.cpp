#include <stdio.h>
#include <iostream>
#include <queue>
#include <thread>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

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
			cout << "Real frame rate : " << frame_sum / sec << endl;
			sec = 0;
			frame_sum = 0;
		}

		if (frame.empty())
			break;

		while (frame_queue.size() > 5)
			frame_queue.pop();

		frame_queue.push(frame);
	}

}

int main(int argc, char **argv) {

	if (argc > 1)
		capture.open(atoi(argv[1]));
	else
		capture.open(0);

	if (!capture.isOpened()) {
		cout << "Could not open VideoCapture" << endl;
		return -1;
	}

	CascadeClassifier cascade;
	String cascade_name = "haarcascade_frontalface_default.xml";

	if (!cascade.load(cascade_name)) {
		cout << "Failed to load cascade classifier" << endl;
		return -1;
	}


	if (argc > 3) {
		capture.set(CV_CAP_PROP_FRAME_WIDTH, atoi(argv[2]));
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, atoi(argv[3]));
	}

	thread cap_thread(captureThread);


	Mat frame, frame_gray;
	vector<Rect> rect;
	double sec = 0;
	int frame_sum = 0;
	while (true) {
		if (frame_queue.empty())
			continue;

		double t = (double)getTickCount();

		frame = frame_queue.front();
		frame_queue.pop();

		if (frame.empty())
			break;


		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		cascade.detectMultiScale(
			frame_gray, rect, 1.25, 2, 0, Size(50, 50), Size(100, 100));

		for (size_t i = 0; i < rect.size(); i++)
			rectangle(frame, rect[i], Scalar(0, 0, 255), 2);

		imshow("output", frame);


		sec += ((double)getTickCount() - t) / getTickFrequency();
		frame_sum++;
		if (sec > 1.0) {
			cout << "Calculate frame rate : " << frame_sum / sec << endl;
			sec = 0;
			frame_sum = 0;
		}
		waitKey(1);
	}

	return 0;
}