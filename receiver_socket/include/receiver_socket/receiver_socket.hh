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

#include <channel/fpga_packet.hh>
#include <channel/shared_queue.hpp>
#include <utility>

#define SIZE_OF_BUFFERS 4096
#define CURRENT_FPGA_PACKET_SIZE 59

class ReceiverSocket{

    public:
        ReceiverSocket(const char * _ifName,std::string _sender_id,std::shared_ptr<SharedQueue<FPGA_Packet>> _queue_ptr):interface_name(_ifName),sender_id(std::move(_sender_id)),_queue_shared_ptr(std::move(_queue_ptr)){}

        void StartReceiving();
        
    private:
        int sockfd;
        bool isActive = false;
        const char * interface_name;
        std::string sender_id;
        std::shared_ptr<SharedQueue<FPGA_Packet>> _queue_shared_ptr;

};