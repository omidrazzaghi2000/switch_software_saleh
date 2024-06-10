#include <iostream>
#include <thread>
#include <array>

#include <receiver_socket/receiver_socket.hh>
#include <sender_socket/sender_socket.hh>
#include <channel/fpga_packet.hh>
#include <channel/shared_queue.hpp>
#include <concatinator/concatinator.hh>


#define WIFI_INTERFACE_ID "wifi&%argw"
#define LAN_INTERFACE_ID "lani&%argw"
#define SENDER_INTERFACE_ID_1 "lan1&%argw"
#define SENDER_INTERFACE_ID_2 "lan2&%argw"
#define CONCAT_INTERFACE_ID "cati&%argw"

#define LAN_INTERFACE_NAME "enp0s31f6"
#define WIFI_INTERFACE_NAME "wlx3460f954dcac"

#define SENDER_LAN_NAME1 "enp6s0f2"
#define SENDER_LAN_NAME2 "enp6s0f3"

#define RECEIVER_LAN_NAME1 "enp3s0f2"
#define RECEIVER_LAN_NAME2 "enp3s0f3"

#define OUTPUT_LAN_INTERFACE_NAME "enp6s0f1"
#define FINAL_LAN_INTERFACE_NAME "enp3s0f1"

int main(){

    std::cout << "In the name of Allah" << std::endl;
    std::vector<std::string> RECEIVERS= {RECEIVER_LAN_NAME1,RECEIVER_LAN_NAME2};
    std::vector<std::string> SENDERS_ID = {SENDER_INTERFACE_ID_1,SENDER_INTERFACE_ID_2};
    std::vector <SharedQueue<FPGA_Packet> *> queues_vector;
    SharedQueue<FPGA_Packet> temp_shared_queue;//for avoiding copy

    auto wifi_interface_queue = SharedQueue<FPGA_Packet>();
    auto lan_interface_queue = SharedQueue<FPGA_Packet>();

    for(int rIndex = 0 ; rIndex < RECEIVERS.size() ; rIndex++) {

        /******************************************************************/
        /** Interface queues                                              */
        /******************************************************************/
        auto * queue = new SharedQueue<FPGA_Packet>();
//        std::unique_ptr<SharedQueue<FPGA_Packet>> queue_ptr = std::make_unique<SharedQueue<FPGA_Packet>>();
        queues_vector.push_back(queue);





        /******************************************************************/
        /** Receivers                                                     */
        /******************************************************************/
//    auto recv_wifi = ReceiverSocket(WIFI_INTERFACE_NAME,WIFI_INTERFACE_ID,std::ref(wifi_interface_queue));
//    auto recv_lan = ReceiverSocket(LAN_INTERFACE_NAME,LAN_INTERFACE_ID,std::ref(lan_interface_queue));
//    std::thread recvWiFiThread(&ReceiverSocket::StartReceiving, recv_wifi);
//    std::thread recvLanThread(&ReceiverSocket::StartReceiving, recv_lan);

        auto recvModule = ReceiverSocket(RECEIVERS[rIndex].c_str(), SENDERS_ID[rIndex], *queue);
        std::thread recvLanThread = std::thread(&ReceiverSocket::StartReceiving, recvModule);
        recvLanThread.detach();

    }


    /******************************************************************/
    /** Senders                                                       */
    /******************************************************************/
    std::vector<std::string> SENDERS = {SENDER_LAN_NAME1,SENDER_LAN_NAME2};
//    auto send_to_wifi = SenderSocket(WIFI_INTERFACE_NAME,WIFI_INTERFACE_ID,1,0);
//    auto send_to_lan = SenderSocket(LAN_INTERFACE_NAME,LAN_INTERFACE_ID,1,1);
//    std::thread sendWiFiThread(&SenderSocket::StartTransmitting,send_to_wifi);
//    std::thread sendLanThread(&SenderSocket::StartTransmitting,send_to_lan);
    for(int sIndex = 0 ; sIndex < SENDERS.size() ; sIndex++){
        auto sendModule = SenderSocket(SENDERS[sIndex].c_str(),SENDERS_ID[sIndex],1000,sIndex);
        std::thread sendLanThread(&SenderSocket::StartTransmitting,sendModule);
        sendLanThread.detach();
    }

    /******************************************************************/
    /** Concatinator                                                  */
    /******************************************************************/
    auto concatinator = Concatinator(CONCAT_INTERFACE_ID,OUTPUT_LAN_INTERFACE_NAME);
    concatinator.addQueue(queues_vector.at(0));
    concatinator.addQueue(queues_vector.at(1));
    std::thread concatinatorThread(&Concatinator::startConcatinator,concatinator);


    /******************************************************************/
    /** Final receiver                                                */
    /******************************************************************/
    //```q``` is defined but it is not used;
//    auto q = SharedQueue<FPGA_Packet>();
//    auto final_recv_module = ReceiverSocket(FINAL_LAN_INTERFACE_NAME,LAN_INTERFACE_ID,std::ref(q));
//    std::thread finalReceiverModule(&ReceiverSocket::StartGrabbing,final_recv_module);


    while(true){
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }

//    sendLanThread.join();
//    sendWiFiThread.join();
//
//    recvLanThread.join();
//    recvWiFiThread.join();




    return 0;
}