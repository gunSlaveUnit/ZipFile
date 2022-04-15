//
// Created by gunSlaveUnit on 13.04.2022.
//

#include <iostream>
#include "AdaptiveHuffmanCoder.h"

void AdaptiveHuffmanCoder::encode(const std::string &filename) {
    clear();

    std::ifstream fin(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (!fin.is_open())
        throw std::runtime_error((std::stringstream("Can' open file ") << filename << " to encode data").str());

    std::filesystem::path path(filename);
    auto outFilename = (std::stringstream() << path.stem().string() << EXT).str();
    ByteWriter writer(outFilename);

    /* esc-узел, пустой, с нулевым весом (частой) */
    escNode = std::make_unique<Node>();
    escNode->s = '\0';
    escNode->w = 0;

    tree = escNode.get(); // инициализация пустого дерева esc-узлом

    /*TODO: надо как-то свести к минимуму количество запросов за памятью */
    uint32_t fileSize = static_cast<uint32_t>(fin.tellg());
    //nodes.resize(fileSize / 2);
    nodes.resize(1);

    fin.seekg(0);

    auto fileExtToEncode = path.extension().string().erase(0, 1);
    uint_fast32_t extLength = fileExtToEncode.length();
    writer.writeByte(extLength);
    for(unsigned char c : fileExtToEncode)
        writer.writeByte(c);

    uint32_t counter = 0;

    while (!fin.eof()) {
        unsigned char ch;
        fin.read((char *) &ch, sizeof(char));

        if (cache[ch & 0xff]) {
            /* Символ уже есть */
            /* Получаем для него код, выдаем код в выходной поток */
            getCodeFor(ch, writer);
        } else {
            /* Символа еще нет */
            /* Получаем код для esc-узла, выдаем его код в выходную последовательность */
            /* Затем выдаем незакодированный символ в выходной поток */
            unsigned char nullSymbol = '\0';
            getCodeFor(nullSymbol, writer);
            writer.writeByte(ch);
        }

        update(ch); // обновление дерева

        ++counter;
    }

    fin.close();
    writer.close();
}

void AdaptiveHuffmanCoder::decode(const std::string &filename) {
    clear();

    std::ifstream fin(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (!fin.is_open())
        throw std::runtime_error((std::stringstream("Can' open file ") << filename << " to encode data").str());

    /* esc-узел, пустой, с нулевым весом (частой) */
    escNode = std::make_unique<Node>();
    escNode->s = '\0';
    escNode->w = 0;

    tree = escNode.get(); // инициализация пустого дерева esc-узлом

    /*TODO: надо как-то свести к минимуму количество запросов за памятью */
    uint32_t fileSize = static_cast<uint32_t>(fin.tellg());
    //nodes.resize(fileSize / 2);
    nodes.resize(1);

    fin.seekg(0);

    std::filesystem::path path(filename);
    ByteWriter writer((std::stringstream() << path.stem().string() << ".txt").str());

    uint32_t counter = 0;

    mode = UNENCODED_BYTE;
    decoderBuffer = BitBuffer();

    while (!fin.eof() && counter < fileSize) {
        unsigned char ch;
        fin.read((char *) &ch, sizeof(char));

        handleByte(ch, writer);

        ++counter;
    }

    fin.close();
    writer.close();
}

void AdaptiveHuffmanCoder::clear() {
    escNode = nullptr;
    tree = nullptr;
    nodes.clear();
    for (int i = 0; i < 256; ++i)
        cache[i] = nullptr;
}

bool AdaptiveHuffmanCoder::reorderWeights() {
    // Нужно найти узел, который нарушает упорядоченность
    int i = 1;
    for (; i < nodes.size(); ++i)
        if (nodes[i - 1]->w < nodes[i]->w)
            break;

    if (i != nodes.size()) {
        auto offender = nodes[i]; // Узел, нарушающий упорядоченность

        // Ищем узел, с которым требуется поменять узел, нарушающий порядок
        int j = 0;
        for (; j < i; ++j)
            if (nodes[j]->w < offender->w)
                break;

        // Меняем два узла в дереве местами
        auto offenderParent = offender->p;
        auto replacement = nodes[j]; // Узел, с которым надо поменять

        if (offender->p == replacement->p) {
            // Если они потомки одного узла, просто меняем их местами в зависимости от веса
            if (offender->w < replacement->w) {
                offenderParent->l = offender;
                offenderParent->r = replacement;
            } else {
                offenderParent->l = replacement;
                offenderParent->r = offender;
            }
        } else {
            if (replacement->p != offender) {
                // Ставим узел, который нарушает порядок, на нужное место
                if (replacement->p->l == replacement)
                    replacement->p->l = offender;
                else
                    replacement->p->r = offender;

                // Меняем родителя для узла, нарушающего порядок
                offender->p = replacement->p;

                // Ставим узел, с которым был обмен, на старое место узла, нарушающего порядок
                if (offenderParent->l == offender)
                    offenderParent->l = replacement;
                else
                    offenderParent->r = replacement;

                // Связываем новый "верный" узел с родителем "неверного узла"
                replacement->p = offenderParent;
            }
        }

        // Меняем эти два узла местами, но уже в списке
        nodes[j] = offender;
        nodes[i] = replacement;
        return true;
    }
    return false;
}

void AdaptiveHuffmanCoder::update(const unsigned char &value) {
    auto checkingIndex = value & 0xff;
    Node *parent; // родитель узла, для обновления веса
    auto node = cache[checkingIndex];
    if (node) {
        /* Узел с такими данными уже есть */
        /* Добавляем вес, отмечаем его родителя для корректировки весов */
        node->w++;
        parent = node->p;
    } else {
        /* Узла с такими данными нет */
        auto escNodeParent = escNode->p;

        /* Создаем узел с такими данными */
        auto *newNode = new Node{};
        newNode->s = value;
        newNode->w = 1;
        newNode->l = newNode->r = newNode->p = nullptr;

        /* Создаем новый промежуточный добавляемый узел */
        auto *intermediate = new Node{};
        intermediate->s = '\0';
        intermediate->w = 0;
        intermediate->l = escNode.get();
        intermediate->r = newNode;
        intermediate->p = escNodeParent;

        cache[checkingIndex] = newNode;

        newNode->p = intermediate;

        /* Определяем, как будет использовать промежуточный узел */
        if (escNodeParent)
            escNodeParent->l = intermediate;
        else
            tree = intermediate;

        /* Добавляем узлы в список узлов */
        nodes[nodes.size() - 1] = intermediate;
        nodes.push_back(newNode);
        nodes.push_back(escNode.get());

        escNode->p = intermediate;
        parent = newNode->p;
    }

    /* Обновляем веса всех родительских узлов для нового добавленного узла */
    while (parent) {
        parent->w++;
        parent = parent->p;
    }

    /* Меняем узлы местами, если требуется */
    while (reorderWeights())
        tree->updateWeights();
}

void AdaptiveHuffmanCoder::getCodeFor(const unsigned char &value, ByteWriter &writer) {
    BitBuffer bitBuffer;

    Node *node;
    if (value)
        node = cache[value & 0xff];
    else
        node = escNode.get();

    /* Выполняем проход от узла до корня, собирая код узла */
    while (node->p) {
        if (node->p->l == node)
            bitBuffer.append(0);
        else
            bitBuffer.append(1);
        node = node->p;
    }

    /* Переворачиваем, поскольку требуется пройти именно от корня к узлу */
    for (int i = bitBuffer.getCurrent() - 1; i >= 0; --i)
        writer.writeBit(bitBuffer.get(i));
}

void AdaptiveHuffmanCoder::handleBit(const unsigned char &bit, ByteWriter &writer) {
    if (mode == UNENCODED_BYTE) { // если мы сейчас читаем незакодированный символ, то пишем бит в буфер, пока не накопится 8 бит
        decoderBuffer.append(bit);
        if (decoderBuffer.getCurrent() == 8) {   // если накопили 8 бит, то
            update(decoderBuffer.bytes[0]);    // обновляем модель считанным незакодированным символом
            writer.writeByte(decoderBuffer.bytes[0]);                 // выдаем символ на выход
            decoderBuffer.setCurrent(0);
            mode = ENCODED_CHAR;                          // переключаемся в режим чтения закодированного символа
            currentNode = tree;
        }
    } else {                                            // если мы сейчас читаем закодированный символ
        if (bit == 1) {                                 // если бит == 1
            currentNode = currentNode->r;            // делаем шаг по дереву направо
        } else {
            currentNode = currentNode->l;             // иначе делаем шаг по дереву налево
        }
        if (currentNode == escNode.get()) {   // если мы пришли в escape символ,
            mode = UNENCODED_BYTE;                // переключаемся в режим чтения незакодированного символа
        }
        if (currentNode->s) {                  // если пришли в обычный узел
            update(currentNode->s); // обновляем модель декодирванным символом
            writer.writeByte(currentNode->s);               // выдаем декодированный символ на выход
            currentNode = tree;                // возвращаемся в начало дерева
        }
    }
}

void AdaptiveHuffmanCoder::handleByte(const unsigned char &byte, ByteWriter &writer) {
    unsigned char mask = 1;
    for (int i = 0; i < 8; ++i) {
        handleBit((byte & mask) > 0 ? 1 : 0, writer);
        mask <<= 1;
    }
}
