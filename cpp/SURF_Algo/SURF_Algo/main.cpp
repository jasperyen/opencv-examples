
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>

using namespace cv;
using namespace std;

int main() {

	//���J�Ϲ��B��ܨ���Ƭ��ǫ׹�
	Mat trainImage = imread("..\\..\\..\\test_data\\lena.png");
	Mat trainImage_gray;

	imshow("trainImage", trainImage);

	cvtColor(trainImage, trainImage_gray, CV_BGR2GRAY);

	//�إ�SURF�t��k����B�]�whessian�֭Ȭ�80
	Ptr<xfeatures2d::SURF> Surf = xfeatures2d::SURF::create(80);

	//�˴�SURF�����I
	vector<KeyPoint> train_keyPoint;
	Surf->detect(trainImage_gray, train_keyPoint);

	//���SURF�˴����G
	Mat surfResult;
	drawKeypoints(trainImage_gray, train_keyPoint, surfResult);
	imshow("Surf Result", surfResult);

	//�إ�SURF�y�z��
	Mat trainDescription;
	Surf->compute(trainImage_gray, train_keyPoint, trainDescription);


	//�i��y�z��FLANN���V�m
	FlannBasedMatcher matcher;
	vector<Mat> train_desc_collection(1, trainDescription);
	matcher.add(train_desc_collection);
	matcher.train();


	VideoCapture cap(0);


	while (true) {

		//�O���_�l�ɶ�
		double time0 = static_cast<double>(getTickCount());

		Mat captureImage, captureImage_gray;

		cap >> captureImage;

		if (captureImage.empty())
			continue;

		//��ƹϹ����Ƕ�
		cvtColor(captureImage, captureImage_gray, CV_BGR2GRAY);

		//�˴�SURF�����I�B�إߴy�z��
		vector<KeyPoint> test_keyPoint;
		Mat testDescriptor;
		Surf->detect(captureImage_gray, test_keyPoint);
		Surf->compute(captureImage_gray, test_keyPoint, testDescriptor);

		//���V�m�M���մy�z��
		vector<vector<DMatch> > matches;
		matcher.knnMatch(testDescriptor, matches, 2);

		//�ھ�knnMatch�o���u�q������I
		vector<DMatch> goodMatches;
		for (unsigned int i = 0; i < matches.size(); i++) {
			DMatch bestMatch = matches[i][0];
			DMatch betterMatch = matches[i][1];

			if (bestMatch.distance < 0.6 * betterMatch.distance)
				goodMatches.push_back(matches[i][0]);
		}

		//ø�s����ܵ���
		Mat dstImage;
		drawMatches(captureImage, test_keyPoint, trainImage, train_keyPoint, goodMatches, dstImage);
		imshow("matching frame", dstImage);


		cout << "�{�b�ռƬ��G" << getTickFrequency() / (getTickCount() - time0) << endl;
		waitKey(1);
	}

	return 0;
}