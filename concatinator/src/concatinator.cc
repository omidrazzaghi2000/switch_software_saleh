//
// Created by nor on 5/25/24.
//
#include "concatinator/concatinator.hh"

void Concatinator::startConcatinator() {

    defineSocket();

    isActive = true;

    FPGA_Packet curPacket;
    int min_frame_number=256; // maximum is 0xff then plus one
    int numbytes;

    std::string sendBuf;
    std::stringstream frame_number_string_stream;
    std::string packet_src_ids;
    std::string data_s;

    while(isActive){
        /* reset everything */
        min_frame_number=256;
        sendBuf = "";
        packet_src_ids = "";
        data_s = "";
        frame_number_string_stream.str("");

        /* find minimum frame_number */
        for(auto & queue : queues){

            if(queue->front().frame_number < min_frame_number){
                min_frame_number = queue->front().frame_number;
            }
        }

        /* find hex string of frame_number for sending packet */
        frame_number_string_stream << std::setfill ('0') << std::setw(2)
                              << std::hex << min_frame_number;

        /* get all packet with minimum number and pop them */
        for(auto & queue : queues) {
            if(queue->front().frame_number == min_frame_number){

                packet_src_ids += queue->front().source_id;

                data_s += queue->front().data;

                queue->pop_front();

            }
        }

        if(!data_s.empty() && !packet_src_ids.empty()){
            /* ready packet and send it to the interface  */
            sendBuf = "[" + packet_src_ids + "]---> " + frame_number_string_stream.str() + data_s;

            numbytes = sendto(sockfd, sendBuf.c_str(), sendBuf.length()  , 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0;
            if(numbytes < 0){
                std::cout << "\033[31m" << "[ERROR] : Concatenation --"<< id <<  "-- Send Failed "<< "\033[0m" << std::endl;
            }else{
                printf("[concatinator] %s: sent packet successfully\n", id.c_str() );
            }
        }





    }
}

void Concatinator::addQueue(SharedQueue<FPGA_Packet> *queue_ptr) {
    queues.push_back(queue_ptr);
}

void Concatinator::defineSocket() {
    int ret_val;
    int byte_count;
    struct ifreq if_idx;
    struct ifreq if_mac;
    long int numbytes;

    /* Open RAW socket to send on */
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
        perror("socket");
    }

    /* Get the index of the interface to send on */
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, output_interface_name.c_str(), IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
        perror("SIOCGIFINDEX");
    /* Get the MAC address of the interface to send on */
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, output_interface_name.c_str(), IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
        perror("SIOCGIFHWADDR");

    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    /* Address length*/
    socket_address.sll_halen = ETH_ALEN;
    /* Destination MAC */


    std::cout << "\033[92m" << "[LOG] : Concatinator Socket OK "<< "\033[0m" << std::endl;
}
