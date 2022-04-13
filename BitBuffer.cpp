//
// Created by gunSlaveUnit on 07.03.2022.
//

#include "BitBuffer.h"

void BitBuffer::append(int value) {
    int byteIndex = currentBit >> 3;
    int bitIndex = currentBit & 0b111;

    if (value != 0)
        bytes[byteIndex] = bytes[byteIndex] | masks[bitIndex];
    else
        bytes[byteIndex] = bytes[byteIndex] & ~masks[bitIndex];

    ++currentBit;
}

BitBuffer::BitBuffer() : size(8), currentBit(0) {
    int sizeInBytes = size >> 3;

    if(size % 8 > 0)
        ++sizeInBytes;

    bytes.resize(sizeInBytes);
}

int BitBuffer::get(int index) {
    int byteIndex = index >> 3;
    int bitIndex = index & 0b111;
    return (bytes[byteIndex] & masks[bitIndex]) != 0 ? 1 : 0;
}

void BitBuffer::setCurrent(int current) {
    this->currentBit = current;
}

int BitBuffer::getCurrent() {
    return currentBit;
}
