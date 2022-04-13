//
// Created by gunSlaveUnit on 13.04.2022.
//

#ifndef ADAPTIVE_HUFFMAN_ADAPTIVE_HUFFMAN_CODER_H
#define ADAPTIVE_HUFFMAN_ADAPTIVE_HUFFMAN_CODER_H

#include "Coder.h"


class AdaptiveHuffmanCoder final: Coder {
    void encode(const std::string &filename) override;
    void decode(const std::string &filename) override;
};


#endif //ADAPTIVE_HUFFMAN_ADAPTIVE_HUFFMAN_CODER_H
