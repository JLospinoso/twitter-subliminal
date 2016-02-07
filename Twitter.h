#pragma once

#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/OAuth10Credentials.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Util/JSONConfiguration.h"
#include "Poco/URI.h"
#include "Poco/Format.h"
#include "Poco/StreamCopier.h"
#include "Poco/Logger.h"

typedef std::function<void(std::string)> stream_callback;

class Twitter  {
public:
    Twitter(std::string consumer_key, std::string consumer_secret_key, std::string access_token,
            std::string access_token_secret, std::string ca_path, std::string lang, size_t request_timeout_seconds = 5) :
            ca_path(ca_path), lang(lang),
            creds(consumer_key, consumer_secret_key, access_token, access_token_secret),
            context(new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, ca_path, ca_path.empty() ? Poco::Net::Context::VERIFY_NONE : Poco::Net::Context::VERIFY_RELAXED)),
            logger(Poco::Logger::get("Twitter")), request_timeout_seconds(request_timeout_seconds) { };
    ~Twitter() = default;
    Twitter(const Twitter &that) = delete;
    Twitter & operator=(const Twitter &that) = delete;
    Twitter(Twitter &&that) = delete;
    Twitter & operator=(Twitter &&that) = delete;

    void stream(stream_callback callback) {
        Poco::URI uri("https://stream.twitter.com/1.1/statuses/sample.json");
        uri.addQueryParameter("language", lang);
        receive_buffered("GET", uri, callback);
    };

    std::string statusDestroyById(const std::string& statusId) {
        std::string destroy_string("https://api.twitter.com/1.1/statuses/destroy/");
        destroy_string.append(statusId);
        destroy_string.append(".json");
        Poco::URI uri(destroy_string);
        return receive("POST", uri);
    };

    std::string getRateLimitStatus() {
        Poco::URI uri("https://api.twitter.com/1.1/application/rate_limit_status.json");
        return receive("GET", uri);
    }

    std::string retweetById(const std::string& statusId) {
        std::string retweet_string("https://api.twitter.com/1.1/statuses/retweet/");
        retweet_string.append(statusId);
        retweet_string.append(".json");
        Poco::URI uri(retweet_string);
        return receive("POST", uri);
    };

    std::string  timelineUserGet(bool includeRetweets,
                          std::string maxId = "",
                          std::string userId = "") {
        Poco::URI uri("https://api.twitter.com/1.1/statuses/user_timeline.json");
        if(!userId.empty()) {
            uri.addQueryParameter("user_id", userId);
        }
        if(!maxId.empty()) {
            uri.addQueryParameter("max_id", maxId);
        }
        if(!includeRetweets) {
            uri.addQueryParameter("include_rts", "false");
        }
        uri.addQueryParameter("trim_user", "true");
        return receive("GET", uri);
    };
private:
    std::string receive(std::string type, Poco::URI uri) {
        Poco::Net::HTTPRequest request(type, uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
        Poco::Net::HTMLForm form(request);
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
        creds.authenticate(request, uri, form);
        std::ostream& ostr = session.sendRequest(request);
        Poco::Net::HTTPResponse res;
        std::istream& rs = session.receiveResponse(res);
        if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
            std::string result(std::istreambuf_iterator<char>(rs), {});
            return result;
        } else {
            Poco::AutoPtr<Poco::Util::JSONConfiguration> pResult = new Poco::Util::JSONConfiguration(rs);
            std::string error_message {pResult->getString("errors[0].message", "")};
            logger.error(error_message);
            throw std::runtime_error(error_message);
        }
    }

    void receive_buffered(std::string type, Poco::URI uri, stream_callback callback) {
        Poco::Net::HTTPRequest request(type, uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
        Poco::Net::HTMLForm form(request);
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
        creds.authenticate(request, uri, form);
        std::ostream& ostr = session.sendRequest(request);
        Poco::Net::HTTPResponse res;
        std::atomic_bool response_received(false);
        std::thread check_thread([&](){
            std::this_thread::sleep_for(std::chrono::seconds(request_timeout_seconds));
            if(!response_received) {
                logger.critical("Twitter receive_buffer is hanging. Check SSL settings; is your system clock correct?");
            }
            return;
        });
        check_thread.detach();
        std::istream& result_stream = session.receiveResponse(res);
        response_received.store(true);
        if (res.getStatus() != Poco::Net::HTTPResponse::HTTP_OK) {
            Poco::AutoPtr<Poco::Util::JSONConfiguration> pResult = new Poco::Util::JSONConfiguration(result_stream);
            std::string error_message {pResult->getString("errors[0].message", "")};
            logger.error(error_message);
            throw std::runtime_error(error_message);
        }
        while(result_stream){
            std::string line;
            std::getline(result_stream, line);
            line.pop_back(); // Remove carriage return.
            callback(line);
        }
    }

    const size_t request_timeout_seconds;
    std::string ca_path, lang;
    Poco::Net::OAuth10Credentials creds;
    Poco::Net::Context::Ptr context;
    Poco::Logger &logger;
    std::array<char, 100000> read_buffer;
};
