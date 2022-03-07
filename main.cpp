#include <iostream>
#include <fstream>
#include <array>
#include <memory>
#include <vector>

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

std::vector<unsigned char> getCodeFor(unsigned char& value) {
    std::vector<unsigned char> binaryCode;

    Node* node;
    if (value)
        node = cache[value & 0xff];
    else
        node = escNode.get();

    /* Выполняем проход от узла до корня, собирая код узла */
    while(node->p) {
        if(node->p->l == node)
            binaryCode.push_back('0');
        else
            binaryCode.push_back('1');
        node = node->p;
    }

    /* Переворачиваем, поскольку требуется пройти именно от корня к узлу */
    std::reverse(binaryCode.begin(), binaryCode.end());

    return binaryCode;
}

void write(unsigned char& value) {
    //std::ofstream fout("out.txt", std::ios::out | std::ios::binary);
    //if(!fout.is_open())
        //std::cout<<"no file";





    //fout.flush();
    //fout.close();
}

void clear(Node* n) {

}

int main(int argc, char **argv) {
    std::ifstream fin("test.txt", std::ios::in | std::ios::binary | std::ios::ate);
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

    std::vector<unsigned char> encodedSequence; // закодированная выходная последовательность

    while(!fin.eof() && counter < fileSize) {
        unsigned char ch;
        fin.read((char*)&ch, sizeof(char));

        if (cache[ch & 0xff]) {
            /* Символ уже есть */
            /* Получаем для него код, выдаем код в выходной поток */
            auto bitCode = getCodeFor(ch);
            encodedSequence.insert(encodedSequence.end(), bitCode.begin(), bitCode.end());
        }
        else {
            /* Символа еще нет */
            /* Получаем код для esc-узла, выдаем его код в выходную последовательность */
            /* Затем выдаем незакодированный символ в выходной поток */
            unsigned char nullSymbol = '\0';
            auto bitCode = getCodeFor(nullSymbol);
            encodedSequence.insert(encodedSequence.end(), bitCode.begin(), bitCode.end());
            encodedSequence.push_back(ch);
        }

        update(ch); // обновление дерева

        ++counter;
    }

    fin.close();

    for(const auto& c : encodedSequence)
        std::cout<<c<<" ";
}