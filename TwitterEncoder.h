#pragma once
#include "TwitterBlockEncoder.h"
#include <memory>
#include "StringBitIterator.h"
#include "ShortBitIterator.h"
#include "BitIterator.h"
#include "Poco/Logger.h"

template <size_t block_size>
class TwitterEncoder {
public:
    TwitterEncoder(std::unique_ptr<TwitterBlockEncoder<block_size>> twitter_encoder, std::unique_ptr<Twitter> twitter)
            : twitter_encoder(std::move(twitter_encoder)), twitter(std::move(twitter)),
              logger(Poco::Logger::get("TwitterBlockEncoder")) {
    }

    void send(std::string message) {
        StringBitIterator stringBitIterator(message);
        send_iterator(&stringBitIterator);
    }

private:
    void send_iterator(BitIterator *iterator) {
        block.reset();
        unsigned long bit_index = 0, bits_sent = 0;
        while(iterator->has_next()) {
            bits_sent++;
            block[bit_index++] = iterator->next();
            if(bit_index == block_size) {
                send_block();
                block.reset();
                bit_index=0;
            }
        }
        if(bit_index != 0) {
            send_block();
            block.reset();
        }
        logger.information("Sent %?u bits.", bits_sent);
    }

    void send_block() {
        auto tweet = twitter_encoder->encode(block);
        TweetBlockCalculator<block_size> c;
        auto encoding = c.calculate(tweet);
        auto result = twitter->retweetById(tweet.get_tweet_id());
        logger.information("Encoded %s via retweet of tweet with id = %s.", encoding.to_string(), tweet.to_string());
    }
    std::unique_ptr<TwitterBlockEncoder<block_size>> twitter_encoder;
    std::unique_ptr<Twitter> twitter;
    std::bitset<block_size> block;
    Poco::Logger &logger;
};
