#include "SenderSocket.h"
#include "JpegEncoder.h"
#include "CaptureThread.h"

using namespace std;


int main() {
	SenderSocket sender("192.168.3.110", 777);

	if (!sender.isConnect())
		return -1;

	//VideoCapture capture(0);
	//capture.set(CV_CAP_PROP_FPS, 60);
	//capture.set(CAP_PROP_FRAME_WIDTH, 640);
	//capture.set(CAP_PROP_FRAME_HEIGHT, 480);

	raspicam::RaspiCam_Cv capture;
	capture.set(CV_CAP_PROP_FPS, 60);
	capture.set(CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CAP_PROP_FRAME_HEIGHT, 480);
	capture.open();

	CaptureThread capthread(capture);



	JpegEncoder jencoder(capthread, false, 100, 1.0);

	capthread.startCapture();
	jencoder.startJpegEncode();

	vector<unsigned char> data;
	while (capthread.isCapturing()) {

		while (!jencoder.getJpegPackage(data)) {
			this_thread::sleep_for(chrono::duration<int, std::milli>(5));
			//cout << "wait jpge data" << endl;
		}

		if (!sender.sendPacket(data))
			break;
	}

	cout << "out" << endl;

    return 0;
}
