// main.cpp : 定義主控台應用程式的進入點。
//

#include "ReceiverSocket.h"
#include "JpegDecoder.h"

int main() {

	ReceiverSocket receiver("127.0.0.1", 777);

	if (!receiver.isConnect())
		return -1;

	JpegDecoder jdecoder;

	vector<unsigned char> data;


	while (receiver.haveData() || receiver.isConnect()) {
		if (!receiver.haveData())
			continue;

		data = receiver.getFrontData();
		jdecoder.showJpeg(data);
	}

    return 0;
}

