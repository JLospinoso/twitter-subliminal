#pragma once
#include "Tweet.h"
#include <array>
#include <queue>
#include <mutex>
#include <future>
#include "Twitter.h"
#include "Sha1Calculator.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Logger.h"

class TwitterStream {
public:
    TwitterStream(std::unique_ptr<Twitter> twitter)
            : twitter(std::move(twitter)), logger(Poco::Logger::get("TwitterStream")){
        killed.store(false);
        new_tweet.store(false);
        poco_trace(logger, "Constructed TwitterStream");
    }

    ~TwitterStream() {
        killed.store(true);
        stream_future.wait();
        poco_trace(logger, "Destructed TwitterStream");
    }

    Tweet next() {
		poco_trace(logger, "Getting next tweet");
        if(!stream_future.valid()) {
            poco_debug(logger, "Initializing stream processing future.");
            generate_future();
        }
        auto streaming_status = stream_future.wait_for(std::chrono::seconds(0));
        if(streaming_status == std::future_status::ready) {
            logger.warning("Stream processing future completed; launching a new one.");
            generate_future();
        }
        do {
            {
                std::lock_guard<std::mutex> acquire_lock(processing_lock);
                poco_trace(logger, "Processing lock acquired");
                if (tweets.size() > 0) {
                    poco_debug_f1(logger, "Found %?z tweets in tweet queue.", tweets.size());
                    auto result = tweets.front();
                    tweets.pop();
                    poco_debug_f1(logger, "Popped Tweet %s from tweet queue.", result.get_tweet_id());
                    return result;
                }
                poco_debug(logger, "No tweets available in stream queue.");
            }
            new_tweet.store(false);
            while(!new_tweet) {
                poco_trace_f1(logger, "Polling for new tweets from stream queue every %?u ms.", poll_interval_ms);
                std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval_ms));
            }
        } while(true);
    }
private:
    void generate_future() {
        poco_trace(logger, "Generating future");
        stream_future = std::async(std::launch::async, &TwitterStream::process_stream, this);
    }

    void process_stream() {
        poco_trace(logger, "Registering process stream callback with twitter.");
        twitter->stream([this](std::string x){ this->process_input(x); });
    }

    void process_input(const std::string &tweet_json) {
        auto input_length = tweet_json.size();
        poco_debug_f1(logger, "Processing input of length %?u", input_length);
        std::lock_guard<std::mutex> acquired_lock(processing_lock);
        if(tweets.size() >= max_queue_size) {
            poco_debug_f1(logger, "Tweet queue overflow; reached maximum capacity of %?u", max_queue_size);
            return;
        }
        if(killed) {
            throw std::runtime_error("Thread killed.");
        }
        Poco::JSON::Object::Ptr tweet;
        try{
            poco_trace(logger, "Parsing tweet as JSON.");
            Poco::JSON::Parser parser;
            parser.reset();
            tweet = parser.parse(std::string(tweet_json)).extract<Poco::JSON::Object::Ptr>();
        } catch (Poco::SyntaxException e) {
            logger.warning("Parse exception from Streaming API: %s", std::string(e.what()));
            return;
        } catch (Poco::JSON::JSONException e) {
            logger.warning("JSON exception from Streaming API: %s", std::string(e.what()));
            return;
        }
        if(tweet->has("retweeted_status")) {
            poco_trace(logger, "Retweet detected; extracting retweet portion.");
            tweet = tweet->getObject("retweeted_status");
        }
        if(!tweet->has("id_str")) {
            poco_trace(logger, "Tweet did not have id_str");
            return;
        }
        auto id = tweet->get("id_str").toString();
        if(!tweet->has("text")) {
            poco_trace_f1(logger, "Tweet %s did not contain a text portion.", id);
            return;
        }
        auto tweet_text = tweet->get("text").toString();
        if(tweet_text.size() > 1 && tweet_text[0] == 'R' && tweet_text[1] == 'T') {
            poco_debug_f1(logger, "Tweet %s from Streaming API still appears to be a retweet (bug in Twitter).", id);
            return; // Bug reported: https://t.co/qF6Bgk5Qkm
        }
        tweets.emplace(std::string(id), sha1Calculator.bitset_hash(id));
        poco_debug_f1(logger, "Tweet %s added to the tweet queue.", id);
        new_tweet.store(true);
    }

    const unsigned long poll_interval_ms = 100;
    size_t max_queue_size = 100;
    std::unique_ptr<Twitter> twitter;
    std::queue<Tweet> tweets;
    std::mutex processing_lock;
    std::future<void> stream_future;
    std::atomic<bool> killed, new_tweet;
    Sha1Calculator sha1Calculator;
    Poco::Logger &logger;
};
