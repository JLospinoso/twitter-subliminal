#pragma once

#include "Twitter.h"
#include <memory>
#include "Sha1Calculator.h"
#include "Tweet.h"
#include <vector>
#include "Poco/Logger.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"

typedef std::function<bool(std::vector<Tweet> &tweets, Poco::JSON::Object::Ptr element)> emplace_fn;

class TweetRetriever {
public:
    TweetRetriever(std::unique_ptr<Twitter> twitter) :
            twitter(std::move(twitter)),
            logger(Poco::Logger::get("TweetRetriever")){

    }

    std::vector<Tweet> get_all_retweets() {
        logger.debug("Retrieving all retweets");
        return get_all(emplace_tweet);
    }

    std::vector<Tweet> get_all_statuses() {
        logger.debug("Retrieving all statuses");
        return get_all(emplace_status);
    }

private:
    std::vector<Tweet> get_all(const emplace_fn &do_emplace) {
        logger.debug("Querying Twitter for a list of tweets.");
        auto root = query_twitter().extract<Poco::JSON::Array::Ptr>();
        std::vector<Tweet> result;
        if(root->size() == 0) {
            logger.warning("Twitter returned no tweets.");
            return result;
        }
        do_emplace(result, root->getObject(0));
        while(root->size() > 1) {
            for (int i = 1; i < root->size(); i++) {
                do_emplace(result, root->getObject(i));
            }
            auto last_id = result.back().get_tweet_id();
            logger.debug("Querying Twitter for tweets after %s", last_id);
            root = query_twitter(last_id).extract<Poco::JSON::Array::Ptr>();
        }
        logger.debug("Retrieved %?u tweets", result.size());
        return result;
    }

    Poco::Dynamic::Var query_twitter(const std::string &max_id = "") {
        std::string response = twitter->timelineUserGet(true, max_id);
        parser.reset();
        return parser.parse(response);
    }

    emplace_fn emplace_status = [&](std::vector<Tweet> &tweets, Poco::JSON::Object::Ptr tweet) {
        auto tweet_id = tweet->get("id_str").toString();
        tweets.emplace_back(tweet_id, sha1Calculator.bitset_hash(tweet_id));
        return true;
    };

    emplace_fn emplace_tweet = [&](std::vector<Tweet> &tweets, Poco::JSON::Object::Ptr tweet) {
        if(!tweet->has("retweeted_status")) {
            return false;
        }
        auto retweeted_status = tweet->getObject("retweeted_status");
        auto tweet_id = retweeted_status->get("id_str").toString();
        tweets.emplace_back(tweet_id, sha1Calculator.bitset_hash(tweet_id));
        return true;
    };

    std::unique_ptr<Twitter> twitter;
    Sha1Calculator sha1Calculator;
    Poco::Logger &logger;
    Poco::JSON::Parser parser;
};
