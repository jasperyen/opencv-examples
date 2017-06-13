#include "CaptureThread.h"

int main() {

	bool showFrame = false;

	//VideoCapture capture(0);
	//capture.set(CV_CAP_PROP_FPS, 60);
	//capture.set(CAP_PROP_FRAME_WIDTH, 640);
	//capture.set(CAP_PROP_FRAME_HEIGHT, 480);

	raspicam::RaspiCam_Cv capture;
	capture.set(CV_CAP_PROP_FPS, 60);
	capture.set(CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CAP_PROP_FRAME_HEIGHT, 480);
	capture.open();

	CaptureThread cap_thread(capture);

	cap_thread.startCapture();

	if (showFrame)
		namedWindow("Frame", WINDOW_AUTOSIZE);

	Mat frame;

	while (cap_thread.isCapturing()) {

		while (!cap_thread.getFrame(frame)) {
			this_thread::sleep_for(chrono::duration<int, std::milli>(5));
			//cout << "wait frame" << endl;
		}

		if (showFrame) {
			imshow("Frame", frame);
			waitKey(1);
		}
	}
    return 0;
}
