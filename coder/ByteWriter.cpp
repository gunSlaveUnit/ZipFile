//
// Created by gunSlaveUnit on 07.03.2022.
//

#include <iostream>

#include "ByteWriter.h"

ByteWriter::ByteWriter(const std::string &outputFileName) : bitWritten(0), bytesWritten(0) {
    output = new std::ofstream(outputFileName, std::ios::out | std::ios::binary);
    if (!output->is_open())
        throw std::runtime_error((std::stringstream() << "Can't open file " << outputFileName).str());
}

void ByteWriter::writeBit(const unsigned char &bit) {
    unsigned char bitIndex = bitWritten & 0b111;
    unsigned char mask = 1 << bitIndex;
    switch (bit) {
        case 0:
            bitBuffer &= ~mask;
            break;
        case 1:
            bitBuffer |= mask;
    }
    ++bitWritten;
    if (bitIndex + 1 == 8) {
        *output << bitBuffer;
        ++bytesWritten;
    }
}

void ByteWriter::writeByte(const unsigned char &value) {
    unsigned char mask = 1;
    for (int i = 0; i < 8; ++i) {
        writeBit((value & mask) > 0 ? 1 : 0);
        mask <<= 1;
    }
}

void ByteWriter::close() {
    if (bitWritten & 0b111) {
        *output << bitBuffer;
        ++bytesWritten;
    }
    output->flush();
    output->close();
}
