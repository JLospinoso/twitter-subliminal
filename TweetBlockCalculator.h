#pragma once
#include <bitset>
#include "Tweet.h"

template <size_t block_size>
class TweetBlockCalculator {
public:
    std::bitset<block_size> calculate(const Tweet &tweet) {
        static_assert(block_size <= 160, "Block size can't be bigger than SHA1 size (160 bits)");

        std::bitset<block_size> result;
        auto sha1 = tweet.get_sha1();
        for(int i=0; i<block_size; i++) {
            result[i] = sha1[i];
        }
        return result;
    }
};
