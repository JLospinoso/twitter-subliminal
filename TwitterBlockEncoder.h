#pragma once

#include <bitset>
#include "Tweet.h"
#include <unordered_map>
#include <mutex>
#include "TwitterStream.h"
#include "TweetBlockCalculator.h"
#include <memory>
#include <future>
#include <cmath>
#include "Twitter.h"
#include <algorithm>
#include <iomanip>
#include "Poco/Logger.h"


constexpr unsigned long long pow_2(unsigned long long n)
{
    return (unsigned long long) 2 << (n-1);
}

template <size_t block_size>
class TwitterBlockEncoder {
public:
    TwitterBlockEncoder(std::shared_ptr<TwitterStream> stream,
                        std::shared_ptr<TweetBlockCalculator<block_size>> blockCalculator,
                        size_t max_tweets_per_block = 10)
            : lookup(pow_2(block_size)), max_tweets_per_block{max_tweets_per_block},
              stream(stream), blockCalculator(blockCalculator),
              logger(Poco::Logger::get("TwitterBlockEncoder")){
        generate_future();
    }

    ~TwitterBlockEncoder() {
        killed = true;
        digestion_future.wait();
        poco_trace(logger, "Destructed TwitterBlockEncoder");
    }

    Tweet encode(std::bitset<block_size> block) {

        auto digestion_status = digestion_future.wait_for(std::chrono::seconds(0));
        if(digestion_status == std::future_status::ready) {
            logger.warning("Digestion processing future completed; launching a new one.");
            generate_future();
        }
        typename std::unordered_multimap<std::bitset<block_size>, Tweet>::iterator entry;
        bool found_match;
        auto t_start = std::chrono::system_clock::now();
        auto t_last = t_start;
        double last_coverage_ratio = 0;
        poco_trace_f1(logger, "Encoding %s", block.to_string());
        do {
            {
                poco_trace(logger, "Attempting to acquire encoding lock");
                std::lock_guard<std::mutex> lock_acquired(lookup_lock);
                poco_trace(logger, "Encoding lock acquired");
                entry = lookup.find(block);
                found_match = entry != lookup.end();
                poco_trace_f1(logger, found_match ? "Found encoding for %s." : "No encoding for %s in store.", block.to_string());
                poco_trace(logger, "Releasing encoding lock");
            }
            if(!found_match) {
                auto milliseconds_since_last_update = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now() - t_last).count();
                auto milliseconds_since_start = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now() - t_start).count();
                if(milliseconds_since_last_update > update_interval_ms) {
                    t_last = std::chrono::system_clock::now();
                    unsigned long long populated_buckets = 0;
                    {
                        poco_trace(logger, "Attempting to acquire encoding lock");
                        std::lock_guard<std::mutex> lock_acquired(lookup_lock);
                        poco_trace(logger, "Encoding lock acquired");
                        for (size_t i = 0; i < lookup.bucket_count(); i++) {
                            if (lookup.begin(i) != lookup.end(i)) {
                                populated_buckets++;
                            }
                        }
                        poco_trace(logger, "Releasing encoding lock");
                    }
                    double coverage_ratio = (double)populated_buckets / (double)block_space;
                    auto coverage_velocity_s = 1000 * (coverage_ratio - last_coverage_ratio) / milliseconds_since_last_update;
                    double time_est = (1-coverage_ratio) / coverage_velocity_s;
                    last_coverage_ratio = coverage_ratio;
                    logger.debug("Block encoder coverage: %5.4f%; ~%5.4f m. to saturation", 100 * coverage_ratio, time_est / 60);
                }
                poco_trace_f2(logger, "Sleeping %?u before polling for %s in store", poll_interval_ms, block.to_string());
                std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval_ms));
            }
        } while(!found_match);
        {
            poco_trace(logger, "Attempting to acquire encoding lock");
            std::lock_guard<std::mutex> lock_acquired(lookup_lock);
            poco_trace(logger, "Encoding lock acquired");
            Tweet result(entry->second);
            lookup.erase(entry);
            poco_trace(logger, "Releasing encoding lock");
            return result;
        }
    }

    void digest() {
        while(!killed) {
            try{
                poco_trace(logger, "Waiting for next tweet from stream.");
                auto next_tweet = stream->next();
                auto block = blockCalculator->calculate(next_tweet);
                poco_trace(logger, "Attempting to acquire encoding lock");
                std::lock_guard<std::mutex>acquired_lock(lookup_lock);
                poco_trace(logger, "Encoding lock acquired");
                if(lookup.count(block) < max_tweets_per_block) {
                    lookup.emplace(std::bitset<block_size>(block), Tweet(next_tweet));
                    logger.debug("Emplaced %s into store (contains %?u elements).", block.to_string(), lookup.count(block));
                } else {
                    logger.debug("Rejected %s into store (contains %?u elements).", block.to_string(), lookup.count(block));
                }
                poco_trace(logger, "Releasing encoding lock");
            } catch (std::exception e) {
                logger.error("Error during tweet digestion: %s", e.what());
            }
        }
    }

private:
    void generate_future() {
        poco_debug(logger, "Initializing digestion future.");
        digestion_future = std::async(std::launch::async, &TwitterBlockEncoder::digest, this);
    }

    TwitterBlockEncoder(const TwitterBlockEncoder &) = delete;
    TwitterBlockEncoder(TwitterBlockEncoder &&) = delete;
    TwitterBlockEncoder & operator=(TwitterBlockEncoder) = delete;
    TwitterBlockEncoder & operator=(TwitterBlockEncoder &&) = delete;

    const unsigned long long block_space = pow_2(block_size);
    const unsigned long poll_interval_ms = 20, update_interval_ms = 10000;
    bool killed = false;
    const size_t max_tweets_per_block;
    std::unordered_multimap<std::bitset<block_size>, Tweet> lookup;
    std::mutex lookup_lock;
    std::shared_ptr<TwitterStream> stream;
    std::shared_ptr<TweetBlockCalculator<block_size>> blockCalculator;
    std::future<void> digestion_future;
    Poco::Logger &logger;
};
