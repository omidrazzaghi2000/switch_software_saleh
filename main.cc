#include <iostream>

#include <receiver_socket/receiver_socket.hh>

int main(){

    std::cout << "In the name of Allah" << std::endl;

    ReceiverSocket recv1 = ReceiverSocket("wlx3460f954dcac");
    
    recv1.StartReceiving();

    return 0;
}