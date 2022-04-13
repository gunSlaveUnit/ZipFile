//
// Created by gunSlaveUnit on 13.04.2022.
//

#ifndef ADAPTIVE_HUFFMAN_ADAPTIVE_HUFFMAN_CODER_H
#define ADAPTIVE_HUFFMAN_ADAPTIVE_HUFFMAN_CODER_H

#include <memory>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Coder.h"
#include "Node.h"
#include "BitBuffer.h"
#include "ByteWriter.h"


class AdaptiveHuffmanCoder final: Coder {
public:
    void encode(const std::string &filename) override;
    void decode(const std::string &filename) override;
    void clear();
private:
    bool reorderWeights();
    void update(unsigned char& value);
    void getCodeFor(unsigned char& value, ByteWriter& writer);
    void handleBit(int bit, ByteWriter& writer);
    void write(unsigned char b, ByteWriter& writer);

    std::unique_ptr<Node> escNode;
    Node* tree;
    std::vector<Node*> nodes; // узлы, отсортированные по весам
    std::array<Node*, 256> cache;
    const std::string EXT = ".ahf";
    enum DECODER_MODES {UNENCODED_BYTE, ENCODED_CHAR};
    Node* currentNode;
    int mode;
    BitBuffer decoderBuffer;
};


#endif //ADAPTIVE_HUFFMAN_ADAPTIVE_HUFFMAN_CODER_H
