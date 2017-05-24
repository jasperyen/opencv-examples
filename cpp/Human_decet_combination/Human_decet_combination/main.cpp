
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;



Mat computeHOGDescriptor(Mat roi) {
	Size winSize = Size(64, 128);
	Size blockSize = Size(16, 16);
	Size strideSize = Size(8, 8);
	Size cellSize = Size(8, 8);
	int bins = 9;
	//一個Cell有9個方向，共9維
	//一個Block有4個Cells
	//一個Window有32個Blocks
	HOGDescriptor hog(winSize, blockSize, strideSize, cellSize, bins);
	vector<float> descriptors;//HOG描述向量
	Size winShiftSize = Size(8, 8);//搜尋框移動位置
	Size paddingSize = Size(0, 0);//補滿邊界位置
	hog.compute(roi, descriptors, winShiftSize, paddingSize);//調用計算函數

	//Store HOG features in Hogfeat matrix
	Mat Hogfeat;
	Hogfeat.create(descriptors.size(), 1, CV_32FC1);
	for (int i = 0; i < descriptors.size(); i++){
		Hogfeat.at<float>(i, 0) = descriptors.at(i);
	}
	return Hogfeat;
}

double distanceHogfeature(Mat Hogfeat_1, Mat Hogfeat_2) {
	double distance = 0;
	for (int i = 0; i < Hogfeat_1.rows; i++) {
		distance += abs(Hogfeat_1.at<float>(i, 0) - Hogfeat_2.at<float>(i, 0));
	}
	//cout << distance << endl;
	return distance;
}

void convertToHSV(Mat src, Mat &dst) {
	Mat hsv;
	cvtColor(src, hsv, CV_BGR2HSV);
	
	dst = Mat::zeros(src.size(), CV_8U);
	for (int i = 0; i < dst.rows; i++) {
		uchar *hsv_ptr = hsv.ptr<uchar>(i);
		uchar *dst_ptr = dst.ptr<uchar>(i);
		for (int j = 0; j < dst.cols; j++) {
			dst_ptr[j] = hsv_ptr[j*3];
		}
	}

	imshow("dst", dst);
}

int main() {
	RNG rng(12345);

	String video = "..\\..\\..\\test_data\\human_detect.mp4";

	VideoCapture capture(0);

	capture.set(CAP_PROP_FRAME_WIDTH, 1280);
	capture.set(CAP_PROP_FRAME_HEIGHT, 720);
	Size scale_size(1280, 720);

	if (!capture.isOpened()) {
		cout << "Could not open VideoCapture" << endl;
		return -1;
	}


	CascadeClassifier cascade;
	String cascade_name = "haarcascades_xml\\haarcascade_frontalface_default.xml";
	//String cascade_name = "haarcascades_xml\\haarcascade_profileface.xml";

	if (!cascade.load(cascade_name)) {
		cout << "Failed to load cascade classifier" << endl;
		return -1;
	}

	vector<Mat> save_hog;
	vector<Scalar> save_color;
	//vector<Mat> save_roi;

	double x_scale = 1.5;
	double y_scale = 3.0;
	
	Mat frame, frame_gray;
	vector<Rect> rectFace, recBody;
	while (true) {
		double t = (double)getTickCount();

		capture >> frame;

		if (frame.empty())
			break;


		//resize(frame, frame, scale_size);

		//	灰階, 直方圖均等
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//	找人臉, 並存入rectFace
		cascade.detectMultiScale(
			frame_gray, rectFace, 1.125, 4, 0, Size(90, 90), Size(180, 180));

		recBody.clear();

		//	根據人臉, 放大找到人形, 並存入recBody
		for (size_t i = 0; i < rectFace.size(); i++) {
			double x = rectFace[i].x;
			x = x - rectFace[i].width * ((x_scale - 1) / 2.0);

			double y = rectFace[i].y + rectFace[i].height;

			double width = x_scale * rectFace[i].width;

			double height = y_scale * rectFace[i].height;


			if (x < 0) {
				//continue;
				width += x;
				x = 0;
			}

			if (x + width > scale_size.width)
				//continue;
				width = scale_size.width - x;
			if (y + height > scale_size.height)
				//continue;
				height = scale_size.height - y;

			cout << "x : " << x << ", y : " << y << ", width : " << width << ", height : " << height << endl;

			Rect rect(x, y, width, height);
			recBody.push_back(rect);
		}

		//	找最大人形
		Rect large_rect;
		int large_body = -1;
		for (size_t i = 0; i < recBody.size(); i++) {
			if (recBody[i].area() > large_body) {
				large_rect = recBody[i];
				large_body = recBody[i].area();
			}
		}

		
		if (large_body != -1) {
			imshow("bodyRoi", frame(large_rect));

			Mat bodyRoi = frame(large_rect).clone();
			
			//Mat hsv;
			convertToHSV(bodyRoi, bodyRoi);
			
			//cvtColor(bodyRoi, bodyRoi, CV_BGR2GRAY);
			resize(bodyRoi, bodyRoi, cvSize(64, 128));

			Mat now_hog = computeHOGDescriptor(bodyRoi);

			int match_people = -1;
			double distance_thres = 300.0;
			double min_match = DBL_MAX;

			for (int i = 0; i < save_hog.size(); i++) {
				double distance = distanceHogfeature(now_hog, save_hog[i]);

				if (distance < distance_thres && distance < min_match) {
					match_people = i;
					save_hog[i] = now_hog;
					
					//imshow("match before", save_roi[i]);
					//save_roi[i] = bodyRoi;
					//imshow("match after", save_roi[i]);
					cout << "update descriptors " << i << endl;
					
					rectangle(frame, large_rect, save_color[i], 3);
				}
			}

			if (match_people == -1) {
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				
				if (save_hog.size() > 16) {
					save_color.erase(save_color.begin());
					save_hog.erase(save_hog.begin());
				}

				save_color.push_back(color);
				save_hog.push_back(now_hog);
				//save_roi.push_back(bodyRoi);
				cout << "save new descriptors " << save_hog.size()-1 << endl;

				rectangle(frame, large_rect, color, 3);
			}

			
			
		}

		for (size_t i = 0; i < rectFace.size(); i++)
			rectangle(frame, rectFace[i], Scalar(0, 0, 255), 2);

		//for (size_t i = 0; i < recBody.size(); i++)
			//rectangle(frame, recBody[i], Scalar(255, 0, 0), 1);


		imshow("output", frame);

		t = 1 / (((double)getTickCount() - t) / getTickFrequency());
		cout << "Frame rate : " << t << endl;
		waitKey(1);
	}

	return 0;
}