#include <gtest/gtest.h>
#include "../ShortBitIterator.h"
#include "TestUtils.h"

class ShortBitIteratorTest : public ::testing::Test {
};

TEST_F(ShortBitIteratorTest, translates_0) {
    ShortBitIterator bit_iterator(0x0000);

    ASSERT_TRUE(matches<16>("0000000000000000", bit_iterator));
    ASSERT_FALSE(bit_iterator.has_next());
}

TEST_F(ShortBitIteratorTest, translates_FFFF) {
    ShortBitIterator bit_iterator(0xFFFF);

    ASSERT_TRUE(matches<16>("1111111111111111", bit_iterator));
    ASSERT_FALSE(bit_iterator.has_next());
}

TEST_F(ShortBitIteratorTest, translates_9999) {
    ShortBitIterator bit_iterator(0x9999);

    ASSERT_TRUE(matches<16>("1001100110011001", bit_iterator));
    ASSERT_FALSE(bit_iterator.has_next());
}

TEST_F(ShortBitIteratorTest, translates_1234) {
    ShortBitIterator bit_iterator(0x1234);

    ASSERT_TRUE(matches<16>("0001001000110100", bit_iterator));
    ASSERT_FALSE(bit_iterator.has_next());
}
