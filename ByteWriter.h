//
// Created by gunSlaveUnit on 07.03.2022.
//

#pragma once

#ifndef ADAPVITVEHAFFMAN_BYTEWRITER_H
#define ADAPVITVEHAFFMAN_BYTEWRITER_H

#include <fstream>

class ByteWriter {
public:
    explicit ByteWriter(const std::string& outputFileName);
    void writeBit(int bit);
    void writeByte(unsigned char value);
    void close();
private:
    unsigned char bitBuffer;

    int bitWritten;
    int bytesWritten;

    std::ofstream output;
};


#endif //ADAPVITVEHAFFMAN_BYTEWRITER_H
