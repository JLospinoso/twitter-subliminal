#include "TwitterSubliminalApplication.h"

#define CASE(N)case N:\
    CONTAINER(N).make_encoder()->send(message);\
    break;

class TwitterSubliminalEncode : public TwitterSubliminalApplication {
public:
    virtual void defineOptions(Poco::Util::OptionSet &options) override {
        TwitterSubliminalApplication::defineOptions(options);
        options.addOption(Poco::Util::Option("blocksize",
                                             "o",
                                             "specify the size of a block [1,20]")
                                  .required(false)
                                  .argument("size")
                                  .binding("blocksize"));
    };

    virtual std::string application_description() override {
        return "Sends subliminal messages to Twitter.";
    }

    virtual int main(const std::vector<std::string> &args) override {
        if (terminate_early) {
            return TwitterSubliminalEncode::EXIT_OK;
        }
        block_size = config().getUInt("blocksize");

        std::string message;
        if(args.size() == 0) {
            std::cin >> std::noskipws;
            std::istream_iterator<char> it(std::cin), end;
            message = std::string(it, end);
        } else if(args.size() == 1) {
            message = args[0];
        } else {
            logger().warning("You must supply one argument (text to encode).");
            displayHelp();
            return TwitterSubliminalEncode::EXIT_USAGE;
        }
        logger().information("Encoding message of size %?u bytes in %?u bit blocks.", message.size(), block_size);


        logger().information("Encoding message: %s", message);
        auto t_start = std::chrono::system_clock::now();
        DO_CASE(block_size);
        auto milliseconds_elapsed = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now() - t_start).count();
        auto seconds_elapsed = milliseconds_elapsed/1000;
        auto baud = block_size * message.size() / seconds_elapsed;
        logger().information("Completed encoding in %6.2f seconds (%6.2f baud).", seconds_elapsed, baud);
        return Application::main(args);
    };

    virtual std::string usage_line() override {
        return "[message] OPTIONS; If message not present, it is taken from stdin";
    }

private:
    size_t block_size = 0;
};

POCO_APP_MAIN(TwitterSubliminalEncode)
