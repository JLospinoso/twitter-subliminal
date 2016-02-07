#include <gtest/gtest.h>
#include "../TweetBlockCalculator.h"

class TweetBlockCalculatorTest : public ::testing::Test {
};

TEST_F(TweetBlockCalculatorTest, parses_from_sha1) {
    auto expected = std::bitset<10>("1010101010");
    auto sha1 = std::bitset<160>("1010101010");
    Tweet tweet("tweet", sha1);
    TweetBlockCalculator<10> tweetBlockCalculator;

    auto result = tweetBlockCalculator.calculate(tweet);

    ASSERT_EQ(expected, result);
}
