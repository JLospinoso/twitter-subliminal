#pragma once
#include <bitset>
#include "../BitIterator.h"

template <size_t size>
bool matches(std::string bits, BitIterator &bit_iterator) {
    std::bitset<size> bits_as_bitset(bits);
    if(!bit_iterator.has_next()) {
        return false;
    }
    for(unsigned int i=0; i<size; i++) {
        if (!bit_iterator.has_next() || bits_as_bitset[i] != bit_iterator.next()) {
            return false;
        }
    }
    return true;
}