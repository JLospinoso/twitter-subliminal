#include "TwitterSubliminalApplication.h"

class TwitterSubliminalResetter : public TwitterSubliminalApplication {
public:
    virtual std::string application_description() override {
        return "Deletes all retweets in subliminal Twitter account.";
    }

    virtual int main(const std::vector<std::string> &args) override {
        if(args.size() > 0) {
            logger().warning("You supplied arguments; program does not take any");
            displayHelp();
            return TwitterSubliminalResetter::EXIT_USAGE;
        }
        if (terminate_early) {
            return TwitterSubliminalResetter::EXIT_OK;
        }
        logger().information("Interrogating Twitter for retweets.");
        TwitterSubliminalContainer<1> container(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);
        auto resetter = container.make_resetter();
        try {
            resetter->reset();
        } catch(std::runtime_error e) {
            logger().error("Error retrieving limits: %s", std::string(e.what()));
            throw e;
        }
        return TwitterSubliminalResetter::EXIT_OK;
    };
};

POCO_APP_MAIN(TwitterSubliminalResetter)
