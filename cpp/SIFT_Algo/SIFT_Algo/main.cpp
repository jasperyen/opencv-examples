
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>

using namespace cv;
using namespace std;

int main() {

	//載入圖像、顯示並轉化為灰度圖
	Mat trainImage = imread("..\\..\\..\\test_data\\lena.png");
	Mat trainImage_gray;

	imshow("trainImage", trainImage);

	cvtColor(trainImage, trainImage_gray, CV_BGR2GRAY);

	//檢測SIFT關鍵點、建立描述符
	Ptr<xfeatures2d::SIFT> Sift = xfeatures2d::SIFT::create();
	vector<KeyPoint> train_keyPoint;
	//xfeatures2d::SiftFeatureDetector featureDetector;
	//featureDetector.detect(trainImage_gray, train_keyPoint);
	Sift->detect(trainImage_gray, train_keyPoint);

	//顯示Sift運算結果
	Mat siftResult;
	drawKeypoints(trainImage_gray, train_keyPoint, siftResult);
	imshow("Sift Result", siftResult);

	Mat trainDescription;
	//xfeatures2d::SiftDescriptorExtractor featureExtractor;
	//featureExtractor.compute(trainImage_gray, train_keyPoint, trainDescription);
	Sift->compute(trainImage_gray, train_keyPoint, trainDescription);


	//進行描述符暴力比對
	BFMatcher matcher;
	vector<Mat> train_desc_collection(1, trainDescription);
	matcher.add(train_desc_collection);
	matcher.train();


	VideoCapture cap(0);


	while (true) {
		
		//記錄起始時間
		double time0 = static_cast<double>(getTickCount());
		
		Mat captureImage, captureImage_gray;
		
		cap >> captureImage;
		
		if (captureImage.empty())
			continue;

		//轉化圖像為灰階
		cvtColor(captureImage, captureImage_gray, CV_BGR2GRAY);

		//檢測SIFT關鍵點、建立描述符
		vector<KeyPoint> test_keyPoint;
		Mat testDescriptor;
		//featureDetector.detect(captureImage_gray, test_keyPoint);
		//featureExtractor.compute(captureImage_gray, test_keyPoint, testDescriptor);
		Sift->detect(captureImage_gray, test_keyPoint);
		Sift->compute(captureImage_gray, test_keyPoint, testDescriptor);

		//比對訓練和測試描述符
		vector<vector<DMatch> > matches;
		matcher.knnMatch(testDescriptor, matches, 2);

		//根據knnMatch得到優秀的比對點
		vector<DMatch> goodMatches;
		for (unsigned int i = 0; i < matches.size(); i++) {
			DMatch bestMatch = matches[i][0];
			DMatch betterMatch = matches[i][1];

			if (bestMatch.distance < 0.6 * betterMatch.distance)
				goodMatches.push_back(matches[i][0]);
		}

		//繪製並顯示視窗
		Mat dstImage;
		drawMatches(captureImage, test_keyPoint, trainImage, train_keyPoint, goodMatches, dstImage);
		imshow("matching frame", dstImage);

		cout << "現在禎數為：" << getTickFrequency() / (getTickCount() - time0) << endl;

		waitKey(1);
	}

	return 0;
}