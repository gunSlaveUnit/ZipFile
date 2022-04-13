//
// Created by gunSlaveUnit on 07.03.2022.
//

#pragma once

#ifndef ADAPTIVE_HUFFMAN_BYTE_WRITER_H
#define ADAPTIVE_HUFFMAN_BYTE_WRITER_H

#include <fstream>

class ByteWriter {
public:
    explicit ByteWriter(const std::string& outputFileName);
    void writeBit(unsigned char bit);
    void writeByte(unsigned char value);
    void close();
private:
    unsigned char bitBuffer;

    int bitWritten;
    int bytesWritten;

    std::ofstream* output;
};


#endif //ADAPTIVE_HUFFMAN_BYTE_WRITER_H
