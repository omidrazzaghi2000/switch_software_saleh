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
#include <sstream>
#include <iomanip> /* std::std::setprecision */
#include <vector>

#define SIZE_OF_BUFFERS 10000

class SenderSocket{

    public:
        SenderSocket(const char * _ifName,std::string _id,int _sleep_time, int _msg_index):interface_name(_ifName),sender_id(_id),sleep_time(_sleep_time),msg_index(_msg_index){}

        void setSleepTime(int _sleep_time);
        void StartTransmitting();
        
    private:
        std::string sender_id;
        int sockfd;
        bool isActive = false;
        const char * interface_name;
        int sleep_time;
        int msg_index;
};