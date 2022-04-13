//
// Created by gunSlaveUnit on 13.04.2022.
//

#ifndef ADAPTIVE_HUFFMAN_CODER_H
#define ADAPTIVE_HUFFMAN_CODER_H

#include <string>


class Coder {
public:
    virtual void encode(const std::string &filename) = 0;
    virtual void decode(const std::string &filename) = 0;
    virtual ~Coder();
};


#endif //ADAPTIVE_HUFFMAN_CODER_H
