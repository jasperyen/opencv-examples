#ifndef SENDERSOCKET_H
#define SENDERSOCKET_H

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SenderSocket {

private:
	SOCKET sConnect;
	SOCKADDR_IN addr;
	bool isConn = false;
	unsigned int packageCount = 0;

public:
	SenderSocket(const string&, const int);
	bool sendPacket(vector<unsigned char> data);
	bool isConnect();

};

#endif