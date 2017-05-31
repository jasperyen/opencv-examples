#include "SenderSocket.h"
#include "JpegEncoder.h"

using namespace std;


int main() {
	SenderSocket sender("140.129.20.136", 777);

	if (!sender.isConnect())
		return -1;

	VideoCapture capture;
	
	if (argc > 1)
		capture.open(atoi(argv[1]));
	else
		capture.open(0);


	JpegEncoder jencoder(capthread, false, 100, 1.0);

	CaptureThread capthread(capture);
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
