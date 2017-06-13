#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

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

using namespace std;
using namespace cv;

class CaptureThread {

private :
	VideoCapture *capture;
	thread *cap_thread;
	mutex queue_mutex;
	queue<Mat> frame_queue;
	int max_queue_size = 5;
	bool capturing = false;
	void printLessCaptureSetting();
	void goCapture();

public : 
	CaptureThread(VideoCapture &capture);
	~CaptureThread();
	void startCapture();
	bool isCapturing();
	Size getFrameSize();
	bool getFrame(Mat &frame);
};

#endif