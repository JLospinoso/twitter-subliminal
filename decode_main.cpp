#include <fstream>
#include "TwitterSubliminalApplication.h"

#define CASE(N)case N:\
    result = CONTAINER(N).make_decoder()->receive();\
    break;

class TwitterSubliminalDecode : public TwitterSubliminalApplication {
public:
    virtual void defineOptions(Poco::Util::OptionSet &options) override {
        TwitterSubliminalApplication::defineOptions(options);
        options.addOption(Poco::Util::Option("blocksize",
                                             "o",
                                             "specify the size of a block [1,20]")
                                  .required(false)
                                  .argument("size")
                                  .binding("blocksize"));
        options.addOption(Poco::Util::Option("output",
                                             "p",
                                             "specify file PATH for output; if omitted, output to stdout")
                                  .required(false)
                                  .argument("PATH")
                                  .binding("output_path"));
    };

    virtual std::string application_description() override {
        return "Retrieves subliminal messages from Twitter.";
    }

    virtual int main(const std::vector<std::string> &args) override {
        if(args.size() > 0) {
            logger().warning("You supplied arguments; program does not take any");
            displayHelp();
            return TwitterSubliminalDecode::EXIT_USAGE;
        }
        if (terminate_early) {
            return TwitterSubliminalDecode::EXIT_OK;
        }
        block_size = config().getUInt("blocksize");
        logger().information("Decoding subliminal message with block size %?u from Twitter.", block_size);
        std::string result;
        DO_CASE(block_size);
        logger().information("Successfully decoded message of size %?u bytes.", result.size());
        if(!config().has("output_path")) {
            logger().information(result);
        } else {
            auto output_path = config().getString("output_path");
            std::ofstream file_out(output_path);
            if(file_out.fail()) {
                logger().critical("Unable to open %s for writing.", output_path);
                return TwitterSubliminalDecode::EXIT_IOERR;
            }
            file_out << result;
            logger().information("Wrote output to %s", output_path);
        }
        return TwitterSubliminalDecode::EXIT_OK;
    };
private:
    size_t block_size = 0;
};

POCO_APP_MAIN(TwitterSubliminalDecode)
