
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>

#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void displayStreaming(SOCKET&);

int main() {
	WSAData wsaData;
	WORD DLLVSERION;
	DLLVSERION = MAKEWORD(2, 1);//Winsocket-DLL 版本

								//用 WSAStartup 開始 Winsocket-DLL
	int startup = WSAStartup(DLLVSERION, &wsaData);
	if (startup != 0) {
		cout << "初始化失敗" << endl;
		return -1;
	}


	//宣告給 socket 使用的 sockadder_in 結構
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	memset(&addr, 0, addrlen);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(777);

	//設定 socket
	SOCKET sConnect;
	sConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//開始連線
	int isconn = connect(sConnect, (SOCKADDR*)&addr, addrlen);

	if (isconn != SOCKET_ERROR) {
		cout << "連線成功" << endl;

		//接收 server 端的訊息
		/*
		char message[200];
		ZeroMemory(message, 200);
		recv(sConnect, message, sizeof(message), 0);
		cout << message << endl;
		*/


		displayStreaming(sConnect);


		//設定 closesocket 時，不經過 TIME-WAIT 過程,直接關閉socket
		//BOOL bDontLinger = FALSE;
		//setsockopt(sConnect,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(BOOL));

		//若之後不再使用，可用 closesocket 關閉連線
		closesocket(sConnect);
	}

	else {
		cout << "連線失敗" << endl;
	}

	return 0;
}

void displayStreaming(SOCKET &sConnect) {
	//921600
	//640*480

	Mat  img = Mat::zeros(480, 640, CV_8UC3);
	int  imgSize = img.total()*img.elemSize();
	uchar sockData[921600] = {0};
	int bytes = 0;
	int recvbytes = 0;

	namedWindow("Client revice", WINDOW_AUTOSIZE);


	while (1) {

		recvbytes = recv(sConnect, reinterpret_cast<char*>(sockData) + bytes, imgSize, 0);

		if (recvbytes == -1) {
			cout << "recive failed" << endl;
			return;
		}
		else if (recvbytes < 921600) {
			cout << "Missing recv : " << recvbytes << endl;
			bytes += recvbytes;

			if (bytes < 921600)
				continue;
			else if (bytes == 921600) {
				cout << "--- Complete recv ---" << endl;
			}
			else {
				cout << "bytes > 921600" << endl;
				return;
			}

		}
		else if (recvbytes == 921600) {
			cout << "Complete recv : " << recvbytes << endl;
		}
		else {
			cout << "recvbytes > 921600" << endl;
			return;
		}

		bytes = 0;

		// Assign pixel value to img

		int ptr = 0;
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				img.at<cv::Vec3b>(i, j) = cv::Vec3b(sockData[ptr + 0], sockData[ptr + 1], sockData[ptr + 2]);
				ptr = ptr + 3;
			}
		}

		imshow("Client revice", img);

		waitKey(1);

	}

}
