//
//  SenderSocket.cpp
//  SocketSenderLinux
//
//  Created by Ronald Lin on 2016/12/6.
//  Copyright © 2016年 Ronald Lin. All rights reserved.
//

#include "SenderSocket.h"

using namespace std;

SenderSocket::SenderSocket(const string &ip, const int port) {
    
    // create server socket
    sConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (sConnect == -1) {
        cout << "Could not create socket : " << strerror(errno) << endl;
        return;
    }
    
    
    int addrlen = sizeof(addr);
    memset(&addr, 0, addrlen);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    
    
    
    int iconn = connect(sConnect, (struct sockaddr *) &addr, addrlen);
    
    if (iconn != -1) {
        cout << "Connect Successful" << endl;
        isConn = true;
    }
    else {
        cout << "Connect failed with error code : " << strerror(errno) << endl;
    }
}


bool SenderSocket::sendPacket(vector<unsigned char> data) {
    
    long int sendbytes = send(sConnect, data.data(), data.size(), 0);
    
    if (sendbytes == -1) {
        cout << "Connect ERROR with error code : " << strerror(errno) << endl;
        isConn = false;
        return false;
    }
    
    cout << "Send package " << ++packageCount << "\tSize : " << sendbytes << endl;
    return true;
}

bool SenderSocket::isConnect() {
    return isConn;
}

