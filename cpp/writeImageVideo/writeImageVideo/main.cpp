
#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {

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
			imwrite("out.png", frame, compression_params);
			cout << "image saved!" << endl;
		}
		else if (key == 'v') {
			if (!writer.isOpened()) {
				cout << "start recording!" << endl;

				writer.open("out.avi", CV_FOURCC('D', 'I', 'V', 'X'), 25.0, Size(width, height), true);
			}
			else {
				//isRecoding = false;
				cout << "stop recording, video saved!" << endl;
				writer.release();
			}
		}
		key = waitKey(1);
	}

	return 0;
}