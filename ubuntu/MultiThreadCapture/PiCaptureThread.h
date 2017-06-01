#pragma once

#include <stdio.h>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

#include <raspicam/raspicam_cv.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class PiCaptureThread {

private :
	raspicam::RaspiCam_Cv *capture;
	thread *cap_thread;
	mutex queue_mutex;
	queue<Mat> frame_queue;
	int max_queue_size = 5;
	bool capturing = false;
	void printLessCaptureSetting();
	void goCapture();

public :
	CaptureThread(raspicam::RaspiCam_Cv &capture);
	~CaptureThread();
	void startCapture();
	bool isCapturing();
	Size getFrameSize();
	bool getFrame(Mat &frame);
};
