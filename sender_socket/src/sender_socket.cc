#include "sender_socket/sender_socket.hh"

void SenderSocket::StartTransmitting()
{

	/* configure for initilizing module*/
	isActive = true;


    int ret_val;
    int byte_count;
    struct ifreq if_idx;
    struct ifreq if_mac;
    struct sockaddr_ll socket_address;
    long int numbytes;

    /* Open RAW socket to send on */
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
        perror("socket");
    }

    /* Get the index of the interface to send on */
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, interface_name, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
        perror("SIOCGIFINDEX");
    /* Get the MAC address of the interface to send on */
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, interface_name, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
        perror("SIOCGIFHWADDR");

    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    /* Address length*/
    socket_address.sll_halen = ETH_ALEN;
    /* Destination MAC */


    std::cout << "\033[92m" << "[LOG] : Sender --"<< sender_id <<  "-- Socket OK "<< "\033[0m" << std::endl;


    /* Generate fake data */
    std::string msg1 = std::string(6232-88,'*');
    std::string msg2 = std::string(6232-88,'#');

    int frame_number = 0;

    auto msg_array = std::vector<std::string>({msg1 , msg2});

    std::string sendBuf;
    std::stringstream frame_number_stream;


	while (isActive)
	{
        frame_number_stream  << std::setfill ('0') << std::setw(2)
                             << std::hex << frame_number;

        sendBuf = "[" + sender_id + "]---> " + frame_number_stream.str() + msg_array[msg_index];
        frame_number_stream.str("");

        numbytes = sendto(sockfd, sendBuf.c_str(), sendBuf.length()  , 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
        if(numbytes < 0){
            std::cout << "\033[31m" << "[ERROR] : Sender --"<< sender_id <<  "-- Send Failed "<< "\033[0m" << std::endl;

        }else{
            frame_number++;
            /*reset frame*/
            if(frame_number == 0xff){
                frame_number = 0;
            }
//            printf("[sender] %s: sent packet successfully\n", sender_id.c_str() );
        }

        usleep(sleep_time);
//        isActive = false;
	}

}

void SenderSocket::setSleepTime(int _sleep_time) {
    sleep_time = _sleep_time;
}

