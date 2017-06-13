// main.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "SenderSocket.h"
#include "JpegEncoder.h"

using namespace std;


int main() {
	SenderSocket sender("127.0.0.1", 777);

	if (!sender.isConnect())
		return -1;

	VideoCapture capture(0);

	CaptureThread capthread(capture);

	JpegEncoder jencoder(capthread, false, 60, 1.0);

	capthread.startCapture();
	jencoder.startJpegEncode();

	vector<unsigned char> *data;
	while (capthread.isCapturing()) {

		while (!jencoder.getJpegPackage(&data)) {
			this_thread::sleep_for(chrono::duration<int, std::milli>(5));
			//cout << "wait jpge data" << endl;
		}

		if (!sender.sendPacket(*data))
			break;

		delete data;
	}

    return 0;
}

