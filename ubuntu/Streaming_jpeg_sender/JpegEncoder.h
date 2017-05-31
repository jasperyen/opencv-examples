#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "CaptureThread.h"

using namespace std;
using namespace cv;

class JpegEncoder {

private :
	CaptureThread *capture;
	Size originalSize;
	Size scaleSize;
	vector<int> compression_params;
	bool showCapture;
	bool getJpegData(vector<unsigned char>&);

public :
	JpegEncoder(CaptureThread&, const bool, const int, const float);
	bool getJpegPackage(vector<unsigned char>&);

};