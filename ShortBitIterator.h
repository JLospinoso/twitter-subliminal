#pragma once

#include <bitset>
#include <string>
#include <stdexcept>
#include <array>
#include "BitIterator.h"

class ShortBitIterator : public BitIterator {
public:
    ShortBitIterator(unsigned short as_ushort) :
            bit_index(0), short_as_bitset(as_ushort) {
        static_assert(sizeof(unsigned short) == 2, "Sizeof unsigned short is very strange.");
    };

    bool next() {
        if(bit_index >= 16) {
            throw std::runtime_error("No more bits.");
        }
        return short_as_bitset[bit_index++];
    }

    bool has_next() const {
        return bit_index < 16;
    }

private:
    ShortBitIterator(ShortBitIterator &) = delete;
    ShortBitIterator & operator= (const ShortBitIterator & other) = delete;

    unsigned long bit_index;
    std::bitset<16> short_as_bitset;
};
