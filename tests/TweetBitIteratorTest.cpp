#include <gtest/gtest.h>
#include "../TweetBitIterator.h"
#include "TestUtils.h"

class TweetBitIteratorTest : public ::testing::Test {
    void SetUp() {
        hash_1[0] = false;
        hash_1[1] = true;
        hash_1[2] = false;
        hash_2[0] = true;
        hash_2[1] = false;
        hash_2[2] = true;
        tweets = { Tweet("t1", hash_1), Tweet("t2", hash_2) };
    }
protected:
    std::vector<Tweet> tweets;
    std::bitset<160> hash_1, hash_2;
};

TEST_F(TweetBitIteratorTest, stores_author_id) {
    TweetBitIterator<3> iterator(tweets);
    ASSERT_TRUE(matches<6>("101010", iterator)); // Reverse bit order
}
