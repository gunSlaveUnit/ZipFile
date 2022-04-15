//
// Created by gunSlaveUnit on 07.03.2022.
//

#pragma once

#ifndef ADAPTIVE_HUFFMAN_BYTE_WRITER_H
#define ADAPTIVE_HUFFMAN_BYTE_WRITER_H

#include <fstream>
#include <sstream>

class ByteWriter {
public:
    explicit ByteWriter(const std::string &outputFileName);
    void writeBit(const unsigned char &bit);
    void writeByte(const unsigned char &value);
    void close();
private:
    unsigned char bitBuffer;

    uint_fast32_t bitWritten;
    uint_fast32_t bytesWritten;

    std::ofstream* output;
};


#endif //ADAPTIVE_HUFFMAN_BYTE_WRITER_H
