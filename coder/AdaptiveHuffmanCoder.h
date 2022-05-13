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
#include <QtCore>
#include <QObject>

#include "Coder.h"
#include "Node.h"
#include "BitBuffer.h"
#include "ByteWriter.h"


class AdaptiveHuffmanCoder final: public QObject, Coder {
    Q_OBJECT
public:
    void encode(const std::string &filename) override;
    void decode(const std::string &filename) override;
    void clear();
signals:
    void send_opened_filename(const std::string &);
    void send_created_filename(const std::string &);
    void byte_processed(const uint_fast32_t &, const uint_fast32_t &);
private:
    bool reorderWeights();
    void update(const unsigned char &value);
    void getCodeFor(const unsigned char &value, ByteWriter &writer);
    void handleBit(const unsigned char &bit, ByteWriter &writer);
    void handleByte(const unsigned char &byte, ByteWriter &writer);

    std::unique_ptr<Node> escNode;
    Node* tree;
    std::vector<Node*> nodes; // узлы, отсортированные по весам
    std::array<Node*, 256> cache;
    const std::string EXT = ".ahf";
    enum DECODER_MODES {UNENCODED_BYTE, ENCODED_CHAR};
    Node* currentNode;
    int mode;
    BitBuffer decoderBuffer;
    uint_fast32_t processed_bytes_amount;
};


#endif //ADAPTIVE_HUFFMAN_ADAPTIVE_HUFFMAN_CODER_H
