//
// Created by gunSlaveUnit on 07.03.2022.
//

#include "BitBuffer.h"

BitBuffer::BitBuffer() : size(8), currentBit(0) {
    uint_fast32_t sizeInBytes = size >> 3;

    if (size % 8 > 0)
        ++sizeInBytes;

    bytes.resize(sizeInBytes);
}

void BitBuffer::append(const unsigned char &bit) {
    uint_fast32_t byteIndex = currentBit >> 3;
    uint_fast32_t bitIndex = currentBit & 0b111;

    if (bit != 0)
        bytes[byteIndex] = bytes[byteIndex] | masks[bitIndex];
    else
        bytes[byteIndex] = bytes[byteIndex] & ~masks[bitIndex];

    ++currentBit;
}

uint_fast32_t BitBuffer::get(const uint_fast32_t &index) const {
    uint_fast32_t byteIndex = index >> 3;
    uint_fast32_t bitIndex = index & 0b111;
    return (bytes[byteIndex] & masks[bitIndex]) != 0 ? 1 : 0;
}

void BitBuffer::setCurrent(const uint_fast32_t &current) {
    this->currentBit = current;
}

uint_fast32_t BitBuffer::getCurrent() const {
    return currentBit;
}
