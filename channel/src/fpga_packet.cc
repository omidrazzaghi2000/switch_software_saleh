//
// Created by nor on 5/25/24.
//

#include "channel/fpga_packet.hh"

FPGA_Packet::FPGA_Packet(const char *raw_data, unsigned int numbyte) {

    /* extract id */
    source_id = "";
    for(int i = START_INDEX_ID; i <= END_INDEX_ID ; i++){
       source_id += raw_data[i];
    }

    /* find frame number */
    std::string temp_frame_number_str = "";
    temp_frame_number_str+=raw_data[MSB_FRAME_NUM_INDEX];
    temp_frame_number_str+=raw_data[LSB_FRAME_NUM_INDEX];
    frame_number = std::stoi(temp_frame_number_str, 0, 16);

    /* extract data */
    data = "";
    for(int i = LSB_FRAME_NUM_INDEX+1; i < numbyte ; i++){
        data += raw_data[i];
    }
}
