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

	while (isActive)
	{
		numbytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&address, &len);

        /* It is commented because in this test we send packets through this interface */
//        if (address.sll_pkttype == PACKET_OUTGOING)
//        {
//            continue;
//        }
		printf("[listener] %s: got packet %lu bytes\n", ifName ,numbytes);


	}

}