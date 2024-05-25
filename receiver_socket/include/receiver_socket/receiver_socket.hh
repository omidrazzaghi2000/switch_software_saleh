#pragma once

#include <string>

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <thread>
#include <chrono>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <time.h>
#include <iostream>

#define SIZE_OF_BUFFERS 4096

class ReceiverSocket{

    public:
        ReceiverSocket(const char * _ifName):interface_name(_ifName){}

        void StartReceiving();
        
    private:
        int sockfd;
        bool isActive = false;
        const char * interface_name;
};