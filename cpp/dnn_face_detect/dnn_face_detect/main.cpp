#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::dnn;

const size_t inWidth = 300;
const size_t inHeight = 300;
const double inScaleFactor = 1.0;
const Scalar meanVal(104.0, 177.0, 123.0);
const float confidenceThreshold = 0.5;

int main(int argc, char** argv) {


	String modelConfiguration = "face_detector/deploy.prototxt";
	String modelBinary = "face_detector/res10_300x300_ssd_iter_140000.caffemodel";

	//! [Initialize network]
	dnn::Net net = readNetFromCaffe(modelConfiguration, modelBinary);

	if (net.empty()){
		cerr << "Can't load network files" << endl;
		return -1;
	}

	//	opencl
	//net.setPreferableTarget(DNN_TARGET_OPENCL);


	String video = "..\\..\\..\\test_data\\human_detect.mp4";

	VideoCapture cap(video);

	if (!cap.isOpened()) {
		cout << "Could not open VideoCapture" << endl;
		return -1;
	}


	
	while (true) {
		Mat frame;
		cap >> frame;

		if (frame.empty())
			break;
		

		//resize(frame, frame, Size(1280, 720));

		//! [Prepare blob]
		Mat inputBlob = blobFromImage(frame, inScaleFactor,
			Size(inWidth, inHeight), meanVal, false, false); //Convert Mat to batch of images
															 //! [Prepare blob]

															 //! [Set input blob]
		net.setInput(inputBlob, "data"); //set the network input
										 //! [Set input blob]

										 //! [Make forward pass]
		Mat detection = net.forward("detection_out"); //compute output
													  //! [Make forward pass]

		vector<double> layersTimings;
		double freq = getTickFrequency() / 1000;
		double time = net.getPerfProfile(layersTimings) / freq;

		Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

		ostringstream ss;
		ss << "FPS: " << 1000 / time << " ; time: " << time << " ms";
		putText(frame, ss.str(), Point(20, 20), 0, 0.5, Scalar(0, 0, 255));


		for (int i = 0; i < detectionMat.rows; i++) {
			float confidence = detectionMat.at<float>(i, 2);

			if (confidence > confidenceThreshold) {
				int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
				int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
				int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
				int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

				Rect object((int)xLeftBottom, (int)yLeftBottom,
					(int)(xRightTop - xLeftBottom),
					(int)(yRightTop - yLeftBottom));

				rectangle(frame, object, Scalar(0, 255, 0));

				ss.str("");
				ss << confidence;
				String conf(ss.str());
				String label = "Face: " + conf;
				int baseLine = 0;
				Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
				rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
					Size(labelSize.width, labelSize.height + baseLine)),
					Scalar(255, 255, 255), FILLED);
				putText(frame, label, Point(xLeftBottom, yLeftBottom),
					FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
			}
		}

		imshow("detections", frame);
		waitKey(1);
	}

	return 0;
}