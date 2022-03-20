#pragma once

#include <cstdint>
#include "HuffmanTree.h"

namespace huffman {

    using Weight = size_t;
    using WeightTree = std::pair<Weight, huffman::HuffmanTree *>;

    class BinaryHeap {
    public:
        BinaryHeap() = default;

        WeightTree GetMin();

        void Insert(const WeightTree &added_tree);

        [[nodiscard]] bool IsEmpty() const;

        void Pop();

        size_t Size();

    private:
        void SiftUp(size_t i);

        void SiftDown(size_t i);

        static size_t LeftSon(size_t i);

        static size_t RightSon(size_t i);

        static size_t Parent(size_t i);
    private:
        std::vector<WeightTree> binary_heap_;
    };

}
