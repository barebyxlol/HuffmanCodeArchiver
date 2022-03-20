#pragma once

#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>

const int CHAR_SIZE = 8;

class BitReader {
public:
    explicit BitReader(std::istream &archive);

    std::vector<bool> GetBits(size_t number_of_bits);

private:
    bool Get1Bit();
    std::istream &input_;
    std::vector<bool> buffer_;
};

class BitWriter {
public:
    explicit BitWriter(std::ostream &archive);

    void Put(const std::vector<bool> &letter);

    void LastPut(const std::vector<bool> &letter);

private:
    std::ostream &output_;
    std::vector<bool> buffer_;

private:
    void PutBuffer();
};
