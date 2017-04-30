
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
	DLLVSERION = MAKEWORD(2, 1);//Winsocket-DLL ����

								//�� WSAStartup �}�l Winsocket-DLL
	int startup = WSAStartup(DLLVSERION, &wsaData);
	if (startup != 0) {
		cout << "��l�ƥ���" << endl;
		return -1;
	}

	//AF_INET�G�ϥ� IPv4, AF_INET6�G�ϥ� IPv6
	//SOCK_STREAM�G�ϥ� TCP ��ĳ, SOCK_DGRAM�G�ϥ� UDP ��ĳ
	//IPPROTO_TCP�G�ϥ� TCP, IPPROTO_UDP�G�ϥ� UDP


	//�ŧi socket ��}��T
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	memset(&addr, 0, addrlen);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(777);


	//�]�w Listen
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);		//���ݳs�u

	SOCKET sConnect;
	SOCKADDR_IN clinetAddr;
	while (true) {
		cout << "waiting..." << endl;

		if (sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, NULL)) {
			cout << "a connection was found" << endl;
			cout << "server: got connection from " << inet_ntoa(addr.sin_addr) << endl;

			//�ǰe�T���� client ��
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
