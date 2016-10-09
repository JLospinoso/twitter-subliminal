#pragma once

#include "Twitter.h"
#include <memory>

class TwitterFactory {
public:
    TwitterFactory(std::string consumer_secret_key, std::string secret_key, std::string access_token,
                   std::string access_token_secret, std::string ca_path, std::string lang)
        : consumer_secret_key(consumer_secret_key), secret_key(secret_key), access_token(access_token),
          access_token_secret(access_token_secret), ca_path(ca_path), lang(lang) { }

    std::string get_consumer_key() const {
        return consumer_secret_key;
    }

    std::string get_secret_key() const {
        return secret_key;
    }

    std::string get_access_token() const {
        return access_token;
    }

    std::string get_access_token_secret() const {
        return access_token_secret;
    }

    std::string get_ca_path() const {
        return ca_path;
    }

    std::string get_lang() const {
        return lang;
    }

    std::unique_ptr<Twitter> make() const {
        return std::make_unique<Twitter>(consumer_secret_key, secret_key, access_token, access_token_secret, ca_path, lang);
    }

private:
    const std::string consumer_secret_key, secret_key, access_token, access_token_secret, ca_path, lang;
};