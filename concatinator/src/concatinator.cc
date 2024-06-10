//
// Created by nor on 5/25/24.
//
#include "concatinator/concatinator.hh"


unsigned short checksum(void *b, int len) {
    unsigned short *buf = (unsigned short *)b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void create_ip_header(char *buffer, const char *src_ip, const char *dst_ip, int id, int frag_offset,bool more_fragments, int data_len) {
    struct iphdr *iph = (struct iphdr *)buffer;
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(sizeof(struct iphdr) + data_len);  // Header length plus UDP header
    iph->id = htons(id);
    iph->frag_off = htons(frag_offset | (more_fragments ? 0x2000 : 0));
    iph->ttl = 64;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr(src_ip);
    iph->daddr = inet_addr(dst_ip);
    iph->check = checksum((unsigned short *)buffer, iph->ihl * 4);
}



void Concatinator::startConcatinator() {

    defineSocket();
//    defineTCP_ServerSocket();

    isActive = true;

    FPGA_Packet curPacket;
    int min_frame_number=256; // maximum is 0xff then plus one
    int numbytes;

    std::string sendBuf;
    std::stringstream frame_number_string_stream;
    std::string packet_src_ids;
    std::string data_s;


    const char *src_ip = "192.168.1.1";
    const char *dst_ip = "192.168.1.2";
    int fragment_size = 9000;
    int packet_id = 54321;
    int offset = 0;


    while(isActive){
        /* reset everything */
        min_frame_number=256;
        sendBuf = "";
        packet_src_ids = "";
        data_s = "";
        frame_number_string_stream.str("");

        /* find minimum frame_number */
        for(int i = 0 ; i < queues.size() ; i++){

            auto queue =  queues[i];

            curPacket = queue->front();

            if(curPacket.frame_number < min_frame_number){
                min_frame_number = curPacket.frame_number;
            }
        }


        /* find hex string of frame_number for sending packet */
        frame_number_string_stream << std::setfill ('0') << std::setw(2)
                              << std::hex << min_frame_number;

        /* get all packet with minimum number and pop them */
        for(auto queue : queues) {

            curPacket = queue->front();

            if(curPacket.frame_number == min_frame_number){

                packet_src_ids += curPacket.source_id;

                data_s += curPacket.data;

                queue->pop_front();

            }
        }

        if(!data_s.empty() && !packet_src_ids.empty()){
            /* ready packet and send it to the interface  */
            sendBuf = "[" + packet_src_ids + "]---> " + frame_number_string_stream.str() + data_s;



            for (int i = 0; i < sendBuf.length(); i += fragment_size) {
                char buffer[MAXIMUM_PACKET_SIZE];
                memset(buffer, 0, MAXIMUM_PACKET_SIZE);

                int data_chunk_len = std::min(fragment_size, int(sendBuf.length()) - i);
                bool more_fragments = (i + fragment_size < sendBuf.length());
                create_ip_header(buffer, src_ip, dst_ip, packet_id, offset,more_fragments,data_chunk_len);
                memcpy(buffer + sizeof(struct iphdr), sendBuf.c_str()+i, data_chunk_len);

                if (sendto(sockfd, buffer, sizeof(struct iphdr) + data_chunk_len, 0, (struct sockaddr *)&socket_address, sizeof(struct sockaddr_ll)) < 0) {
                    std::cerr << "Failed to send packet" << std::endl;
//                    printf("The last error message is: %s\n", strerror(errno));
                }

                offset += fragment_size / 8;
            }

        }



    }
}

void Concatinator::addQueue(SharedQueue<FPGA_Packet> * queue_ptr) {
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

void Concatinator::onTCPClientMessageReceived(){

}

void Concatinator::onTCPClientDisconnect(){

}

void Concatinator::defineTCP_ServerSocket(){
//    // start server on port 65123
//    pipe_ret_t startRet = server.start(65123);
//    if (startRet.isSuccessful()) {
//        std::cout << "Server setup succeeded\n";
//    } else {
//        std::cout << "Server setup failed: " << startRet.message() << "\n";
//        return;
//    }
//
//    // configure and register observer1
//    observer1.incomingPacketHandler =  std::bind(&Concatinator::onTCPClientMessageReceived, this);
//    observer1.disconnectionHandler = std::bind(&Concatinator::onTCPClientDisconnect,this);
//    observer1.wantedIP = "127.0.0.1";
//    server.subscribe(observer1);
//
//
//    // accept clients
//    try {
//        std::cout << "waiting for incoming client...\n";
//        std::string clientIP = server.acceptClient(0);
//        std::cout << "accepted new client with IP: " << clientIP << "\n" <<
//                  "== updated list of accepted clients ==" << "\n";
//        server.printClients();
//    } catch (const std::runtime_error &error) {
//        std::cout << "Accepting client failed: " << error.what() << "\n";
//    }

}

