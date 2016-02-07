#include <gtest/gtest.h>
#include "../Tweet.h"

class TweetTest : public ::testing::Test {
protected:
    std::string tweet_id = "12345";
    std::bitset<160> sha1 = 0x123456;
};

TEST_F(TweetTest, stores_tweet_id) {
    Tweet tweet(tweet_id, sha1);

    ASSERT_EQ(tweet_id, tweet.get_tweet_id());
}

TEST_F(TweetTest, stores_sha1) {
    Tweet tweet(tweet_id, sha1);

    ASSERT_EQ(sha1, tweet.get_sha1());
}
