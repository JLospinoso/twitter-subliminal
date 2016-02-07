#include <gtest/gtest.h>
#include "../TwitterSubliminalContainer.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "../TwitterSubliminalApplication.h"

class TestApp : public TwitterSubliminalApplication {
public:
    TestApp() {
        initialize(*this);
    }

    virtual int main(const std::vector<std::string> &args) override {
        return 0;
    }

    virtual std::string application_description() override {
        return "Test App";
    }
};

class IntegrationTest : public ::testing::Test {
protected:

    virtual void SetUp() override {
        ::testing::Test::SetUp();
        // Configure logging
        std::string log_file("IntegrationTest.log");
        Poco::AutoPtr<Poco::SimpleFileChannel> channel(new Poco::SimpleFileChannel(log_file));
        Poco::AutoPtr<Poco::PatternFormatter> pattern_formatter(new Poco::PatternFormatter);
        pattern_formatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s %p [%P %I] %t");
        Poco::AutoPtr<Poco::FormattingChannel> formatted_channel(new Poco::FormattingChannel(pattern_formatter, channel));

        Poco::Logger::root().setChannel(formatted_channel);
        Poco::Logger::root().setLevel("information");
        ASSERT_NE("", app.consumer_key);
        ASSERT_NE("", app.secret_key);
        ASSERT_NE("", app.access_token);
        ASSERT_NE("", app.access_token_secret);
    }

    TestApp app;
	std::string message = "ABCDE";
};

TEST_F(IntegrationTest, CanReadWriteMessage4) {
    auto test_container = container<4>(app.consumer_key, app.secret_key, app.access_token, app.access_token_secret, app.ca_path, app.lang);
    auto encoder = test_container.make_encoder();
    auto decoder = test_container.make_decoder();
    auto resetter = test_container.make_resetter();

    resetter->reset();
    std::cout << "[          ] Reset tweets." << std::endl;
    encoder->send(message);
    std::cout << "[          ] Sent message " << message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto result = decoder->receive();
    std::cout << "[          ] Decoded message: " << result << std::endl;
    ASSERT_EQ(message, result);
}

TEST_F(IntegrationTest, CanReadWriteMessage8) {
    auto test_container = container<8>(app.consumer_key, app.secret_key, app.access_token, app.access_token_secret, app.ca_path, app.lang);
    auto encoder = test_container.make_encoder();
    auto decoder = test_container.make_decoder();
    auto resetter = test_container.make_resetter();

    resetter->reset();
    std::cout << "[          ] Reset tweets." << std::endl;
    encoder->send(message);
    std::cout << "[          ] Sent message " << message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto result = decoder->receive();
    std::cout << "[          ] Decoded message: " << result << std::endl;
    ASSERT_EQ(message, result);
}

TEST_F(IntegrationTest, CanReadWriteMessage10) {
    auto test_container = container<10>(app.consumer_key, app.secret_key, app.access_token, app.access_token_secret, app.ca_path, app.lang);
    auto encoder = test_container.make_encoder();
    auto decoder = test_container.make_decoder();
    auto resetter = test_container.make_resetter();

    resetter->reset();
    std::cout << "[          ] Reset tweets." << std::endl;
    encoder->send(message);
    std::cout << "[          ] Sent message " << message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto result = decoder->receive();
    std::cout << "[          ] Decoded message: " << result << std::endl;
    ASSERT_EQ(message, result);
}