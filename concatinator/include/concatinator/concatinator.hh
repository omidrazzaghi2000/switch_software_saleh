//
// Created by nor on 5/25/24.
//

#ifndef OSWITCH_CONCATINATOR_HH
#define OSWITCH_CONCATINATOR_HH

#include <channel/fpga_packet.hh>
#include <channel/shared_queue.hpp>
#include <utility>

class Concatinator{
public:
    Concatinator(std::shared_ptr<SharedQueue<FPGA_Packet>> _queue):_queue(std::move(_queue)){}
    void startConcatinator();
private:
    std::shared_ptr<SharedQueue<FPGA_Packet>> _queue;
    bool isActive;
};


#endif //OSWITCH_CONCATINATOR_HH
