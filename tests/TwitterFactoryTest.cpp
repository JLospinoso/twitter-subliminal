#include <gtest/gtest.h>
#include "../ShortBitIterator.h"
#include "../TwitterFactory.h"

class TwitterFactoryTest : public ::testing::Test {
protected:
    std::string consumer_key = "my_consumer_key";
    std::string secret_key = "my_secret_key";
    std::string access_token = "my_access_token";
    std::string access_token_secret = "my_token_secret";
    std::string ca_path = "my_ca_path";
    std::string lang = "my_lang";
};

TEST_F(TwitterFactoryTest, gets_consumer_key) {
    TwitterFactory twitter_factory(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);

    ASSERT_EQ("my_consumer_key", twitter_factory.get_consumer_key());
}

TEST_F(TwitterFactoryTest, gets_secret_key) {
    TwitterFactory twitter_factory(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);

    ASSERT_EQ("my_secret_key", twitter_factory.get_secret_key());
}

TEST_F(TwitterFactoryTest, gets_access_token) {
    TwitterFactory twitter_factory(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);

    ASSERT_EQ("my_access_token", twitter_factory.get_access_token());
}

TEST_F(TwitterFactoryTest, get_access_token_secret) {
    TwitterFactory twitter_factory(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);

    ASSERT_EQ("my_token_secret", twitter_factory.get_access_token_secret());
}

TEST_F(TwitterFactoryTest, get_ca_path) {
    TwitterFactory twitter_factory(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);

    ASSERT_EQ("my_ca_path", twitter_factory.get_ca_path());
}

TEST_F(TwitterFactoryTest, get_lang) {
    TwitterFactory twitter_factory(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);

    ASSERT_EQ("my_lang", twitter_factory.get_lang());
}
