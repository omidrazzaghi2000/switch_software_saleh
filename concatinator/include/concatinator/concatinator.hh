//
// Created by nor on 5/25/24.
//

#ifndef OSWITCH_CONCATINATOR_HH
#define OSWITCH_CONCATINATOR_HH

#include <channel/fpga_packet.hh>
#include <channel/shared_queue.hpp>
#include <utility>
#include <sstream>
#include <iomanip> /* for setfill */
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

#define MAXIMUM_PACKET_SIZE 1500

class Concatinator{
public:
    Concatinator(std::string _id,std::string  output):output_interface_name(std::move(output)),id(std::move(_id)){}
    void addQueue(SharedQueue<FPGA_Packet> * queue);
    void startConcatinator();
private:
    void defineSocket();

    std::vector<SharedQueue<FPGA_Packet> *>  queues;
    bool isActive;
    std::string output_interface_name;
    int sockfd;
    struct sockaddr_ll socket_address;
    std::string id;
};


#endif //OSWITCH_CONCATINATOR_HH
