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
#include <csignal>

#include <tcp_server.h>
#define MAXIMUM_PACKET_SIZE 10000
unsigned short checksum(void *b, int len);
void create_ip_header(char *buffer, const char *src_ip, const char *dst_ip, int id, int frag_offset,bool more_fragments, int data_len);
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
    socklen_t len = sizeof(socket_address);
    std::string id;
//    TcpServer server;
//
//
//    server_observer_t observer1, observer2;

    void onTCPClientMessageReceived();

    void onTCPClientDisconnect();

    void defineTCP_ServerSocket();
};


#endif //OSWITCH_CONCATINATOR_HH
