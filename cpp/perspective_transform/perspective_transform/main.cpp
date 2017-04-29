
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

vector<Point2f> vpoint;
Mat src, showimg;


void draw_transform() {
	showimg = src.clone();

	for (int i = 0; i < vpoint.size(); i++) {
		cout << "(x, y) : " << vpoint.at(i).x << ", " << vpoint.at(i).y << endl;
		circle(showimg, vpoint.at(i), 5, Scalar(0, 0, 255), -1);
	}

	imshow("showimg", showimg);

	if (vpoint.size() < 4)
		return;


	Mat result;

	// Input Quadilateral or Image plane coordinates
	Point2f inputQuad[4];
	// Output Quadilateral or World plane coordinates
	Point2f outputQuad[4];


	inputQuad[0] = vpoint.at(0);
	inputQuad[1] = vpoint.at(1);
	inputQuad[2] = vpoint.at(2);
	inputQuad[3] = vpoint.at(3);


	outputQuad[0] = Point2f(0, 0);
	outputQuad[1] = Point2f(showimg.cols, 0);
	outputQuad[2] = Point2f(showimg.cols, showimg.rows);
	outputQuad[3] = Point2f(0, showimg.rows);

	// Get the Perspective Transform Matrix i.e. lambda 
	Mat lambda = getPerspectiveTransform(inputQuad, outputQuad);
	// Apply the Perspective Transform just found to the src image
	warpPerspective(showimg, result, lambda, showimg.size());

	//Display input and output
	imshow("result", result);
}


void onMouse(int Event, int x, int y, int flags, void* param) {
	switch (Event) {
		case CV_EVENT_LBUTTONUP: {
			cout << "CV_EVENT_LBUTTONUP : (x, y) : " << x << ", " << y << endl;

			if (vpoint.size() < 4) {
				vpoint.push_back(Point(x, y));
				draw_transform();
			}
			
			break;
		}
		case CV_EVENT_RBUTTONUP: {
			cout << "CV_EVENT_RBUTTONUP : (x, y) : " << x << ", " << y << endl;
			vpoint.clear();
			destroyWindow("result");

			draw_transform();
			break;
		}

	}
}

int main() {
	String imageFile = "..\\..\\..\\test_data\\bridge.jpg";
	src = imread(imageFile);
	showimg = src.clone();

	imshow("showimg", showimg);

	setMouseCallback("showimg", onMouse, NULL);
	waitKey(0);

	return 0;
}