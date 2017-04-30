
#include "JpegEncoder.h"

using namespace std;
using namespace cv;

JpegEncoder::JpegEncoder(const bool show, const int quality, const float s) {
	capture.open(0);
	if (!capture.isOpened())
		return;
	initialization(show, quality, s);
}

JpegEncoder::JpegEncoder(const int c, const bool show, const int quality, const float s) {
	capture.open(c);
	if (!capture.isOpened())
		return;
	initialization(show, quality, s);
}

JpegEncoder::JpegEncoder(const string str, const bool show, const int quality, const float s) {
	capture.open(str);
	if (!capture.isOpened())
		return;
	initialization(show, quality, s);
}

void JpegEncoder::initialization(const bool show, const int quality, const float s) {
	if (showVideo)
		namedWindow("Sender capture", WINDOW_AUTOSIZE);

	showVideo = show;

	jpegQuality = quality;

	scale = s;
}

const vector<unsigned char> JpegEncoder::GetJpegFromCapture() {
	vector<unsigned char> data;
	Mat frame;
	
	capture >> frame;

	if (frame.empty())
		return data;

	if ( showVideo )
		imshow("Sender capture", frame);

	if (scale < 1.0) {
		Size original_size = frame.size();
		Size new_size((int)(original_size.width * scale), (int)(original_size.height * scale));
		resize(frame, frame, new_size);
	}

	vector<int> compression_params = {
		IMWRITE_JPEG_QUALITY, jpegQuality
	};

	imencode(".jpg", frame, data, compression_params);
	
	
	/*
	unsigned char c = 0;
	for (int i = 0; i < 256; i++, c++) {
		data.push_back(c);
	}
	*/
	return data;
}

const vector<unsigned char> JpegEncoder::GetJpegPackageFromCapture() {

	vector<unsigned char> data = JpegEncoder::GetJpegFromCapture();

	if (data.empty())
		return data;

	for (int i = 0; i < sizeof(unsigned int); i++)
		data.insert(data.begin(), 0);
	

	unsigned int i[1];
	i[0] = data.size();
	unsigned char *ptr = data.data();

	memcpy(ptr, i, sizeof(unsigned int));

	return data;
}

bool JpegEncoder::isCapturing() {
	return capture.isOpened();
}
