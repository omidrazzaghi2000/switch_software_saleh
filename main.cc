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
#define CONCAT_INTERFACE_ID "cati&%argw"

#define LAN_INTERFACE_NAME "enp0s31f6"
#define WIFI_INTERFACE_NAME "wlx3460f954dcac"

#define SENDER_LAN_NAME1 "enp3s0f2"
#define SENDER_LAN_NAME2 "enp3s0f3"

#define RECEIVER_LAN_NAME1 "enp6s0f2"
#define RECEIVER_LAN_NAME2 "enp6s0f3"

#define OUTPUT_LAN_INTERFACE_NAME "enp6s0f1"
#define FINAL_LAN_INTERFACE_NAME "enp3s0f1"


void appendToVectorRef(std::vector <std::unique_ptr<SharedQueue<FPGA_Packet>>> & vectorRef){
    std::unique_ptr<SharedQueue<FPGA_Packet>> toPush = std::make_unique<SharedQueue<FPGA_Packet>>();
    vectorRef.push_back(std::move(toPush));
}

int main(){

    std::cout << "In the name of Allah" << std::endl;
    std::vector<std::string> RECEIVERS= {RECEIVER_LAN_NAME1,RECEIVER_LAN_NAME2};
    std::vector <SharedQueue<FPGA_Packet> *> queues_vector;
    SharedQueue<FPGA_Packet> temp_shared_queue;//for avoiding copy

    auto wifi_interface_queue = SharedQueue<FPGA_Packet>();
    auto lan_interface_queue = SharedQueue<FPGA_Packet>();

    for(int rIndex = 0 ; rIndex < RECEIVERS.size() ; rIndex++) {

        /******************************************************************/
        /** Interface queues                                              */
        /******************************************************************/
//        auto * queue = new SharedQueue<FPGA_Packet>();
//        std::unique_ptr<SharedQueue<FPGA_Packet>> queue_ptr = std::make_unique<SharedQueue<FPGA_Packet>>();
//        queues_vector.push_back(queue);





        /******************************************************************/
        /** Receivers                                                     */
        /******************************************************************/
//    auto recv_wifi = ReceiverSocket(WIFI_INTERFACE_NAME,WIFI_INTERFACE_ID,std::ref(wifi_interface_queue));
//    auto recv_lan = ReceiverSocket(LAN_INTERFACE_NAME,LAN_INTERFACE_ID,std::ref(lan_interface_queue));
//    std::thread recvWiFiThread(&ReceiverSocket::StartReceiving, recv_wifi);
//    std::thread recvLanThread(&ReceiverSocket::StartReceiving, recv_lan);
        if(rIndex == 0){
            auto recvModule = ReceiverSocket(RECEIVERS[rIndex].c_str(), LAN_INTERFACE_ID, std::ref(wifi_interface_queue));
            std::thread recvLanThread = std::thread(&ReceiverSocket::StartReceiving, recvModule);
            recvLanThread.detach();
        }

        if(rIndex == 1){
            auto recvModule = ReceiverSocket(RECEIVERS[rIndex].c_str(), LAN_INTERFACE_ID, std::ref(lan_interface_queue));
            std::thread recvLanThread = std::thread(&ReceiverSocket::StartReceiving, recvModule);
            recvLanThread.detach();
        }

    }


    std::vector<std::string> SENDERS = {SENDER_LAN_NAME1,SENDER_LAN_NAME2};
    /******************************************************************/
    /** Senders                                                       */
    /******************************************************************/
//    auto send_to_wifi = SenderSocket(WIFI_INTERFACE_NAME,WIFI_INTERFACE_ID,1,0);
//    auto send_to_lan = SenderSocket(LAN_INTERFACE_NAME,LAN_INTERFACE_ID,1,1);
//    std::thread sendWiFiThread(&SenderSocket::StartTransmitting,send_to_wifi);
//    std::thread sendLanThread(&SenderSocket::StartTransmitting,send_to_lan);
    auto msgIndex = 0;
    for(auto &s: SENDERS){
        auto sendModule = SenderSocket(s.c_str(),LAN_INTERFACE_ID,1000000,msgIndex);
        msgIndex = !msgIndex;
        std::thread sendLanThread(&SenderSocket::StartTransmitting,sendModule);
        sendLanThread.detach();
    }

    /******************************************************************/
    /** Concatinator                                                  */
    /******************************************************************/
    auto concatinator = Concatinator(CONCAT_INTERFACE_ID,OUTPUT_LAN_INTERFACE_NAME);
    concatinator.addQueue(&wifi_interface_queue);
    concatinator.addQueue(&lan_interface_queue);
    std::thread concatinatorThread(&Concatinator::startConcatinator,concatinator);
//
//    concatinatorThread.join();


    /******************************************************************/
    /** Final receiver                                                */
    /******************************************************************/
//    auto final_recv_module = ReceiverSocket(FINAL_LAN_INTERFACE_NAME,LAN_INTERFACE_ID, nullptr);
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