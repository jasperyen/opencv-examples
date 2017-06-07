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

#include <raspicam/raspicam_cv.h>

using namespace std;
using namespace cv;

class CaptureThread {

private :
	enum CAPTURE_SPEC {VIDEOCAPTURE, RASPICAM};
	CAPTURE_SPEC spec;
	VideoCapture *videocapture;
	raspicam::RaspiCam_Cv *raspicam_cv;
	thread *cap_thread;
	mutex queue_mutex;
	queue<Mat> frame_queue;
	int max_queue_size = 5;
	bool capturing = false;
	void printLessCaptureSetting();
	void goCapture();

public :
	CaptureThread(VideoCapture &capture);
	CaptureThread(raspicam::RaspiCam_Cv &capture);
	~CaptureThread();
	void startCapture();
	bool isCapturing();
	Size getFrameSize();
	bool getFrame(Mat &frame);
};
