#pragma once

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

#include "CaptureThread.h"

using namespace std;
using namespace cv;

class CascadeThread {

private:
	CaptureThread *capture;
	mutex *queue_mutex;
	queue<Mat> *result_queue;
	CascadeClassifier cascade;
	string cascade_name = "haarcascade_frontalface_default.xml";
	thread *cascade_thread;
	bool keepRuning = false;
	void goCascade();

public:
	CascadeThread(CaptureThread &capture, queue<Mat> &result_queue, mutex &queue_mutex);
	~CascadeThread();
	void start_thread();
};