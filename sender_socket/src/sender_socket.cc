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
    int numbytes;

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
    std::string msg1 = "Hello I am message 1 from Omid in which ";
    std::string msg2 = "Omid is very nice and attractive.";

    auto msg_array = std::vector<std::string>({msg1 , msg2});

    std::string sendBuf = "[" + sender_id + "]---> " + msg_array[msg_index];

	while (isActive)
	{


        numbytes = sendto(sockfd, sendBuf.c_str(), sendBuf.length()  , 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0;
        if(numbytes < 0){
            std::cout << "\033[31m" << "[ERROR] : Sender --"<< sender_id <<  "-- Send Failed "<< "\033[0m" << std::endl;
        }else{
            printf("[sender] %s: sent packet %lu bytes\n", sender_id.c_str() , numbytes);
        }

        sleep(sleep_time);

	}

}

void SenderSocket::setSleepTime(int _sleep_time) {
    sleep_time = _sleep_time;
}

