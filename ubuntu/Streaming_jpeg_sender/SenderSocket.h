//
//  SenderSocket.hpp
//  SocketSenderLinux
//
//  Created by Ronald Lin on 2016/12/6.
//  Copyright © 2016年 Ronald Lin. All rights reserved.
//

#ifndef SenderSocket_hpp
#define SenderSocket_hpp

#endif /* SenderSocket_hpp */


#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // solve "inet_addr" not declared problem
#include <string.h> // solve "strerror" not declared problem
#include <errno.h> // solve "errno" not declared problem
#include <unistd.h> // solve "close" not declared problem

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SenderSocket {
    
private:
    int sConnect;
    struct sockaddr_in addr;
    bool isConn = false;
    unsigned int packageCount = 0;
    
public:
    SenderSocket(const string&, const int);
    bool sendPacket(vector<unsigned char> data);
    bool isConnect();
    
};
