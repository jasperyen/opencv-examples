#include <stdio.h>
#include <iostream>
#include <iomanip>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;


int main() {
	//	�̦h��ܤp�ƫ���
	cout << fixed << setprecision(2);

	String videoFile = "..\\..\\..\\test_data\\mog2_test.mp4";
	VideoCapture capture(videoFile);

	//	�ŧiMOG2�t��k
	Ptr<BackgroundSubtractor> pMOG2;
	Mat frame;
	Mat fgMaskMOG2;

	//	�����ҽk�j�p
	int blurSize = 17;
	//	�G�Ȥƻ֭�
	int threhold = 81;
	//	���ʭ��n��v
	double subRatio = 0.02;
	//	�I���ҫ��ǲ߳t�v
	double learningRate = -1;

	//	�إߺt��k, �]�w�Ѽ�
	pMOG2 = createBackgroundSubtractorMOG2(500, 128, true);

	if (!capture.isOpened()) {
		cout << "Unable to open cpature " << endl;
		exit(EXIT_FAILURE);
	}

	while (true) {
		//	�N�ثe���Ʀs�Jframe
		if (!capture.read(frame)) {
			cout << "Unable to read next frame." << endl;
			exit(EXIT_FAILURE);
		}

		imshow("Frame", frame);

		//	�z�LMOG2�B��, �N���G�s�JfgMaskMOG2
		pMOG2->apply(frame, fgMaskMOG2, learningRate);
		imshow("FG Mask MOG 2", fgMaskMOG2);

		//	�����ҽk
		GaussianBlur(fgMaskMOG2.clone(), fgMaskMOG2, Size(blurSize, blurSize), 0, 0);
		imshow("After blur", fgMaskMOG2);

		double count = 0;
		uchar *p;
		//	�G�ȤƹB��, �p�Ⲿ�ʹ���
		for (int i = 0; i < fgMaskMOG2.rows; i++) {
			p = fgMaskMOG2.ptr<uchar>(i);
			for (int j = 0; j < fgMaskMOG2.cols; j++) {
				if (p[j] > threhold) {
					p[j] = 255;
					count++;
				}
				else {
					p[j] = 0;
				}
			}
		}
		imshow("Result", fgMaskMOG2);

		//	�p�Ⲿ�ʤ�v
		double currRatio = count / (fgMaskMOG2.rows * fgMaskMOG2.cols);

		if (currRatio > subRatio) {
			cout << "�ثe���ʤ�v : " << currRatio * 100 << "%" << endl;
		}

		waitKey(30);
	}
	capture.release();

	return 0;
}