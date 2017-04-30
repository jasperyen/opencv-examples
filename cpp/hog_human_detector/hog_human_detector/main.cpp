
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main() {

	String video = "..\\..\\..\\test_data\\human_detect.mp4";

	VideoCapture capture(video);

	if (!capture.isOpened()) {
		cout << "Could not open VideoCapture" << endl;
		return -1;
	}


	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	
	Mat frame;
	vector<Rect> rect;
	while (true) {
		double t = (double)getTickCount();

		capture >> frame;

		if (frame.empty())
			break;
		

		resize(frame, frame, Size(960, 540));

		hog.detectMultiScale(frame, rect, 1, Size(8, 8), Size(8, 8), 1.25, 0);



		for (size_t i = 0; i < rect.size(); i++)
			rectangle(frame, rect[i], Scalar(0, 0, 255), 2);

		imshow("output", frame);


		t = 1 / (((double)getTickCount() - t) / getTickFrequency());
		cout << "Frame rate : " << t << endl;
		waitKey(1);
	}

	return 0;
}