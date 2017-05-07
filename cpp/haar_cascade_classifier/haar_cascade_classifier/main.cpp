
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main() {
	
	String video = "..\\..\\..\\test_data\\human_detect.mp4";
	//String video = "..\\..\\..\\test_data\\faceocc2.webm";

	VideoCapture capture(video);

	if (!capture.isOpened()) {
		cout << "Could not open VideoCapture" << endl;
		return -1;
	}


	CascadeClassifier cascade;
	//String cascade_name = "haarcascades_xml\\haarcascade_frontalface_default.xml";
	String cascade_name = "haarcascades_xml\\haarcascade_fullbody.xml";


	if (!cascade.load(cascade_name)) {
		cout << "Failed to load cascade classifier" << endl;
		return -1;
	}

	Mat frame, frame_gray;
	vector<Rect> rect;
	while (true) {
		double t = (double)getTickCount();

		capture >> frame;

		if (frame.empty())
			break;


		resize(frame, frame, Size(1280, 720));
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);
		
		/*
			image		:	Matrix of the type CV_8U containing an image where objects are detected.
			objects		:	Vector of rectangles where each rectangle contains the detected object, the rectangles may be partially outside the original image.
			
			scaleFactor	:	Parameter specifying how much the image size is reduced at each image scale.
							每次掃描放大倍率

			minNeighbors:	Parameter specifying how many neighbors each candidate rectangle should have to retain it.
							構成目標最小相鄰個數

			flags		:	It is not used for a new cascade.
			minSize		:	Minimum possible object size. Objects smaller than that are ignored.
			maxSize		:	Maximum possible object size. Objects larger than that are ignored. If maxSize == minSize model is evaluated on single scale.
		*/

		cascade.detectMultiScale(
			frame_gray, rect, 1.05, 2, 0, Size(80, 160), Size(300, 600));



		for (size_t i = 0; i < rect.size(); i++)
			rectangle(frame, rect[i], Scalar(0, 0, 255), 2);

		imshow("output", frame);


		t = 1 / (((double)getTickCount() - t) / getTickFrequency());
		cout << "Frame rate : " << t << endl;
		waitKey(1);
	}

	return 0;
}