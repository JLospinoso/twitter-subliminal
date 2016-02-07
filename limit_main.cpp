#include "TwitterSubliminalApplication.h"

class TwitterSubliminalLimit : public TwitterSubliminalApplication {
public:
    virtual std::string application_description() override {
        return "Determines the rate limited status of a Twitter Application";
    }

    virtual int main(const std::vector<std::string> &args) override {
        if(args.size() > 0) {
            logger().warning("You supplied arguments; program does not take any");
            displayHelp();
            return TwitterSubliminalLimit::EXIT_USAGE;
        }
        if (terminate_early) {
            return TwitterSubliminalLimit::EXIT_OK;
        }
        logger().information("Determining Twitter application limit status.");
        TwitterSubliminalContainer<1> container(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);
        auto limit_retriever = container.make_limit_retriever();
        try {
            limit_retriever->retrieve();
        } catch(std::runtime_error e) {
            logger().error("Error retrieving limits: %s", std::string(e.what()));
            throw e;
        }
        return TwitterSubliminalLimit::EXIT_OK;
    };
};

POCO_APP_MAIN(TwitterSubliminalLimit)
