#include <stdio.h>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <raspicam/raspicam_cv.h>

#include "CaptureThread.h"
#include "CascadeThread.h"

using namespace cv;
using namespace std;


int main(int argc, char **argv) {

	mutex queue_mutex;
	queue<Mat> result_queue;
	int thread_count;
	bool show_result = false;

	//VideoCapture capture(0);
	//capture.set(CV_CAP_PROP_FPS, 60);
	//capture.set(CAP_PROP_FRAME_WIDTH, 640);
	//capture.set(CAP_PROP_FRAME_HEIGHT, 480);

	raspicam::RaspiCam_Cv capture;
	capture.set(CV_CAP_PROP_FPS, 60);
	capture.set(CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CAP_PROP_FRAME_HEIGHT, 480);
	capture.open();

	CaptureThread cap_thread(capture);

	if (argc > 1 && atoi(argv[1]) == 1) {
		show_result = true;
		namedWindow("result", WINDOW_AUTOSIZE);
	}

	if (argc > 2)
		thread_count = atoi(argv[2]);
	else
		thread_count = 1;

	vector<CascadeThread*> thread_vector;
	for (int i = 0; i < thread_count; i++) {
		CascadeThread *cas_thread = new CascadeThread(cap_thread, result_queue, queue_mutex);
		thread_vector.push_back(cas_thread);
	}

	cap_thread.startCapture();

	for (int i = 0; i < thread_count; i++) {
		thread_vector[i]->start_thread();
	}

	unique_lock<mutex> locker(queue_mutex, defer_lock);

	Mat frame;
	double sec = 0;
	int frame_sum = 0;

	while (true) {
		double t = (double)getTickCount();

		locker.lock();
		while (true) {
			if (!result_queue.empty()) {
				frame = result_queue.front();
				result_queue.pop();
				break;
			}
			else {
				locker.unlock();
				this_thread::sleep_for(chrono::duration<int, std::milli>(5));
				locker.lock();
			}
		}
		locker.unlock();

		sec += ((double)getTickCount() - t) / getTickFrequency();
		frame_sum++;
		if (sec > 1.0) {
			cout << "Cal frame rate : " << frame_sum / sec << endl;
			sec = 0;
			frame_sum = 0;
		}

		if (show_result) {
			imshow("result", frame);
			waitKey(1);
		}
	}

	return 0;
}
