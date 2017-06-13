#include "SenderSocket.h"

using namespace std;

SenderSocket::SenderSocket(const string &ip, const int port) {

	WSAData wsaData;
	WORD DLLVSERION;
	DLLVSERION = MAKEWORD(2, 1);

	int startup = WSAStartup(DLLVSERION, &wsaData);
	if (startup != 0) {
		cout << "��l�ƥ���" << endl;
		return;
	}

	//�]�w socket
	sConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sConnect == INVALID_SOCKET) {
		cout << "Could not create socket : " << WSAGetLastError() << endl;
		return;
	}

	int addrlen = sizeof(addr);
	memset(&addr, 0, addrlen);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());

	//�}�l�s�u
	int iconn = connect(sConnect, (SOCKADDR*)&addr, addrlen);

	if (iconn != SOCKET_ERROR) {
		cout << "�s�u���\" << endl;
		isConn = true;
	}
	else {
		cout << "Connect failed with error code : " << WSAGetLastError() << endl;
	}
}


bool SenderSocket::sendPacket(vector<unsigned char> data) {
	//�ǰe�ʥ]
	int sendbytes = send(sConnect, reinterpret_cast<char*>(data.data()), data.size(), 0);

	if (sendbytes == SOCKET_ERROR) {
		cout << "Connect ERROR with error code : " << WSAGetLastError() << endl;
		isConn = false;
		return false;
	}
	
	//cout << "Send package " << ++packageCount << "\tSize : " << sendbytes << endl;
	return true;
}

bool SenderSocket::isConnect() {
	return isConn;
}
	
