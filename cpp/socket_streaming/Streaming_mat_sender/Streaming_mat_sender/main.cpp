
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>

#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

void sendImage(SOCKET&);


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

	//AF_INET：使用 IPv4, AF_INET6：使用 IPv6
	//SOCK_STREAM：使用 TCP 協議, SOCK_DGRAM：使用 UDP 協議
	//IPPROTO_TCP：使用 TCP, IPPROTO_UDP：使用 UDP


	//宣告 socket 位址資訊
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	memset(&addr, 0, addrlen);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(777);


	//設定 Listen
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);		//等待連線

	SOCKET sConnect;
	SOCKADDR_IN clinetAddr;
	while (true) {
		cout << "waiting..." << endl;

		if (sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, NULL)) {
			cout << "a connection was found" << endl;
			cout << "server: got connection from " << inet_ntoa(addr.sin_addr) << endl;

			//傳送訊息給 client 端
			//char *sendbuf = "sending data test";
			//send(sConnect, sendbuf, (int)strlen(sendbuf), 0);

			sendImage(sConnect);
		}
	}
	return 0;
}

void sendImage(SOCKET &sConnect) {
	VideoCapture cap(0);

	Mat img;
	int  imgSize;
	int bytes = 0;

	cap.set(CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CAP_PROP_FRAME_HEIGHT, 480);

	while (1) {
		cap >> img;

		if (img.empty())
			break;

		imshow("Server send", img);

		img = img.reshape(0, 1);
		imgSize = img.total()*img.elemSize();

		bytes = send(sConnect, reinterpret_cast<char*>(img.data), imgSize, 0);

		cout << bytes << endl;


		waitKey(100);
	}
}
