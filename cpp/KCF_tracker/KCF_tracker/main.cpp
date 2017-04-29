
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/tracking/tracking.hpp>

using namespace cv;
using namespace std;

int main() {
	String video = "..\\..\\..\\test_data\\faceocc2.webm";
	VideoCapture cap(video);


	// declares all required variables
	Rect2d roi;
	Mat frame;

	// create a tracker object
	Ptr<Tracker> tracker = Tracker::create("KCF");


	// get bounding box
	cap >> frame;
	roi = selectROI("tracker", frame);


	//quit if ROI was not selected
	if (roi.width == 0 || roi.height == 0)
		return 0;


	// initialize the tracker
	tracker->init(frame, roi);


	// perform the tracking process
	printf("Start the tracking process, press ESC to quit.\n");

	while(true) {

		cap >> frame;

		if (frame.empty())
			break;

		// update the tracking result
		tracker->update(frame, roi);

		// draw the tracked object
		rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);

		// show image with the tracked object

		imshow("tracker", frame);

		//quit on ESC button
		if (waitKey(1) == 27)
			break;
	}
	return 0;
}