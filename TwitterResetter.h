#pragma once

#include "TweetRetriever.h"
#include <algorithm>

class TwitterResetter {
public:
    TwitterResetter(std::unique_ptr<TweetRetriever> tr, std::unique_ptr<Twitter> tc) :
        tr(std::move(tr)), tc(std::move(tc)),
        logger(Poco::Logger::get("TwitterResetter")) {
    }

    bool reset() {
        bool success = true;
        auto tweets = tr->get_all_statuses();
        logger.information("Retrieved %?u statuses for deletion.", tweets.size());
        std::for_each(tweets.begin(), tweets.end(), [&](Tweet t){
            auto id = t.get_tweet_id();
            try {
                tc->statusDestroyById(id);
                logger.information("Deleted tweet with id %s.", id);
            } catch (std::runtime_error e) {
                logger.error("Failed to delete tweet with id %s: %s", id, e.what());
                success = false;
            }
        });
        return success;
    }
private:
    std::unique_ptr<TweetRetriever> tr;
    std::unique_ptr<Twitter> tc;
    Poco::Logger &logger;
};
