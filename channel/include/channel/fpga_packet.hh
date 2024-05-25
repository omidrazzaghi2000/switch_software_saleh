//
// Created by nor on 5/25/24.
//

#ifndef OSWITCH_FPGA_PACKET_HH
#define OSWITCH_FPGA_PACKET_HH

#include <iostream>

#define START_INDEX_ID 1
#define END_INDEX_ID 10
#define MSB_FRAME_NUM_INDEX 17
#define LSB_FRAME_NUM_INDEX 18

class FPGA_Packet {
public:
    FPGA_Packet()=default;
    FPGA_Packet(const char * raw_data, unsigned int numbyte);
    std::string source_id;
    int frame_number;
    std::string data;

private:
};


#endif //OSWITCH_FPGA_PACKET_HH
