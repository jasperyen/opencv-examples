
#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/text.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

using namespace cv;
using namespace std;

int main() {

	Ptr<text::OCRHMMDecoder::ClassifierCallback> ocrProc;
	string vocabulary = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	ocrProc = text::loadOCRHMMClassifierCNN("OCRBeamSearch_CNN_model_data.xml.gz");

	Mat charMat;

	String imgFile = "..\\..\\..\\test_data\\ocr_8.png";

	charMat = imread(imgFile);
	imshow("charMat", charMat);


	vector<int> out_classes;
	vector<double> out_confidences;
	ocrProc->eval(charMat, out_classes, out_confidences);

	cout << fixed << setprecision(5);

	for (int i = 0; i < out_classes.size(); i++) {
		cout << "character : " << vocabulary[out_classes[i]]
			<< " , confidence : " << out_confidences[i] << endl;
	}

	waitKey(0);

	return 0;
}

