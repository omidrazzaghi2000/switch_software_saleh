#include <iostream>
#include <thread>

#include <receiver_socket/receiver_socket.hh>
#include <sender_socket/sender_socket.hh>
#include <channel/fpga_packet.hh>
#include <channel/shared_queue.hpp>


#define WIFI_INTERFACE_ID "wifi&%argw"
#define LAN_INTERFACE_ID "lani&%argw"

int main(){

    std::cout << "In the name of Allah" << std::endl;


    /******************************************************************/
    /** Interface queues                                              */
    /******************************************************************/
    auto wifi_interface_queue = SharedQueue<FPGA_Packet>();
    auto lan_interface_queue = SharedQueue<FPGA_Packet>();

    /******************************************************************/
    /** Receivers                                                     */
    /******************************************************************/
    auto recv_wifi = ReceiverSocket("wlx3460f954dcac",WIFI_INTERFACE_ID);
    auto recv_lan = ReceiverSocket("enp0s31f6",LAN_INTERFACE_ID);
    std::thread recvWiFiThread(&ReceiverSocket::StartReceiving, recv_wifi);
    std::thread recvLanThread(&ReceiverSocket::StartReceiving, recv_lan);


    /******************************************************************/
    /** Senders                                                       */
    /******************************************************************/
    auto send_to_wifi = SenderSocket("wlx3460f954dcac",WIFI_INTERFACE_ID,1,0);
    auto send_to_lan = SenderSocket("enp0s31f6",LAN_INTERFACE_ID,1,1);
    std::thread sendWiFiThread(&SenderSocket::StartTransmitting,send_to_wifi);
    std::thread sendLanThread(&SenderSocket::StartTransmitting,send_to_lan);

    sendLanThread.join();
    sendWiFiThread.join();

    recvLanThread.join();
    recvWiFiThread.join();


    return 0;
}