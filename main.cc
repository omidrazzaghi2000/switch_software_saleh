#include <iostream>
#include <thread>

#include <receiver_socket/receiver_socket.hh>
#include <sender_socket/sender_socket.hh>
#include <channel/fpga_packet.hh>
#include <channel/shared_queue.hpp>
#include <concatinator/concatinator.hh>


#define WIFI_INTERFACE_ID "wifi&%argw"
#define LAN_INTERFACE_ID "lani&%argw"
#define CONCAT_INTERFACE_ID "cati&%argw"

#define LAN_INTERFACE_NAME "enp0s31f6"
#define WIFI_INTERFACE_NAME "wlx3460f954dcac"

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
    auto recv_wifi = ReceiverSocket(WIFI_INTERFACE_NAME,WIFI_INTERFACE_ID,std::ref(wifi_interface_queue));
    auto recv_lan = ReceiverSocket(LAN_INTERFACE_NAME,LAN_INTERFACE_ID,std::ref(lan_interface_queue));
    std::thread recvWiFiThread(&ReceiverSocket::StartReceiving, recv_wifi);
    std::thread recvLanThread(&ReceiverSocket::StartReceiving, recv_lan);


    /******************************************************************/
    /** Senders                                                       */
    /******************************************************************/
    auto send_to_wifi = SenderSocket(WIFI_INTERFACE_NAME,WIFI_INTERFACE_ID,1,0);
    auto send_to_lan = SenderSocket(LAN_INTERFACE_NAME,LAN_INTERFACE_ID,1,1);
    std::thread sendWiFiThread(&SenderSocket::StartTransmitting,send_to_wifi);
    std::thread sendLanThread(&SenderSocket::StartTransmitting,send_to_lan);

    /******************************************************************/
    /** Concatinator                                                  */
    /******************************************************************/
    auto concatinator = Concatinator(CONCAT_INTERFACE_ID,LAN_INTERFACE_NAME);
    concatinator.addQueue(&wifi_interface_queue);
    concatinator.addQueue(&lan_interface_queue);
    std::thread concatinatorThread(&Concatinator::startConcatinator,concatinator);

    concatinatorThread.join();

    sendLanThread.join();
    sendWiFiThread.join();

    recvLanThread.join();
    recvWiFiThread.join();




    return 0;
}