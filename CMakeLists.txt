cmake_minimum_required(VERSION 3.8)

project(archiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++20")

set(SOURCE_FILES main.cpp BitOperations.h BitOperations.cpp HuffmanCode.h HuffmanCode.cpp HuffmanTree.h HuffmanTree.cpp AlgorithmProcessing.h AlgorithmProcessing.cpp BinaryHeap.h BinaryHeap.cpp)

add_executable(archiver ${SOURCE_FILES})
