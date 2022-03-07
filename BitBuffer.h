//
// Created by gunSlaveUnit on 07.03.2022.
//

#pragma once

#ifndef ADAPVITVEHAFFMAN_BITBUFFER_H
#define ADAPVITVEHAFFMAN_BITBUFFER_H

#include <cstdint>
#include <vector>


class BitBuffer {
public:
    explicit BitBuffer(int size);
    void append(int value);
    void setCurrent(int current);
    int getCurrent();
    int get(int index);
private:
    static constexpr int BITES_IN_BYTE = 8;
    static constexpr unsigned char masks[BITES_IN_BYTE] = {
            0b00000001, 0b00000010, 0b00000100, 0b00001000,
            0b00010000, 0b00100000, 0b01000000, 0b10000000
    };
    int size;
    int currentBit{};
    std::vector<unsigned char> bytes;
};


#endif //ADAPVITVEHAFFMAN_BITBUFFER_H
