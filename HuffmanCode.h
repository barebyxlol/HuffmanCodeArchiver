#pragma once

#include <fstream>
#include "BitOperations.h"
#include "BinaryHeap.h"
#include <unordered_map>
#include <queue>
#include <deque>

namespace huffman {

    class Encoder {
    public:
        explicit Encoder(std::ofstream &archive);

        ~Encoder();

        void EncodeFile(const std::string &file_name);

        void CloseEncoder();

    private:
        BitWriter archive_;
        bool is_empty_;
        std::unordered_map<Letter, size_t> letters_count_;
        std::unordered_map<Letter, Code> letters_codes_;
        BinaryHeap list_of_trees_;
        std::vector<EncodedLetter> encoded_letters_;

    private:
        static bool CompareEncodedLetters(const EncodedLetter &letter_a, const EncodedLetter &letter_b);

        static void CodesToCanonical(std::vector<EncodedLetter> &encoded_letters);

        static Code SizeToCode(size_t number);
    };

    class Decoder {
    public:
        explicit Decoder(std::ifstream &archive);

        void Decode();

    private:
        BitReader archive_;
        std::unordered_map<Code, Letter> letters_codes_;

    private:
        static Letter CodeToLetter(Code code);

        static std::unordered_map<Code, Letter> GetCanonicalCodes(const std::vector<Letter> &letters,
                                                                  std::vector<size_t> &code_length);

        Letter GetLetter();
    };

}  // namespace huffman
