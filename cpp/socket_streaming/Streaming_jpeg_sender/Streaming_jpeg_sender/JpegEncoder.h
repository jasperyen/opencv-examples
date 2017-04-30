#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class JpegEncoder {

private :
	VideoCapture capture;
	int jpegQuality;
	float scale;
	bool showVideo;

public :
	JpegEncoder(const bool, const int, const float);
	JpegEncoder(const int, const bool, const int, const float);
	JpegEncoder(const string, const bool, const int, const float);
	void initialization(const bool, const int, const float);
	const vector<unsigned char> GetJpegFromCapture();
	const vector<unsigned char> GetJpegPackageFromCapture();
	bool isCapturing();

};