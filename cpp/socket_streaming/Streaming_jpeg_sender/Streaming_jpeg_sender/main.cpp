// main.cpp : 定義主控台應用程式的進入點。
//

#include "SenderSocket.h"
#include "JpegEncoder.h"

using namespace std;


int main() {
	SenderSocket sender("127.0.0.1", 777);

	if (!sender.isConnect())
		return -1;

	VideoCapture capture(1);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);


	CaptureThread capthread(capture);

	JpegEncoder jencoder(capthread, 60, 1.0);

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

