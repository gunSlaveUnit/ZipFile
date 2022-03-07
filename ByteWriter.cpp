//
// Created by gunSlaveUnit on 07.03.2022.
//

#include <iostream>

#include "ByteWriter.h"

ByteWriter::ByteWriter(const std::string& outputFileName) : bitWritten(0), bytesWritten(0) {
    output = new std::ofstream(outputFileName, std::ios::out | std::ios::binary);
    if(!output->is_open())
        std::cout<<"no file";
}

void ByteWriter::writeBit(int bit) {
    int bitIndex = bitWritten & 0b111;
    int mask = 1 << bitIndex;
    switch (bit) {
        case 0:
            bitBuffer &= ~mask;
            break;
        case 1:
            bitBuffer |= mask;
    }
    bitWritten += 1;
    if (bitIndex + 1 == 8) {
        *output << bitBuffer;
        bytesWritten += 1;
        std::cout<<"Out byte "<<bytesWritten<<std::endl;
    }
}

void ByteWriter::writeByte(unsigned char value) {
    int mask = 1;
    for (int i = 0; i < 8; ++i) {
        writeBit((value & mask) > 0 ? 1 : 0);
        mask <<= 1;
    }
}

void ByteWriter::close() {
    if ((bitWritten & 0b111) != 0) {
        *output << bitBuffer;
        bytesWritten += 1;
    }
    output->flush();
    output->close();
}
