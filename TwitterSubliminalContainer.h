#pragma once
#include <string>
#include "TwitterDecoder.h"
#include "TwitterEncoder.h"
#include "TwitterResetter.h"
#include "TwitterFactory.h"
#include "TwitterLimitRetriever.h"

#define CONTAINER(N) container<N>(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang)

template <size_t block_size>
class TwitterSubliminalContainer {
public:
    TwitterSubliminalContainer(const std::string &consumer_key, const std::string &secret_key,
                               const std::string &access_token, const std::string &access_token_secret,
                               const std::string &ca_path, const std::string &lang) {
        tc_factory = std::unique_ptr<TwitterFactory>(new TwitterFactory(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang));
    }

    std::unique_ptr<TwitterDecoder<block_size>> make_decoder() {
        return std::unique_ptr<TwitterDecoder<block_size>>(new TwitterDecoder<block_size>(get_tweet_retriever()));
    };

    std::unique_ptr<TwitterEncoder<block_size>> make_encoder() {
        return std::unique_ptr<TwitterEncoder<block_size>>(new TwitterEncoder<block_size>(get_twitter_block_encoder(), tc_factory->make()));
    };

    std::unique_ptr<TwitterResetter> make_resetter() {
        return std::unique_ptr<TwitterResetter>(new TwitterResetter(get_tweet_retriever(), tc_factory->make()));
    }

    std::unique_ptr<TwitterLimitRetriever> make_limit_retriever() {
        return std::unique_ptr<TwitterLimitRetriever>(new TwitterLimitRetriever(tc_factory->make()));
    }

    std::unique_ptr<TweetRetriever> get_tweet_retriever() {
        return std::unique_ptr<TweetRetriever>(new TweetRetriever(tc_factory->make()));
    };

    std::unique_ptr<TwitterStream> get_twitter_stream() {
        return std::unique_ptr<TwitterStream>(new TwitterStream(tc_factory->make()));
    }

    std::unique_ptr<TwitterBlockEncoder<block_size>> get_twitter_block_encoder() {
        return std::unique_ptr<TwitterBlockEncoder<block_size>>(new TwitterBlockEncoder<block_size>(get_twitter_stream(), get_tweet_block_calculator()));
    }

    std::unique_ptr<TweetBlockCalculator<block_size>> get_tweet_block_calculator() {
        return std::unique_ptr<TweetBlockCalculator<block_size>>(new TweetBlockCalculator<block_size>());
    }

private:
    std::unique_ptr<TwitterFactory> tc_factory;
};

template <size_t block_size>
TwitterSubliminalContainer<block_size> container(const std::string &consumer_key, const std::string &secret_key, const std::string &access_token,
                                                 const std::string &access_token_secret, const std::string &ca_path, const std::string &lang) {
    return TwitterSubliminalContainer<block_size>(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);
}
