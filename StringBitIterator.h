#pragma once

#include <bitset>
#include <string>
#include <stdexcept>
#include "BitIterator.h"

class StringBitIterator : public BitIterator {
public:
    StringBitIterator(std::string as_str) :
            as_str(as_str), bits(8*as_str.length()),
            char_index(0) {
        configure_bitset();
    };

    bool next() {
        if(bit_index == 8) {
            increment_char();
        }
        return char_as_bitset[bit_index++];
    }

    bool has_next() const {
        if(as_str.length() == 0) {
            return false;
        }
        auto chars_left_to_read = (char_index < as_str.length() - 1);
        auto bits_left_to_read_on_last_char = (char_index == as_str.length() - 1) && (bit_index < 8);
        return chars_left_to_read || bits_left_to_read_on_last_char;
    }

private:
    void increment_char() {
        if(char_index + 1 >= as_str.length()) {
            throw std::runtime_error("No more chars.");
        }
        ++char_index;
        configure_bitset();
    }

    void configure_bitset() {
        bit_index = 0;
        if(as_str.length() > 0) {
            current_char = static_cast<unsigned long>(as_str.at(char_index));
            char_as_bitset = current_char;
        }
    }

    std::string as_str;
    const size_t bits;
    unsigned long bit_index;
    unsigned long char_index;
    unsigned long current_char;
    std::bitset<8> char_as_bitset;
};
