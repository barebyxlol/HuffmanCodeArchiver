#include "BitOperations.h"

BitWriter::BitWriter(std::ostream &archive) : output_(archive) {
}

void BitWriter::PutBuffer() {
    std::bitset<CHAR_SIZE> b;
    for (size_t i = 0; i < CHAR_SIZE; ++i) {
        b[CHAR_SIZE - i - 1] = buffer_[i];
    }
    output_.put(static_cast<char>(b.to_ulong()));
    buffer_.clear();
}

void BitWriter::Put(const std::vector<bool> &letter) {
    for (const auto &bit: letter) {
        buffer_.push_back(bit);
        if (buffer_.size() == CHAR_SIZE) {
            PutBuffer();
        }
    }
}

void BitWriter::LastPut(const std::vector<bool> &letter) {
    this->Put(letter);
    if (!buffer_.empty()) {
        while (buffer_.size() != CHAR_SIZE) {
            buffer_.push_back(false);
        }
        PutBuffer();
    }
}

BitReader::BitReader(std::istream &archive) : input_(archive){
}

bool BitReader::Get1Bit() {
    bool returned_value;
    if (buffer_.empty()) {
        if(input_.peek() == EOF){
            throw std::invalid_argument("No more bits to read");
        }
        std::bitset<CHAR_SIZE> char_bits = input_.get();
        for (size_t i = 0; i < CHAR_SIZE; ++i) {
            buffer_.push_back(char_bits[i]);
        }
    }
    returned_value = buffer_.back();
    buffer_.pop_back();
    return returned_value;
}

std::vector<bool> BitReader::GetBits(size_t number_of_bits) {
    std::vector<bool> returned_value;

    for (size_t i = 0; i < number_of_bits; ++i) {
        returned_value.push_back(Get1Bit());
    }

    return returned_value;
}

