#pragma once

#include <string>
#include <bitset>

class Tweet {
public:
    Tweet(std::string tweet_id, std::bitset<160> sha1) :
            tweet_id(tweet_id), sha1(sha1) { };

    std::string get_tweet_id() const {
        return tweet_id;
    }

    std::bitset<160> get_sha1() const {
        return sha1;
    }

    std::string to_string() const {
        return tweet_id;
    }

private:
    std::string tweet_id;
    std::bitset<160> sha1;
};
