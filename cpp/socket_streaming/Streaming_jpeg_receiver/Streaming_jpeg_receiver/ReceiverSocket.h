#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>
#include <time.h>
#include <deque>

using namespace std;

class ReceiverSocket {

private:
	SOCKET sListen;
	SOCKET sConnect;
	SOCKADDR_IN addr;
	bool isConn = false;
	deque<vector<unsigned char>> dataQue;
	unsigned int packageCount = 0;

public:
	ReceiverSocket(const string&, const int);
	void recvThread();
	vector<unsigned char> getFrontData();
	bool isConnect();
	bool haveData();
};