#pragma once

#include "TweetRetriever.h"
#include "Poco/Logger.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"

class TwitterLimitRetriever {
public:
    TwitterLimitRetriever(std::unique_ptr<Twitter> twitter) :
            twitter(std::move(twitter)), logger(Poco::Logger::get("TwitterLimitRetriever")) {
    }

    bool retrieve() {
        std::string response = twitter->getRateLimitStatus();
        auto root = parser.parse(response).extract<Poco::JSON::Object::Ptr>();
        auto resources = root->getObject("resources");
        auto statuses = resources->getObject("statuses");
        auto application = resources->getObject("application");
        auto account = resources->getObject("account");
        auto user_timeline = statuses->getObject("/statuses/user_timeline");
        auto rate_limit_status = application->getObject("/application/rate_limit_status");
        auto verify_credentials = account->getObject("/account/verify_credentials");
        auto retweet = statuses->getObject("/statuses/retweet");
        auto destroy = statuses->getObject("/statuses/destroy");
        print_limit("Encoder (/statuses/retweet)", retweet);
        print_limit("Decoder (/statuses/user_timeline)", destroy);
        print_limit("Deleter (/statuses/destroy)", user_timeline);
        print_limit("Limit Retriever (/application/rate_limit_status)", rate_limit_status);
        print_limit("Verification (/account/verify_credentials)", verify_credentials);
        return true;
    }
private:
    void print_limit(std::string description, Poco::JSON::Object::Ptr element) {
        if(element.isNull() || !element->has("remaining") || !element->has("limit")) {
            logger.information("%s not mentioned in rate limits.", description);
            return;
        }
        auto remaining = element->get("remaining").convert<int>();
        auto limit = element->get("limit").convert<int>();
        auto used = limit - remaining;
        auto limited = (0 == remaining);
        if(limited) {
            logger.warning("%s is limited! Used %?u of %?u calls within 15 minute window.", description, used, limit);
        } else {
            logger.information("%s is not limited. Used %?u of %?u calls within 15 minute window.", description, used, limit);
        }
    }

    std::unique_ptr<Twitter> twitter;
    Poco::Logger &logger;
    Poco::JSON::Parser parser;
};
