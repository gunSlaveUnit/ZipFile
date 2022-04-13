#include "AdaptiveHuffmanCoder.h"


int main(int argc, char **argv) {
    auto coder = AdaptiveHuffmanCoder();
    //coder.encode("alice.txt");
    coder.decode("alice.ahf");
}
