#include <gtest/gtest.h>
#include "../Sha1Calculator.h"

class Sha1CalculatorTest : public ::testing::Test {
};

TEST_F(Sha1CalculatorTest, hashesOfEqualDataEqual_bitset) {
    Sha1Calculator calculator1;
    Sha1Calculator calculator2;
    auto hash1 = calculator1.bitset_hash("A");
    auto hash2 = calculator1.bitset_hash("A");
    auto hash3 = calculator2.bitset_hash("A");
    ASSERT_EQ(hash2, hash1);
    ASSERT_EQ(hash3, hash1);
}

TEST_F(Sha1CalculatorTest, hashesOfUnequalDataUnequal_bitset) {
    Sha1Calculator calculator1;
    auto hash1 = calculator1.bitset_hash("A");
    auto hash2 = calculator1.bitset_hash("B");
    ASSERT_NE(hash2, hash1);
}

TEST_F(Sha1CalculatorTest, hashesOfEqualDataEqual_string) {
    Sha1Calculator calculator1;
    Sha1Calculator calculator2;
    auto hash1 = calculator1.hash("A");
    auto hash2 = calculator1.hash("A");
    auto hash3 = calculator2.hash("A");
    ASSERT_EQ(hash2, hash1);
    ASSERT_EQ(hash3, hash1);
}

TEST_F(Sha1CalculatorTest, hashesOfUnequalDataUnequal_string) {
    Sha1Calculator calculator;
    auto hash1 = calculator.hash("A");
    auto hash2 = calculator.hash("B");
    ASSERT_NE(hash2, hash1);
}
