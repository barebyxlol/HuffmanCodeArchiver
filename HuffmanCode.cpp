#include "HuffmanCode.h"
#include <tuple>
#include <iostream>

namespace huffman {

    const size_t MAX_CODE_LENGTH = 300;
    const Letter FILENAME_END = 256;
    const Letter ONE_MORE_FILE = 257;
    const Letter ARCHIVE_END = 258;
    const int ONE_BIT = 1;

    Encoder::Encoder(std::ofstream &archive) : archive_(archive), is_empty_(true) {
    }

    bool Encoder::CompareEncodedLetters(const EncodedLetter &letter_a, const EncodedLetter &letter_b) {
        return std::make_tuple(letter_a.second.size(), letter_a.first.to_ulong()) <
               std::make_tuple(letter_b.second.size(), letter_b.first.to_ulong());
    }

    void Encoder::CodesToCanonical(std::vector<EncodedLetter> &encoded_letters) {
        std::sort(encoded_letters.begin(), encoded_letters.end(), Encoder::CompareEncodedLetters);
        size_t code = 0;
        std::bitset<MAX_CODE_LENGTH> binary_code;
        for (size_t i = 0; i < encoded_letters.size(); ++i) {
            binary_code = code;
            for (size_t j = 0; j < encoded_letters[i].second.size(); ++j) {
                encoded_letters[i].second[j] = binary_code[j];
            }
            ++code;
            if (i + 1 < encoded_letters.size()) {
                code = code << (encoded_letters[i + 1].second.size() - encoded_letters[i].second.size());
            }
        }
    }

    Code Encoder::SizeToCode(size_t number) {
        std::bitset<LETTER_SIZE> b = number;
        Code code;
        for (size_t i = 0; i < LETTER_SIZE; ++i) {
            code.push_back(b[i]);
        }
        return code;
    }

    void Encoder::EncodeFile(const std::string &file_name) {
        if (!is_empty_) {
            archive_.Put(letters_codes_[ONE_MORE_FILE]);
        }

        letters_count_.clear();
        letters_codes_.clear();
        encoded_letters_.clear();
        while (!list_of_trees_.IsEmpty()) {
            list_of_trees_.Pop();
        }
        std::ifstream file(file_name, std::ifstream::binary);
        while (file.peek() != EOF) {
            ++letters_count_[file.get()];
        }
        file.close();
        for (unsigned char c: file_name) {
            ++letters_count_[c];
        }
        letters_count_[FILENAME_END] = 1;
        letters_count_[ONE_MORE_FILE] = 1;
        letters_count_[ARCHIVE_END] = 1;
        for (const auto &letter: letters_count_) {
            list_of_trees_.Insert(WeightTree(letter.second, new HuffmanTree(letter.first)));
        }
        while (list_of_trees_.Size() > 1) {
            WeightTree tree_a = list_of_trees_.GetMin();
            list_of_trees_.Pop();
            WeightTree tree_b = list_of_trees_.GetMin();
            list_of_trees_.Pop();
            list_of_trees_.Insert(
                    WeightTree(tree_a.first + tree_b.first, HuffmanTree::MergeTrees(*tree_a.second, *tree_b.second)));
        }

        encoded_letters_ = list_of_trees_.GetMin().second->GetCodes();
        CodesToCanonical(encoded_letters_);
        for (auto &letter: encoded_letters_) {
            std::reverse(letter.second.begin(), letter.second.end());
            letters_codes_[letter.first] = letter.second;
        }

        archive_.Put(SizeToCode(encoded_letters_.size()));
        for (const auto &letter: encoded_letters_) {
            archive_.Put(SizeToCode(letter.first.to_ulong()));
        }
        size_t max_code_length = encoded_letters_.back().second.size();
        std::vector<int> code_length(max_code_length, 0);
        for (const auto &letter: encoded_letters_) {
            ++code_length[letter.second.size() - 1];
        }
        for (const auto &length: code_length) {
            archive_.Put(SizeToCode(length));
        }

        file = std::ifstream(file_name, std::ifstream::binary);
        for (unsigned char c: file_name) {
            archive_.Put(letters_codes_[c]);
        }
        archive_.Put(letters_codes_[FILENAME_END]);

        while (file.peek() != EOF) {
            unsigned char c = file.get();
            archive_.Put(letters_codes_[c]);
        }
        file.close();
        is_empty_ = false;
    }

    void Encoder::CloseEncoder() {
        archive_.LastPut(letters_codes_[ARCHIVE_END]);
    }

    Encoder::~Encoder() {
        CloseEncoder();
    }

    Decoder::Decoder(std::ifstream &archive) : archive_(archive) {
    }

    Letter Decoder::CodeToLetter(Code code) {
        Letter letter;
        for (size_t i = 0; i < LETTER_SIZE; ++i) {
            letter[i] = code[i];
        }
        return letter;
    }

    std::unordered_map<Code, Letter> Decoder::GetCanonicalCodes(const std::vector<Letter> &letters,
                                                                std::vector<size_t> &code_length) {
        size_t code = 0;
        std::bitset<MAX_CODE_LENGTH> binary_code;
        std::unordered_map<Code, Letter> letters_codes;
        size_t current_length = 1;
        for (auto letter: letters) {
            Code current_code;
            while (code_length[current_length - 1] == 0) {
                ++current_length;
                code = code << 1;
            }
            binary_code = code;
            for (size_t j = 0; j < current_length; ++j) {
                current_code.push_back(binary_code[current_length - j - 1]);
            }
            letters_codes[current_code] = letter;
            --code_length[current_length - 1];
            ++code;
        }
        return letters_codes;
    }

    Letter Decoder::GetLetter() {
        Code current_code;
        while (letters_codes_.find(current_code) == letters_codes_.end()) {
            current_code.push_back(archive_.GetBits(ONE_BIT).front());
        }
        if(current_code.size() > MAX_CODE_LENGTH){
            throw std::exception();
        }
        return letters_codes_[current_code];
    }

    void Decoder::Decode() {
        bool terminator_sign = false;
        while (!terminator_sign) {
            letters_codes_.clear();
            size_t symbols_count = CodeToLetter(archive_.GetBits(LETTER_SIZE)).to_ulong();
            std::vector<Letter> letters;
            for (size_t i = 0; i < symbols_count; ++i) {
                letters.push_back(CodeToLetter(archive_.GetBits(LETTER_SIZE)));
            }
            std::vector<size_t> code_length;
            while (symbols_count > 0) {
                code_length.push_back(CodeToLetter(archive_.GetBits(LETTER_SIZE)).to_ulong());
                symbols_count -= code_length.back();
            }

            letters_codes_ = GetCanonicalCodes(letters, code_length);
            std::string file_name;

            Letter current_letter = GetLetter();
            while (current_letter != FILENAME_END) {
                file_name.push_back(static_cast<char>(current_letter.to_ullong()));
                current_letter = GetLetter();
            }

            std::ofstream file(file_name, std::iostream::binary);
            current_letter = GetLetter();
            while (current_letter != ONE_MORE_FILE && current_letter != ARCHIVE_END) {
                file.put(static_cast<char>(current_letter.to_ulong()));
                current_letter = GetLetter();
            }
            file.close();

            std::cout << "File " << file_name << " decoded successfully\n";
            terminator_sign = current_letter == ARCHIVE_END;
        }
        std::cout << "Decoding finished\n";
    }

}  // namespace huffman
