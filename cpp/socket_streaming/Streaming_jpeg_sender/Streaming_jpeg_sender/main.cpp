// main.cpp : 定義主控台應用程式的進入點。
//

#include "SenderSocket.h"
#include "JpegEncoder.h"

using namespace std;


int main() {
	SenderSocket sender("127.0.0.1", 777);

	if (!sender.isConnect())
		return -1;

	//String videoFile = "..\\..\\..\\..\\test_data\\mog2_test.mp4";
	JpegEncoder jencoder(0, true, 100, 1.0);

	if (!jencoder.isCapturing()) {
		cout << "視訊擷取失敗" << endl;
		return -1;
	}

	vector<unsigned char> data;
	while (jencoder.isCapturing()) {
		
		data = jencoder.GetJpegPackageFromCapture();
		
		if (data.empty())
			break;

		if (!sender.sendPacket(data))
			break;
		
		waitKey(1);
	}

    return 0;
}

