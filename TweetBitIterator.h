#pragma once

#include <bitset>
#include <string>
#include <stdexcept>
#include "BitIterator.h"
#include "Tweet.h"

template <size_t block_size>
class TweetBitIterator : public BitIterator {
public:
    TweetBitIterator(const std::vector<Tweet> &tweets) :
        tweets(tweets), bits(8*tweets.size()),
        tweet_index(0), current_tweet("", std::bitset<160>()) {
        configure_bitset();
    };

    bool next() {
        if(bit_index == block_size) {
            increment_tweet();
        }
        return tweet_as_bitset[bit_index++];
    }

    bool has_next() const {
        if(tweets.size() == 0) {
            return false;
        }
        auto tweets_left_to_read = (tweet_index < tweets.size() - 1);
        auto bits_left_to_read_on_last = (tweet_index == tweets.size() - 1) && (bit_index < block_size);
        return tweets_left_to_read || bits_left_to_read_on_last;
    }

private:
    void increment_tweet() {
        if(tweet_index + 1 >= tweets.size()) {
            throw std::runtime_error("No more tweets.");
        }
        ++tweet_index;
        configure_bitset();
    }

    void configure_bitset() {
        bit_index = 0;
        if(tweets.size() > 0) {
            current_tweet = tweets.at(tweet_index);
            tweet_as_bitset = current_tweet.get_sha1();
        }
    }

    std::vector<Tweet> tweets;
    const size_t bits;
    unsigned long bit_index;
    unsigned long tweet_index;
    Tweet current_tweet;
    std::bitset<160> tweet_as_bitset;
};
