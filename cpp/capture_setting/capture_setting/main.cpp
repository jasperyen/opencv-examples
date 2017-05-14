
#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void printAllCaptureSetting(VideoCapture capture) {

	cout << "All VideoCapture Properties" << endl << endl;

	cout << "CAP_PROP_POS_MSEC" << "\t\t" << capture.get(CAP_PROP_POS_MSEC) << endl;
	cout << "CAP_PROP_POS_FRAMES" << "\t\t" << capture.get(CAP_PROP_POS_FRAMES) << endl;
	cout << "CAP_PROP_POS_AVI_RATIO" << "\t\t" << capture.get(CAP_PROP_POS_AVI_RATIO) << endl;
	cout << "CAP_PROP_FRAME_WIDTH" << "\t\t" << capture.get(CAP_PROP_FRAME_WIDTH) << endl;
	cout << "CAP_PROP_FRAME_HEIGHT" << "\t\t" << capture.get(CAP_PROP_FRAME_HEIGHT) << endl;
	cout << "CAP_PROP_FPS" << "\t\t" << capture.get(CAP_PROP_FPS) << endl;
	cout << "CAP_PROP_FOURCC" << "\t\t" << capture.get(CAP_PROP_FOURCC) << endl;
	cout << "CAP_PROP_FRAME_COUNT" << "\t\t" << capture.get(CAP_PROP_FRAME_COUNT) << endl;
	cout << "CAP_PROP_FORMAT" << "\t\t" << capture.get(CAP_PROP_FORMAT) << endl;
	cout << "CAP_PROP_MODE" << "\t\t" << capture.get(CAP_PROP_MODE) << endl;
	cout << "CAP_PROP_BRIGHTNESS" << "\t\t" << capture.get(CAP_PROP_BRIGHTNESS) << endl;
	cout << "CAP_PROP_CONTRAST" << "\t\t" << capture.get(CAP_PROP_CONTRAST) << endl;
	cout << "CAP_PROP_SATURATION" << "\t\t" << capture.get(CAP_PROP_SATURATION) << endl;
	cout << "CAP_PROP_HUE" << "\t\t" << capture.get(CAP_PROP_HUE) << endl;
	cout << "CAP_PROP_GAIN" << "\t\t" << capture.get(CAP_PROP_GAIN) << endl;
	cout << "CAP_PROP_EXPOSURE" << "\t\t" << capture.get(CAP_PROP_EXPOSURE) << endl;
	cout << "CAP_PROP_CONVERT_RGB" << "\t\t" << capture.get(CAP_PROP_CONVERT_RGB) << endl;
	cout << "CAP_PROP_WHITE_BALANCE_BLUE_U" << "\t\t" << capture.get(CAP_PROP_WHITE_BALANCE_BLUE_U) << endl;
	cout << "CAP_PROP_RECTIFICATION" << "\t\t" << capture.get(CAP_PROP_RECTIFICATION) << endl;
	cout << "CAP_PROP_MONOCHROME" << "\t\t" << capture.get(CAP_PROP_MONOCHROME) << endl;
	cout << "CAP_PROP_SHARPNESS" << "\t\t" << capture.get(CAP_PROP_SHARPNESS) << endl;
	cout << "CAP_PROP_AUTO_EXPOSURE" << "\t\t" << capture.get(CAP_PROP_AUTO_EXPOSURE) << endl;
	cout << "CAP_PROP_GAMMA" << "\t\t" << capture.get(CAP_PROP_GAMMA) << endl;
	cout << "CAP_PROP_TEMPERATURE" << "\t\t" << capture.get(CAP_PROP_TEMPERATURE) << endl;
	cout << "CAP_PROP_TRIGGER" << "\t\t" << capture.get(CAP_PROP_TRIGGER) << endl;
	cout << "CAP_PROP_TRIGGER_DELAY" << "\t\t" << capture.get(CAP_PROP_TRIGGER_DELAY) << endl;
	cout << "CAP_PROP_WHITE_BALANCE_RED_V" << "\t\t" << capture.get(CAP_PROP_WHITE_BALANCE_RED_V) << endl;
	cout << "CAP_PROP_ZOOM" << "\t\t" << capture.get(CAP_PROP_ZOOM) << endl;
	cout << "CAP_PROP_FOCUS" << "\t\t" << capture.get(CAP_PROP_FOCUS) << endl;
	cout << "CAP_PROP_GUID" << "\t\t" << capture.get(CAP_PROP_GUID) << endl;
	cout << "CAP_PROP_ISO_SPEED" << "\t\t" << capture.get(CAP_PROP_ISO_SPEED) << endl;
	cout << "CAP_PROP_BACKLIGHT" << "\t\t" << capture.get(CAP_PROP_BACKLIGHT) << endl;
	cout << "CAP_PROP_PAN" << "\t\t" << capture.get(CAP_PROP_PAN) << endl;
	cout << "CAP_PROP_TILT" << "\t\t" << capture.get(CAP_PROP_TILT) << endl;
	cout << "CAP_PROP_ROLL" << "\t\t" << capture.get(CAP_PROP_ROLL) << endl;
	cout << "CAP_PROP_IRIS" << "\t\t" << capture.get(CAP_PROP_IRIS) << endl;
	cout << "CAP_PROP_SETTINGS" << "\t\t" << capture.get(CAP_PROP_SETTINGS) << endl;
	cout << "CAP_PROP_BUFFERSIZE" << "\t\t" << capture.get(CAP_PROP_BUFFERSIZE) << endl;
	cout << "CAP_PROP_AUTOFOCUS" << "\t\t" << capture.get(CAP_PROP_AUTOFOCUS) << endl;
}


