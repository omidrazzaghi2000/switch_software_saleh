#include "receiver_socket/receiver_socket.hh"

void ReceiverSocket::StartReceiving()
{

	/* configure for initilizing module*/
	isActive = true;


	int ret, i;
	int sockopt;
	ssize_t numbytes;
	struct ifreq ifopts; /* set promiscuous mode */
	struct ifreq if_ip;	 /* get ip addr */
	struct sockaddr_storage their_addr;
	uint8_t buf[SIZE_OF_BUFFERS];
	char ifName[IFNAMSIZ];

	/* Header structures */
	struct ether_header *eh = (struct ether_header *)buf;

	strcpy(ifName, interface_name);
	/* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)
	{
		perror("listener: socket");
		exit(EXIT_FAILURE);
	}

	/* Set interface to promiscuous mode - do we need to do this every time? */
	strncpy(ifopts.ifr_name, ifName, IFNAMSIZ - 1);
	ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl(sockfd, SIOCSIFFLAGS, &ifopts);
	/* Allow the socket to be reused - incase connection is closed prematurely */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1)
	{
		perror("setsockopt");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	/* Bind to device */
	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ - 1) == -1)
	{
		perror("SO_BINDTODEVICE");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	int counter = 0;
	sockaddr_ll address;
	socklen_t len = sizeof(address);

    FPGA_Packet curPacket;

	while (isActive)
	{
		numbytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&address, &len);


        /* It is commented because in this test we send packets through this interface */
        if (address.sll_pkttype == PACKET_OUTGOING)
        {
            continue;
        }


        /* Check that whether it is a fpga packet or not */
        if(numbytes == CURRENT_FPGA_PACKET_SIZE){
            curPacket = FPGA_Packet((char *)buf,numbytes-1);

            if(curPacket.source_id == sender_id){
//                std::cout << "\033[37m" << &_queue << " - " <<curPacket.data << "-" << curPacket.frame_number << "\033[0m" <<std::endl;
                _queue.push_back(curPacket);
//                printf("\033[43m [listener] %s: got packet %lu bytes \033[0m\n", curPacket.data.c_str() ,numbytes);
            }
        }



	}

}

void ReceiverSocket::StartGrabbing(){
    /* configure for initilizing module*/
    isActive = true;


    int ret, i;
    int sockopt;
    ssize_t numbytes;
    struct ifreq ifopts; /* set promiscuous mode */
    struct ifreq if_ip;	 /* get ip addr */
    struct sockaddr_storage their_addr;
    uint8_t buf[SIZE_OF_BUFFERS];
    char ifName[IFNAMSIZ];

    /* Header structures */
    struct ether_header *eh = (struct ether_header *)buf;

    strcpy(ifName, interface_name);
    /* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
    if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)
    {
        perror("listener: socket");
        exit(EXIT_FAILURE);
    }

    /* Set interface to promiscuous mode - do we need to do this every time? */
    strncpy(ifopts.ifr_name, ifName, IFNAMSIZ - 1);
    ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
    ifopts.ifr_flags |= IFF_PROMISC;
    ioctl(sockfd, SIOCSIFFLAGS, &ifopts);
    /* Allow the socket to be reused - incase connection is closed prematurely */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1)
    {
        perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    /* Bind to device */
    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ - 1) == -1)
    {
        perror("SO_BINDTODEVICE");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    int counter = 0;
    sockaddr_ll address;
    socklen_t len = sizeof(address);

    FPGA_Packet curPacket;

    while (isActive)
    {
        numbytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&address, &len);


        /* It is commented because in this test we send packets through this interface */
        if (address.sll_pkttype == PACKET_OUTGOING)
        {
            continue;
        }

        if(numbytes == 109){
            /* Check that whether it is a fpga packet or not */
            AddToFile("omid.txt",(char*)buf);
        }


        memset(buf,0,sizeof(buf));

    }
}

void ReceiverSocket::AddToFile(std::string filename,char * data){
    std::ofstream outfile;

    outfile.open(filename, std::ios_base::app);
    outfile << data << std::endl;
    outfile.close();
}