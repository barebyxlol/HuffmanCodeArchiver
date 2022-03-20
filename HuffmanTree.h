#pragma once

#include "BitOperations.h"
#include <vector>
#include <iostream>

namespace huffman {

    const int LETTER_SIZE = 9;
    using Letter = std::bitset<LETTER_SIZE>;
    using Code = std::vector<bool>;
    using EncodedLetter = std::pair<Letter, Code>;


    class HuffmanTree {
    private:
        class TreeNode {
        public:
            explicit TreeNode(Letter letter);

            [[nodiscard]] bool IsTerminate() const;

        public:
            TreeNode *left;
            TreeNode *right;
            Letter letter;
        };

    public:
        explicit HuffmanTree(Letter letter);

        ~HuffmanTree();

        static HuffmanTree *MergeTrees(const HuffmanTree &tree_a, const HuffmanTree &tree_b);

        [[nodiscard]] std::vector<EncodedLetter> GetCodes() const;

    private:
        static void
        TreeDFS(TreeNode *node, std::vector<EncodedLetter> &encoded_letters, std::vector<bool> current_letter);

    private:
        TreeNode *root_;

    private:
        static void Delete(TreeNode *tree_node);
    };

}  // namespace huffman
