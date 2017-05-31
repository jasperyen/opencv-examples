// main.cpp : 定義主控台應用程式的進入點。
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

	JpegEncoder jencoder(capthread, false, 100, 1.0);

	capthread.startCapture();

	vector<unsigned char> data;
	while (capthread.isCapturing()) {
		if (!jencoder.getJpegPackage(data))
			break;

		if (!sender.sendPacket(data))
			break;
	}

	cout << "out" << endl;

    return 0;
}

