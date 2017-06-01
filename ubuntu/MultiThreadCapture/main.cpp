#include "CaptureThread.h"

int main() {

	bool showFrame = true;

	VideoCapture capture(0);

	CaptureThread cap_thread(capture);
	cap_thread.startCapture();


	if (showFrame)
		namedWindow("Frame", WINDOW_AUTOSIZE);

	Mat frame;

	while (cap_thread.isCapturing()) {

		while (!cap_thread.getFrame(frame)) {
			this_thread::sleep_for(chrono::duration<int, std::milli>(5));
			cout << "wait frame" << endl;
		}

		if (showFrame) {
			imshow("Frame", frame);
			waitKey(1);
		}
	}
    return 0;
}
