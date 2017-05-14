
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main() {

	String video = "..\\..\\..\\test_data\\human_detect.mp4";

	VideoCapture capture(video);

	//capture.set(CAP_PROP_FRAME_WIDTH, 1280);
	//capture.set(CAP_PROP_FRAME_HEIGHT, 720);
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

	double x_scale = 2.0;
	double y_scale = 4.0;
	
	Mat frame, frame_gray;
	vector<Rect> rectFace, recBody;
	while (true) {
		double t = (double)getTickCount();

		capture >> frame;

		if (frame.empty())
			break;


		resize(frame, frame, scale_size);

		//	�Ƕ�, ����ϧ���
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//	��H�y, �æs�JrectFace
		cascade.detectMultiScale(
			frame_gray, rectFace, 1.25, 2, 0, Size(25, 25), Size(50, 50));

		recBody.clear();

		//	�ھڤH�y, ��j���H��, �æs�JrecBody
		for (size_t i = 0; i < rectFace.size(); i++) {
			double x = rectFace[i].x;
			x = x - rectFace[i].width * ((x_scale - 1) / 2.0);

			double y = rectFace[i].y + rectFace[i].height;

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

		//	��̤j�H��
		Rect large_rect;
		int large_body = -1;
		for (size_t i = 0; i < recBody.size(); i++) {
			if (recBody[i].area() > large_body) {
				large_rect = recBody[i];
				large_body = recBody[i].area();
			}
		}

		if (large_body != -1) {
			Mat bodyRoi = frame(large_rect).clone();

			resize(bodyRoi, bodyRoi, cvSize(60, 120));

			Size winSize = Size(60, 60);
			Size blockSize = Size(20, 20);
			Size strideSize = Size(10, 10);
			Size cellSize = Size(5, 5);
			int bins = 9;
			//�@��Cell��9�Ӥ�V�A�@9��
			//�@��Block��16��Cells((20/5)*(20/5))�A�@144��
			//�@��Window��25��Blocks(((60-20)/10+1)*((60-20)/10+1))�A�@3600��
			HOGDescriptor *hog = new  HOGDescriptor(winSize, blockSize, strideSize, cellSize, bins);
			vector< float >descriptors;//HOG�y�z�V�q
			Size winShiftSize = cvSize(10, 10);//�j�M�ز��ʦ�m
			Size paddingSize = cvSize(0, 0);//�ɺ���ɦ�m
			hog->compute(bodyRoi, descriptors, winShiftSize, paddingSize);//�եέp����  
			cout << descriptors.size() << endl;

			
			imshow("bodyRoi", frame(large_rect));
			imshow("result", bodyRoi);
		}

		for (size_t i = 0; i < rectFace.size(); i++)
			rectangle(frame, rectFace[i], Scalar(0, 0, 255), 2);

		for (size_t i = 0; i < recBody.size(); i++)
			rectangle(frame, recBody[i], Scalar(255, 0, 0), 2);


		imshow("output", frame);

		t = 1 / (((double)getTickCount() - t) / getTickFrequency());
		cout << "Frame rate : " << t << endl;
		waitKey(1);
	}

	return 0;
}