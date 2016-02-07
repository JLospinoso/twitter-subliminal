#pragma once

#include "TwitterSubliminalContainer.h"
#include "Poco/Util/Application.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Util/HelpFormatter.h"

#define PROPERTIES_NAME "twitter-subliminal.properties"

#define DO_CASE(N) switch (block_size) {\
    CASE(1);\
    CASE(2);\
    CASE(3);\
    CASE(4);\
    CASE(5);\
    CASE(6);\
    CASE(7);\
    CASE(8);\
    CASE(9);\
    CASE(10);\
    CASE(11);\
    CASE(12);\
    CASE(13);\
    CASE(14);\
    CASE(15);\
    CASE(16);\
    default:\
        throw Poco::InvalidArgumentException("Bad blocksize. Must be on the closed interval [1,16].", block_size );\
    }

class TwitterSubliminalApplication : public Poco::Util::Application {
public:
    virtual void initialize(Application& self) override {
        if(terminate_early) {
            return;
        }

        // Load configuration from file
        try{
            loadConfiguration(PROPERTIES_NAME);
        } catch (Poco::FileNotFoundException e) {
            logger().error("Could not find %s configuration file in working directory (or parents).", std::string(PROPERTIES_NAME));
            terminate_early = true;
            return;
        }

        // Extract configuration keys
        std::string log_file, log_level, log_pattern;
        try {
            consumer_key = config().getString("consumer.key");
            secret_key = config().getString("secret.key");
            access_token = config().getString("access.token");
            access_token_secret = config().getString("access.token.secret");

            ca_path = config().getString("ca.path");
            lang = config().getString("lang");

            bw_console = config().has("bwconsole");
            log_file = config().getString("log.file");
            log_level = config().getString("log.level");
            log_pattern = config().getString("log.pattern");
        } catch (Poco::NotFoundException e) {
            logger().error("Missing properties in confugration file. See %s.example for a template", std::string(PROPERTIES_NAME));
            displayHelp();
            terminate_early = true;
            return;
        }

        configure_logging(log_file, log_level, log_pattern);

        Application::initialize(self);
        logger().debug("Initialization complete.");
    }

    void configure_logging(const std::string &log_file, const std::string &log_level, const std::string &log_pattern) {
        Poco::AutoPtr<Poco::SplitterChannel> splitter_channel(new Poco::SplitterChannel());

        if(bw_console) {
            splitter_channel->addChannel(new Poco::ConsoleChannel());
        } else {
            splitter_channel->addChannel(new Poco::ColorConsoleChannel());
        }

        Poco::AutoPtr<Poco::PatternFormatter> pattern_formatter(new Poco::PatternFormatter);
        pattern_formatter->setProperty("pattern", log_pattern);
        Poco::AutoPtr<Poco::FormattingChannel> formatted_log_file(new Poco::FormattingChannel(pattern_formatter, new Poco::SimpleFileChannel(log_file)));
        splitter_channel->addChannel(formatted_log_file);

        Poco::Logger::root().setChannel(splitter_channel);
        Poco::Logger::root().setLevel(log_level);
        setLogger(Poco::Logger::root());
    }

    virtual void defineOptions(Poco::Util::OptionSet &options) override {
        options.addOption(Poco::Util::Option("help",
                                             "h",
                                             "display this help")
                        .required(false)
                        .repeatable(false)
                        .callback(Poco::Util::OptionCallback<TwitterSubliminalApplication>(this, &TwitterSubliminalApplication::handleHelp)));
        options.addOption(Poco::Util::Option("bwconsole",
                                             "b",
                                             "specify to log console output in one color")
                                  .required(false)
                                  .noArgument()
                                  .binding("bwconsole"));
        options.addOption(Poco::Util::Option("log.level",
                                             "l",
                                             "specify the logging LEVEL")
                                  .required(false)
                                  .argument("LEVEL")
                                  .binding("log.level"));
        options.addOption(Poco::Util::Option("log.file",
                                             "f",
                                             "specify logging output file PATH")
                                  .required(false)
                                  .argument("PATH")
                                  .binding("log.file"));
        Application::defineOptions(options);
    };

    void handleHelp(const std::string& name, const std::string& value) {
        terminate_early = true;
        displayHelp();
        stopOptionsProcessing();
    }

    void displayHelp() {
        Poco::Util::HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage(usage_line());
        helpFormatter.setHeader(application_description());
        helpFormatter.format(std::cout);
    }

    virtual std::string usage_line() {
        return "OPTIONS";
    }

    virtual int main(const std::vector<std::string> &args) override = 0;

    virtual std::string application_description() = 0;

    std::string consumer_key, secret_key, access_token, access_token_secret, ca_path, lang;
protected:
    bool bw_console = true, terminate_early = false;
};
