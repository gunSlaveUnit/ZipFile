#include <iostream>
#include <fstream>
#include <array>
#include <memory>
#include <vector>

#include "BitBuffer.h"

// uint8_t?

std::ofstream out("War and Peace.ahf");

class FileWriter {
public:
    void writeBit(unsigned char bit) {
        unsigned char bitIndex = bitWritten & 0b111;
        unsigned char mask = 1 << bitIndex;
        switch (bit) {
            case 1:
                bitBuffer |= mask;
                break;
            case 0:
                bitBuffer &= ~mask;
                break;
        }
        bitWritten += 1;
        if (bitIndex + 1 == 8) {
            out << bitBuffer;
            bytesWritten += 1;
            //std::cout<<bytesWritten<<std::endl;
        }
    }
    void writeByte(unsigned char byte) {
        unsigned char mask = 1;
        for (int i = 0; i < 8; i++) {
            writeBit((byte & mask) > 0 ? 1 : 0);
            mask <<= 1;
        }
    }
    void close() {
        if ((bitWritten & 0b111) != 0) {
            out << bitBuffer;
            bytesWritten += 1;
        }
        out.flush();
        out.close();
    }
private:
    unsigned char bitBuffer;

    int bitWritten;
    int bytesWritten;
} fileWriter;

struct Node {
    uint32_t updateWeights() {
        if (s != '\0')
            return w;
        else {
            uint32_t weight = 0;
            if (l)
                weight += l->updateWeights();
            if (r)
                weight += r->updateWeights();

            w = weight;

            return w;
        }
    }

    unsigned char s;
    uint32_t w;
    Node *l, *r, *p;
};

std::unique_ptr<Node> escNode = nullptr;
Node* tree = nullptr;
std::vector<Node*> nodes; // узлы, отсортированные по весам
std::array<Node*, 256> cache;

bool reorderWeights() {
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

void update(unsigned char& value) {
    auto checkingIndex = value & 0xff;
    Node* parent; // родитель узла, для обновления веса
    auto node = cache[checkingIndex];
    if(node) {
        /* Узел с такими данными уже есть */
        /* Добавляем вес, отмечаем его родителя для корректировки весов */
        node->w++;
        parent = node->p;
    }
    else {
        /* Узла с такими данными нет */
        auto escNodeParent = escNode->p;

        /* Создаем узел с такими данными */
        auto* newNode = new Node{};
        newNode->s = value;
        newNode->w = 1;
        newNode->l = newNode->r = newNode->p = nullptr;

        /* Создаем новый промежуточный добавляемый узел */
        auto* intermediate = new Node{};
        intermediate->s = '\0';
        intermediate->w = 0;
        intermediate->l = escNode.get();
        intermediate->r = newNode;
        intermediate->p = escNodeParent;

        cache[checkingIndex] = newNode;

        newNode->p = intermediate;

        /* Определяем, как будет использовать промежуточный узел */
        if(escNodeParent)
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
    while(parent) {
        parent->w++;
        parent = parent->p;
    }

    /* Меняем узлы местами, если требуется */
    while(reorderWeights())
        tree->updateWeights();
}

void getCodeFor(unsigned char& value) {
    BitBuffer bitBuffer(8);

    Node* node;
    if (value)
        node = cache[value & 0xff];
    else
        node = escNode.get();

    /* Выполняем проход от узла до корня, собирая код узла */
    while(node->p) {
        if(node->p->l == node)
            bitBuffer.append(0);
        else
            bitBuffer.append(1);
        node = node->p;
    }

    /* Переворачиваем, поскольку требуется пройти именно от корня к узлу */
    for (int i = bitBuffer.getCurrent() - 1; i >= 0 ; --i)
        fileWriter.writeBit(bitBuffer.get(i));
}

int main(int argc, char **argv) {
    std::ifstream fin("War and Peace.txt", std::ios::in | std::ios::binary | std::ios::ate);
    if(!fin.is_open())
        std::cout<<"no file";

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
    uint32_t counter = 0;

    while(!fin.eof() && counter < fileSize) {
        unsigned char ch;
        fin.read((char*)&ch, sizeof(char));

        if (cache[ch & 0xff]) {
            /* Символ уже есть */
            /* Получаем для него код, выдаем код в выходной поток */
            getCodeFor(ch);
        }
        else {
            /* Символа еще нет */
            /* Получаем код для esc-узла, выдаем его код в выходную последовательность */
            /* Затем выдаем незакодированный символ в выходной поток */
            unsigned char nullSymbol = '\0';
            getCodeFor(nullSymbol);
            fileWriter.writeByte(ch);
        }

        update(ch); // обновление дерева

        ++counter;
    }

    fin.close();
    fileWriter.close();
}