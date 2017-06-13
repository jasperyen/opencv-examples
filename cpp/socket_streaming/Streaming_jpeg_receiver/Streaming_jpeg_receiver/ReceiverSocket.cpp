#include "ReceiverSocket.h"

using namespace std;

ReceiverSocket::ReceiverSocket(const string &ip, const int port) {

	WSAData wsaData;
	WORD DLLVSERION;
	DLLVSERION = MAKEWORD(2, 1);

	int startup = WSAStartup(DLLVSERION, &wsaData);
	if (startup != 0) {
		cout << "Failed. Error Code : " << WSAGetLastError() << endl;
		return;
	}

	//設定 socket
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET) {
		cout << "Could not create socket : " << WSAGetLastError() << endl;
		return;
	}

	int addrlen = sizeof(addr);
	memset(&addr, 0, addrlen);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());

	
	if ( ::bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR ) {
		cout << "Bind failed with error code : " << WSAGetLastError () << endl;
		return;
	}
	

	if ( listen(sListen, SOMAXCONN) == SOCKET_ERROR ) {
		cout << "Listen failed with error code : " << WSAGetLastError() << endl;
		return;
	}
	//開始監聽

	SOCKADDR_IN clinetAddr;
	memset(&clinetAddr, 0, sizeof(clinetAddr));
	
	cout << "Waiting..." << endl;

	sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, NULL);
	//有連線進入

	if (sConnect == INVALID_SOCKET) {
		cout << "Connect failed with error code : " << WSAGetLastError() << endl;
		return;
	}

	
	cout << "a connection was found" << endl;
	printf("server: got connection from %s\n", inet_ntoa(clinetAddr.sin_addr));

	isConn = true;

	thread *t;
	t = new thread(&ReceiverSocket::recvThread, this);

}

void ReceiverSocket::recvThread() {
	static const int buffer_size = 4194304; //4MB
	unsigned char *buffer = new (unsigned char[buffer_size]);
	unsigned int datasize[1] = {0};
	int recvbytes;
	int bytes = 0;
	vector<unsigned char> data;
	unique_lock<mutex> locker(queue_mutex, defer_lock);

	clock_t last_c, now_c;
	double duration;

	cout << fixed;
	cout.precision(2);

	while (true) {
		last_c = clock();
		while (true) {
			recvbytes = recv(sConnect, reinterpret_cast<char*>(buffer) + bytes, buffer_size - bytes, 0);

			if (recvbytes == SOCKET_ERROR) {
				cout << "Connect ERROR with error code in recv : " << WSAGetLastError() << endl;
				isConn = false;
				return;
			}
			else if (recvbytes == 0) {
				cout << "Connect close" << endl;
				isConn = false;
				return;
			}

			//cout << "Receive package size is : " << recvbytes << endl;

			bytes += recvbytes;

			if (datasize[0] == 0) {
				memcpy(datasize, buffer, sizeof(unsigned int));
				packageCount++;
				//cout << packageCount << " package size is : " << datasize[0] << endl;
			}

			if (bytes < datasize[0]) {
				continue;
			}
			else if (bytes == datasize[0]) {
				//cout << packageCount << " package receive complete ."<< endl;
				now_c = clock();
				duration = (now_c - last_c) / (double)CLOCKS_PER_SEC;

				
				cout << "現在網路速度 : " << setw(9) << (datasize[0] / duration / 1024.0) 
					<< " KB/S\t封包大小 : " << setw(7) << datasize[0]
					<< "\t即時禎數 : " << setw(5) << (1 / duration) << endl;


				data.assign(buffer + sizeof(unsigned int), buffer + bytes);

				locker.lock();
				dataQue.push_back(data);
				locker.unlock();

				datasize[0] = 0;
				bytes = 0;
				break;
			}
			else {
				//cout << packageCount << " package receive complete ." << endl;
				now_c = clock();
				duration = (now_c - last_c) / (double)CLOCKS_PER_SEC;

				
				cout << "現在網路速度 : " << setw(9) << (datasize[0] / duration / 1024.0)
					<< " KB/S\t封包大小 : " << setw(7) << datasize[0]
					<< "\t即時禎數 : " << setw(5) << (1 / duration) << endl;

				data.assign(buffer + sizeof(unsigned int), buffer + datasize[0]);

				locker.lock();
				dataQue.push_back(data);
				locker.unlock();

				//cout << "Remove buffer direction !" << endl;
				for (int i = 0; i < (bytes - datasize[0]); i++) {
					buffer[i] = buffer[datasize[0] + i];
				}

				bytes = bytes - datasize[0];

				memcpy(datasize, buffer, sizeof(unsigned int));
				packageCount++;
				//cout << packageCount << " package size is : " << datasize[0] << endl;

				break;
			}
		}

	
	}
}

bool ReceiverSocket::getFrontData(vector<unsigned char> &data) {
	unique_lock<mutex> locker(queue_mutex);

	if (dataQue.empty())
		return false;

	data = dataQue.front();
	dataQue.pop_front();

	return true;
}

bool ReceiverSocket::isConnect() {
	return isConn;
}

bool ReceiverSocket::haveData() {
	return !dataQue.empty();
}