void printLessCaptureSetting(VideoCapture capture) {

	cout << endl << "Less VideoCapture Properties" << endl << endl;

	cout << "CAP_PROP_FRAME_WIDTH" << "\t\t" << capture.get(CAP_PROP_FRAME_WIDTH) << endl << endl;
	cout << "CAP_PROP_FRAME_HEIGHT" << "\t\t" << capture.get(CAP_PROP_FRAME_HEIGHT) << endl << endl;
	cout << "CAP_PROP_BRIGHTNESS" << "\t\t" << capture.get(CAP_PROP_BRIGHTNESS) << endl << endl;
	cout << "CAP_PROP_CONTRAST" << "\t\t" << capture.get(CAP_PROP_CONTRAST) << endl << endl;
	cout << "CAP_PROP_SATURATION" << "\t\t" << capture.get(CAP_PROP_SATURATION) << endl << endl;
	cout << "CAP_PROP_GAIN" << "\t\t" << capture.get(CAP_PROP_GAIN) << endl << endl;
	cout << "CAP_PROP_EXPOSURE" << "\t\t" << capture.get(CAP_PROP_EXPOSURE) << endl << endl;
	cout << "CAP_PROP_WHITE_BALANCE_BLUE_U" << "\t\t" << capture.get(CAP_PROP_WHITE_BALANCE_BLUE_U) << endl << endl;
	cout << "CAP_PROP_SHARPNESS" << "\t\t" << capture.get(CAP_PROP_SHARPNESS) << endl << endl;
	cout << "CAP_PROP_FOCUS" << "\t\t" << capture.get(CAP_PROP_FOCUS) << endl << endl;
	cout << "CAP_PROP_BACKLIGHT" << "\t\t" << capture.get(CAP_PROP_BACKLIGHT) << endl << endl;
}

void settingC905(VideoCapture capture) {

	capture.set(CAP_PROP_FRAME_WIDTH, 1280);
	capture.set(CAP_PROP_FRAME_HEIGHT, 800);
	capture.set(CAP_PROP_GAIN, 255);
	capture.set(CAP_PROP_EXPOSURE, -5);
	capture.set(CAP_PROP_WHITE_BALANCE_BLUE_U, 5000);

}

int main() {

	cout << fixed;
	cout.precision(2);

	VideoCapture capture;
	Mat frame;


	capture.open(0);

	if (!capture.isOpened())
		return 0;

	//settingC905(capture);

	//printAllCaptureSetting(capture);
	
	//capture.set(CAP_PROP_FRAME_WIDTH, 1280);
	//capture.set(CAP_PROP_FRAME_HEIGHT, 720);
	
	printLessCaptureSetting(capture);
	//printAllCaptureSetting(capture);

	//capture.set(CAP_PROP_SETTINGS, 1);

	double sec = 0;
	int frame_sum = 0;
	while (1) {
		double t = (double)getTickCount();

		capture >> frame;

		sec += ((double)getTickCount() - t) / getTickFrequency();
		frame_sum++;
		if (sec > 1.0) {
			cout << "Frame rate : " << frame_sum / sec << endl;
			sec = 0;
			frame_sum = 0;
		}

		if (frame.empty())
			break;

		
		//imshow("Display", frame);
		//waitKey(1);
	}

	return 0;
}