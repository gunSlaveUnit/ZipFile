cmake_minimum_required(VERSION 3.21)
project(AdaptiveHuffman)

set(CMAKE_CXX_STANDARD 23)

add_executable(AdaptiveHuffman
        main.cpp
        BitBuffer.cpp BitBuffer.h
        ByteWriter.cpp ByteWriter.h
        Coder.cpp Coder.h
        AdaptiveHuffmanCoder.cpp AdaptiveHuffmanCoder.h Node.cpp Node.h)
