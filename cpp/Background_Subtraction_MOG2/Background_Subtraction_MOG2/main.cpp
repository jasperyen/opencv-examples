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
	//	最多顯示小數後兩位
	cout << fixed << setprecision(2);

	String videoFile = "..\\..\\..\\test_data\\mog2_test.mp4";
	VideoCapture capture(videoFile);

	//	宣告MOG2演算法
	Ptr<BackgroundSubtractor> pMOG2;
	Mat frame;
	Mat fgMaskMOG2;

	//	高斯模糊大小
	int blurSize = 17;
	//	二值化閥值
	int threhold = 81;
	//	移動面積比率
	double subRatio = 0.02;
	//	背景模型學習速率
	double learningRate = -1;

	//	建立演算法, 設定參數
	pMOG2 = createBackgroundSubtractorMOG2(500, 128, true);

	if (!capture.isOpened()) {
		cout << "Unable to open cpature " << endl;
		exit(EXIT_FAILURE);
	}

	while (true) {
		//	將目前偵數存入frame
		if (!capture.read(frame)) {
			cout << "Unable to read next frame." << endl;
			exit(EXIT_FAILURE);
		}

		imshow("Frame", frame);

		//	透過MOG2運算, 將結果存入fgMaskMOG2
		pMOG2->apply(frame, fgMaskMOG2, learningRate);
		imshow("FG Mask MOG 2", fgMaskMOG2);

		//	高斯模糊
		GaussianBlur(fgMaskMOG2.clone(), fgMaskMOG2, Size(blurSize, blurSize), 0, 0);
		imshow("After blur", fgMaskMOG2);

		double count = 0;
		uchar *p;
		//	二值化運算, 計算移動像素
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

		//	計算移動比率
		double currRatio = count / (fgMaskMOG2.rows * fgMaskMOG2.cols);

		if (currRatio > subRatio) {
			cout << "目前移動比率 : " << currRatio * 100 << "%" << endl;
		}

		waitKey(30);
	}
	capture.release();

	return 0;
}