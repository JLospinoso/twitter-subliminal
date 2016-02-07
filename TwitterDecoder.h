#pragma once
#include "TwitterBlockEncoder.h"
#include <memory>
#include "StringBitIterator.h"
#include "ShortBitIterator.h"
#include "BitIterator.h"
#include "Tweet.h"
#include <algorithm>
#include "TweetBitIterator.h"
#include "TweetRetriever.h"

template <size_t block_size>
class TwitterDecoder {
public:
    TwitterDecoder(std::unique_ptr<TweetRetriever> tweet_retreiver) :
            tweet_retriever(std::move(tweet_retreiver)) {
    }

    std::string receive() {
        auto tweets = tweet_retriever->get_all_retweets();
        std::reverse(tweets.begin(), tweets.end());
        auto n_bits = tweets.size() * block_size;
        auto n_chars = n_bits / 8;
        TweetBitIterator<block_size> bit_iterator(tweets);
        std::string message(n_chars, '\0');
        std::bitset<8> char_bs;
        unsigned long bits_received = 0;
        for(int i=0; i<n_chars; i++) {
            char_bs.reset();
            for(int j=0; j<8; j++) {
                char_bs[j] = bit_iterator.next();
                bits_received++;
            }
            message[i] = *reinterpret_cast<char *>(&char_bs);
        }
        return message;
    }
private:
    std::unique_ptr<TweetRetriever> tweet_retriever;
};
