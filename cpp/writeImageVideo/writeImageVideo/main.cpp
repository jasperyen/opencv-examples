
#include <stdio.h>
#include <iostream>
#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	struct tm *T;
	time_t t;
	time(&t);


	vector<int> compression_params;
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return 0;
	}

	cout << "push \"i\" save as image !" << endl;
	cout << "push \"v\" save as video !" << endl;


	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);


	VideoWriter writer;
	int key = 0;
	while (true) {

		Mat frame;
		cap >> frame;

		imshow("Frame", frame);

		if (writer.isOpened())
			writer.write(frame);

		if (key == 'i') {
			T = localtime(&t);
			String filename = to_string(T->tm_mon + 1) + "-" + to_string(T->tm_mday)
				+ "-" + to_string(T->tm_hour) + "-" + to_string(T->tm_min) + "-" + to_string(T->tm_sec) + ".png";

			imwrite(filename, frame, compression_params);
			cout << "image saved : " << filename << endl;
		}
		else if (key == 'v') {
			if (!writer.isOpened()) {
				T = localtime(&t);
				String filename = to_string(T->tm_mon + 1) + "-" + to_string(T->tm_mday)
					+ "-" + to_string(T->tm_hour) + "-" + to_string(T->tm_min) + "-" + to_string(T->tm_sec) + ".avi";
				
				cout << "start recording : " << filename << endl;
				writer.open(filename, CV_FOURCC('D', 'I', 'V', 'X'), 25.0, Size(width, height), true);
			}
			else {
				cout << "stop recording, video saved!" << endl;
				writer.release();
			}
		}
		key = waitKey(1);
	}

	return 0;
}