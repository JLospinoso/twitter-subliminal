#include <gtest/gtest.h>
#include "../StringBitIterator.h"
#include "TestUtils.h"

class StringBitIteratorTest : public ::testing::Test {
};

TEST_F(StringBitIteratorTest, translates_single_character) {
    StringBitIterator bit_iterator("A");

    ASSERT_TRUE(matches<8>("01000001", bit_iterator));
    ASSERT_FALSE(bit_iterator.has_next());
}

TEST_F(StringBitIteratorTest, translates_two_characters) {
    StringBitIterator bit_iterator("AB");

    ASSERT_TRUE(matches<8>("01000001", bit_iterator)); //A
    ASSERT_TRUE(matches<8>("01000010", bit_iterator)); //B
    ASSERT_FALSE(bit_iterator.has_next());
}

TEST_F(StringBitIteratorTest, translates_large_string_of_ones) {
    auto length = static_cast<unsigned int>(0xFFFF);
    StringBitIterator bit_iterator(std::string(length, '\xff'));

    for(unsigned int i=0; i<length*8; i++) {
        ASSERT_TRUE(bit_iterator.has_next());
        ASSERT_TRUE(bit_iterator.next());
    }
    ASSERT_FALSE(bit_iterator.has_next());
}

TEST_F(StringBitIteratorTest, translates_large_string_of_zeros) {
    auto length = static_cast<unsigned int>(0xFFFF);
    StringBitIterator bit_iterator(std::string(length, '\x00'));

    for(unsigned int i=0; i<length*8; i++) {
        ASSERT_TRUE(bit_iterator.has_next());
        ASSERT_FALSE(bit_iterator.next());
    }
    ASSERT_FALSE(bit_iterator.has_next());
}

TEST_F(StringBitIteratorTest, translates_empty_gracefully) {
    StringBitIterator bit_iterator("");

    ASSERT_FALSE(bit_iterator.has_next());
}
