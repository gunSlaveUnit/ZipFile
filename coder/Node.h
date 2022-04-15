//
// Created by gunSlaveUnit on 13.04.2022.
//

#ifndef ADAPTIVE_HUFFMAN_NODE_H
#define ADAPTIVE_HUFFMAN_NODE_H

#include <cstdint>


struct Node {
    uint_fast32_t updateWeights();

    unsigned char s;
    uint_fast32_t w;
    Node *l, *r, *p;
};


#endif //ADAPTIVE_HUFFMAN_NODE_H
