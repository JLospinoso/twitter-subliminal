#include "TwitterSubliminalApplication.h"
#include <cstdlib> // http://stackoverflow.com/questions/20145488/cygwin-g-stdstoi-error-stoi-is-not-a-member-of-std
#define CASE(N) case N:\
    encoding_test<N>();\
    break;

class TwitterSubliminalPerformance : public TwitterSubliminalApplication {
public:
    virtual void defineOptions(Poco::Util::OptionSet &options) override {
        TwitterSubliminalApplication::defineOptions(options);
        options.addOption(Poco::Util::Option("sample-time",
                                             "s",
                                             "specify how many SECONDS to sample from Twitter Streaming API; specify 0 to skip Streaming test")
                                  .required(false)
                                  .argument("SECONDS")
                                  .binding("sample.time"));
        options.addOption(Poco::Util::Option("update.interval",
                                             "u",
                                             "during stream test, specify how many TWEETS to elapse before giving an update")
                                  .required(false)
                                  .argument("TWEETS")
                                  .binding("update.interval"));
        options.addOption(Poco::Util::Option("blocks.trial",
                                             "o",
                                             "during encoding test, specify how many BLOCKS to encode per blocksize")
                                  .required(false)
                                  .argument("BLOCKS")
                                  .binding("blocks.trial"));
        options.addOption(Poco::Util::Option("encoding.test",
                                             "t",
                                             "add encoding test for BLOCKSIZE; multiples permitted; omit to skip encoding test; valid values [1,20]")
                                  .required(false)
                                  .argument("BLOCKSIZE")
                                  .repeatable(true)
                                  .callback(Poco::Util::OptionCallback<TwitterSubliminalPerformance>(this, &TwitterSubliminalPerformance::registerEncodingTest)));

        Application::defineOptions(options);
    };

    void registerEncodingTest(const std::string& name, const std::string& value) {
        // Bug in MinGW/Cygwin GCC. std::stoul not recognized.
        // block_sizes.emplace(std::stoul(value));
        block_sizes.emplace(strtol(value.c_str(), 0, 10));
    }

    virtual std::string application_description() override {
        return "Samples Twitter Streaming API and estimates encoding times.";
    }

    virtual int main(const std::vector<std::string> &args) override {
        if(args.size() > 0) {
            logger().warning("You supplied arguments; program does not take any");
            displayHelp();
            return TwitterSubliminalPerformance::EXIT_USAGE;
        }
        if (terminate_early) {
            return TwitterSubliminalPerformance::EXIT_OK;
        }
        stream_sampling_time_ms = config().getUInt("sample.time") * 1000;
        stream_sampling_update_interval = config().getUInt("update.interval");
        blocks_per_trial = config().getUInt("blocks.trial");
        if(stream_sampling_time_ms > 0) {
            stream_velocity_test();
        }
        std::for_each(block_sizes.begin(), block_sizes.end(), [this](size_t block_size) {
            DO_CASE(N)
        });
        return TwitterSubliminalPerformance::EXIT_OK;
    };

private:
    unsigned long blocks_per_trial=0, stream_sampling_time_ms = 0, stream_sampling_update_interval = 0;
    std::set<size_t> block_sizes;

    template <size_t block_size>
    void encoding_test() {
        auto test_container = container<block_size>(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);
        auto encoder = test_container.get_twitter_block_encoder();
        auto t_start = std::chrono::system_clock::now();
        logger().information("Beginning encoding with block size %?u", block_size);
        for(unsigned long bitset_value = 0; bitset_value < blocks_per_trial; bitset_value++) {
            std::bitset<block_size> bitset_to_encode = bitset_value;
            encoder->encode(bitset_to_encode);
            logger().information("   Encoded %s (%3?u of %3?u for size %?u)", bitset_to_encode.to_string(), bitset_value, blocks_per_trial, block_size);
        }
        auto milliseconds_elapsed = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now() - t_start).count();
        auto baud = block_size * (double)blocks_per_trial / milliseconds_elapsed * (double)1000;
        logger().information("Blocks of %?u bits encoded at %5.2f baud.", block_size, baud);
    }

    void stream_velocity_test() {
        auto test_container = container<1>(consumer_key, secret_key, access_token, access_token_secret, ca_path, lang);
        auto stream = test_container.get_twitter_stream();
        unsigned long n_tweets = 0;
        logger().information("Sampling Twitter Stream for %?u seconds to estimate velocity.", stream_sampling_time_ms / 1000);
        auto t_start = std::chrono::system_clock::now();
        double milliseconds_elapsed = 0;
        while(milliseconds_elapsed < stream_sampling_time_ms) {
            stream->next();
            n_tweets++;
            milliseconds_elapsed = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now() - t_start).count();
            if(n_tweets % stream_sampling_update_interval == 0) {
                logger().information("    Sampled %5?u tweets in %6.1f seconds.", n_tweets, milliseconds_elapsed / (double)1000);
            }
        }
        auto seconds_elapsed = milliseconds_elapsed / (double)1000;
        auto tweets_per_second = (double)n_tweets / seconds_elapsed;
        logger().information("Received %?u tweets in %5.2f seconds (%5.2f tweets per second).", n_tweets, seconds_elapsed, tweets_per_second);
        logger().information("Estimated encoding times (no caching, i.e. how long to expect for first block):");
        for(unsigned int block_size = 1; block_size <= 20; block_size++) {
            double domain = pow(2, block_size);
            double p_block_fail = 1 - 1 / domain;
            double expected_trials = p_block_fail/(1-p_block_fail);
            double blocks_per_hour = tweets_per_second * (double)3600 / expected_trials;
            double baud = blocks_per_hour / (double)3600 * (double)block_size;
            logger().information("%5?u: %10.3f baud (%15.2f %3?u-bit blocks per hour)", block_size, baud, blocks_per_hour, block_size);
        }
    }
};

POCO_APP_MAIN(TwitterSubliminalPerformance)
