//
// Created by nor on 5/25/24.
//
#include "concatinator/concatinator.hh"

void Concatinator::startConcatinator() {

    isActive = true;
    FPGA_Packet curPacket;

    while(isActive){

        curPacket = _queue->front();
        //TODO:.....

    }
}
