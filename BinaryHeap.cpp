#include "BinaryHeap.h"

namespace huffman {

    const size_t ROOT = 0;

    size_t BinaryHeap::LeftSon(size_t i) {
        return 2 * i + 1;
    }

    size_t BinaryHeap::RightSon(size_t i) {
        return 2 * i + 2;
    }

    size_t BinaryHeap::Parent(size_t i) {
        return (i - 1) / 2;
    }

    void BinaryHeap::SiftDown(size_t i) {
        while (LeftSon(i) < binary_heap_.size()) {
            size_t left = LeftSon(i);
            size_t right = RightSon(i);
            size_t j = left;
            if (right < binary_heap_.size() && binary_heap_[right].first < binary_heap_[left].first) {
                j = right;
            }
            if (binary_heap_[i].first < binary_heap_[j].first) {
                break;
            }
            std::swap(binary_heap_[i], binary_heap_[j]);
            i = j;
        }
    }

    void BinaryHeap::SiftUp(size_t i) {
        while (binary_heap_[i].first < binary_heap_[Parent(i)].first) {
            std::swap(binary_heap_[i], binary_heap_[Parent(i)]);
            i = Parent(i);
        }
    }

    void BinaryHeap::Pop() {
        binary_heap_[ROOT] = binary_heap_.back();
        binary_heap_.pop_back();
        SiftDown(ROOT);
    }

    WeightTree BinaryHeap::GetMin() {
        return binary_heap_[ROOT];
    }

    void BinaryHeap::Insert(const WeightTree &added_tree) {
        binary_heap_.emplace_back(added_tree);
        SiftUp(binary_heap_.size() - 1);
    }

    bool BinaryHeap::IsEmpty() const {
        return binary_heap_.empty();
    }

    size_t BinaryHeap::Size() {
        return binary_heap_.size();
    }

}
