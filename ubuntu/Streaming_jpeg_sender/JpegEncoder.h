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

class JpegEncoder {

private :
	CaptureThread *capture;
	Size originalSize;
	Size scaleSize;
	vector<int> compression_params;
	mutex queue_mutex;
	queue<vector<unsigned char>> jpeg_queue;
	thread *encode_thread;
	int max_queue_size = 5;
	bool showCapture;
	bool encoding = false;
	void encodeJpegPackage(Mat&, vector<unsigned char>&);
	void goEncode();

public :
	JpegEncoder(CaptureThread&, const bool, const int, const float);
	~JpegEncoder();
	void startJpegEncode();
	bool getJpegPackage(vector<unsigned char>&);
	bool isEncoding();

};