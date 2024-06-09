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
#include <fstream>
#include <channel/fpga_packet.hh>
#include <channel/shared_queue.hpp>
#include <utility>

#define SIZE_OF_BUFFERS 4096
#define CURRENT_FPGA_PACKET_SIZE 60

class ReceiverSocket{

    public:
    ReceiverSocket(const char * _ifName,std::string _sender_id,SharedQueue<FPGA_Packet> & _queue_ref):interface_name(_ifName),sender_id(std::move(_sender_id)),_queue(_queue_ref){}

//        ReceiverSocket(const char * _ifName,std::string _sender_id,const std::unique_ptr<SharedQueue<FPGA_Packet>>  & _queue_ref):interface_name(_ifName),sender_id(std::move(_sender_id)),_queue_ptr(_queue_ref){}

        void StartReceiving();
        void StartGrabbing();
        
    private:
        int sockfd;
        bool isActive = false;
        const char * interface_name;
        std::string sender_id;
        SharedQueue<FPGA_Packet> & _queue;
//    const std::unique_ptr<SharedQueue<FPGA_Packet>> & _queue_ptr;

    void AddToFile(std::string filename, char *data);
};