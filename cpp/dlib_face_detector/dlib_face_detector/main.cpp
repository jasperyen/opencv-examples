
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <dlib/image_processing/object_detector.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>
#include <dlib/gui_widgets.h>


using namespace std;
using namespace dlib;


int main(int argc, char** argv) {

	string video = "..\\..\\..\\test_data\\human_detect.mp4";

	cv::VideoCapture capture(video);


	if (!capture.isOpened()) {
		cout << "Could not open VideoCapture" << endl;
		return 0;
	}


	frontal_face_detector detector = get_frontal_face_detector();
	image_window fhogwin(draw_fhog(detector), "Learned fHOG detector");


	image_window win;
	win.set_title("Detect windows");
	
	cv::Mat frame, frame_gray;
	cv_image<unsigned char> dlibImage;

	std::vector<rectangle> face_dets;

	while (true) {
		capture >> frame;

		if (frame.empty())
			break;


		cv::cvtColor(frame, frame_gray, CV_RGB2GRAY);
		cv::equalizeHist(frame_gray, frame_gray);

		dlibImage = cv_image<unsigned char>(frame_gray);


		double t = (double)cv::getTickCount();

		face_dets = detector(dlibImage);

		t = 1 / (((double)cv::getTickCount() - t) / cv::getTickFrequency());
		cout << "Frame rate : " << t << endl;


		win.clear_overlay();
		win.set_image(dlibImage);
		win.add_overlay(face_dets, rgb_pixel(255, 0, 0));
	}

}


