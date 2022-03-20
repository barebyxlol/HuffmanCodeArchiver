#include "HuffmanTree.h"

namespace huffman {
    const Letter MAX_LETTER_VALUE = (1 << 10) - 1;

    HuffmanTree::TreeNode::TreeNode(Letter letter) {
        left = nullptr;
        right = nullptr;
        this->letter = letter;
    }

    bool HuffmanTree::TreeNode::IsTerminate() const {
        if (right == nullptr && left == nullptr) {
            return true;
        }
        return false;
    }

    HuffmanTree::HuffmanTree(Letter letter) {
        root_ = new TreeNode(letter);
    }


    HuffmanTree *HuffmanTree::MergeTrees(const HuffmanTree &tree_a, const HuffmanTree &tree_b) {
        auto *new_tree = new HuffmanTree(MAX_LETTER_VALUE);
        new_tree->root_->left = tree_a.root_;
        new_tree->root_->right = tree_b.root_;
        return new_tree;
    }

    void HuffmanTree::TreeDFS(TreeNode *node, std::vector<EncodedLetter> &encoded_letters,
                              std::vector<bool> current_letter) {
        if (node->IsTerminate()) {
            encoded_letters.emplace_back(node->letter, current_letter);
            current_letter.pop_back();
            return;
        }
        current_letter.push_back(true);
        TreeDFS(node->left, encoded_letters, current_letter);
        current_letter.pop_back();
        current_letter.push_back(false);
        TreeDFS(node->right, encoded_letters, current_letter);
        current_letter.pop_back();
        current_letter.pop_back();
    }

    std::vector<EncodedLetter> HuffmanTree::GetCodes() const {
        std::vector<EncodedLetter> encoded_letters;
        std::vector<bool> current_letter;
        TreeDFS(root_, encoded_letters, current_letter);
        return encoded_letters;
    }

    void HuffmanTree::Delete(HuffmanTree::TreeNode *tree_node) {
        if (tree_node == nullptr) {
            return;
        }
        Delete(tree_node->left);
        Delete(tree_node->right);
        delete tree_node;
    }

    HuffmanTree::~HuffmanTree() {
        Delete(root_);
    }

}  // namespace huffman
