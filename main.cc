#include <iostream>
#include <thread>

#include <receiver_socket/receiver_socket.hh>
#include <sender_socket/sender_socket.hh>

int main(){

    std::cout << "In the name of Allah" << std::endl;

    /******************************************************************/
    /** Receivers                                                     */
    /******************************************************************/
    auto recv_wifi = ReceiverSocket("wlx3460f954dcac");
    auto recv_lan = ReceiverSocket("enp0s31f6");
    std::thread recvWiFiThread(&ReceiverSocket::StartReceiving, recv_wifi);
    std::thread recvLanThread(&ReceiverSocket::StartReceiving, recv_lan);


    /******************************************************************/
    /** Senders                                                       */
    /******************************************************************/
    auto send_to_wifi = SenderSocket("wlx3460f954dcac","wifi&%argw",1,0);
    auto send_to_lan = SenderSocket("enp0s31f6","lan&%argw",1,1);
    std::thread sendWiFiThread(&SenderSocket::StartTransmitting,send_to_wifi);
    std::thread sendLanThread(&SenderSocket::StartTransmitting,send_to_lan);

    sendLanThread.join();
    sendWiFiThread.join();

    recvLanThread.join();
    recvWiFiThread.join();


    return 0;
}