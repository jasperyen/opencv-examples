
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main() {
	String video = "..\\..\\..\\test_data\\faceocc2.webm";

	VideoCapture capture(video);
	if (!capture.isOpened()) {
		cout << "Could not open VideoCapture" << endl;
		return -1;
	}


	CascadeClassifier faceCascade;
	String faceCascade_name = "haarcascades_xml\\haarcascade_frontalface_default.xml";

	if (!faceCascade.load(faceCascade_name)) {
		cout << "Failed to load cascade classifier" << endl;
		return -1;
	}

	Mat frame;
	while (true) {
		capture >> frame;

		if (frame.empty())
			break;

		vector<Rect> faces;


		faceCascade.detectMultiScale( 
			frame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));


		for (size_t i = 0; i < faces.size(); i++)
			rectangle(frame, faces[i], Scalar(0, 0, 255), 2);

		imshow("output", frame);


		waitKey(1);
	}

	return 0;
}