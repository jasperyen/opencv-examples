
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


	CascadeClassifier cascade;
	String cascade_name = "haarcascades_xml\\haarcascade_frontalface_default.xml";
	//String cascade_name = "haarcascades_xml\\haarcascade_frontalface_alt_tree.xml";

	if (!cascade.load(cascade_name)) {
		cout << "Failed to load cascade classifier" << endl;
		return -1;
	}



	Size scale_size(1280, 720);

	double x_scale = 3.0;
	double y_scale = 9.0;


	Mat frame, frame_gray, fgMask, bgMask;
	vector<Rect> rectFace, recBody;
	while (true) {
		double t = (double)getTickCount();

		capture >> frame;

		if (frame.empty())
			break;

		
		resize(frame, frame, scale_size);

		//	灰階, 直方圖均等
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//	找人臉, 並存入rectFace
		cascade.detectMultiScale(
			frame_gray, rectFace, 1.18, 2, 0, Size(15, 15), Size(45, 45));

		recBody.clear();

		//	根據人臉, 放大找到人形, 並存入recBody
		for (size_t i = 0; i < rectFace.size(); i++) {
			double x = rectFace[i].x;
			x = x - rectFace[i].width * ((x_scale - 1) / 2.0);
			
			double y = rectFace[i].y;

			double width = x_scale * rectFace[i].width;

			double height = y_scale * rectFace[i].height;


			if (x < 0) {
				width += x; 
				x = 0;
			}
			if (x + width > scale_size.width)
				width = scale_size.width - x;
			if (y + height > scale_size.height)
				height = scale_size.height - y;

			cout << "x : " << x << ", y : " << y << ", width : " << width << ", height : " << height << endl;

			Rect rect(x, y, width, height);
			recBody.push_back(rect);
		}

		//	找最大人形
		Rect large_rect;
		int large_body = 0;
		for (size_t i = 0; i < recBody.size(); i++) {
			if (recBody[i].area() > large_body) {
				large_rect = recBody[i];
				large_body = recBody[i].area();
			}
		}

		if (large_body != 0) {
			Mat bodyRoi = frame_gray(large_rect).clone();

			Mat edge;
			blur(bodyRoi, bodyRoi, Size(7, 7));
			Canny(bodyRoi, edge, 50, 150, 3);


			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;

			findContours(edge, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

			//cout << contours.size() << endl;

			if (contours.size() == 0)
				continue;

			int large_index = 0;
			double large_area = 0;
			for (int i = 0; i < contours.size(); i++) {
				double area = contourArea(contours[i]);
				if (area > large_area)
					large_index = i;
			}

			//for (int i = 0; i < contours.size(); i++) 
			drawContours(edge, contours, large_index, Scalar(255), 2, 8, hierarchy);

			imshow("edge", edge);
			imshow("bodyRoi", frame(large_rect));
		}

		for (size_t i = 0; i < rectFace.size(); i++)
			rectangle(frame, rectFace[i], Scalar(0, 0, 255), 2);

		for (size_t i = 0; i < recBody.size(); i++)
			rectangle(frame, recBody[i], Scalar(255, 0, 0), 2);


		imshow("output", frame);

		/*
		pSub->apply(frame, fgMask, -1);
		imshow("fgMask", fgMask);

		pSub->getBackgroundImage(bgMask);
		imshow("bgMask", bgMask);
		*/

		t = 1 / (((double)getTickCount() - t) / getTickFrequency());
		cout << "Frame rate : " << t << endl;
		waitKey(1);
	}

	return 0;
}