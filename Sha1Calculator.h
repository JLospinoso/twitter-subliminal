#pragma once

#include <bitset>
#include <string>
#include <stdexcept>
#include "Poco/SHA1Engine.h"
#include "StringBitIterator.h"

class Sha1Calculator {
public:
    std::bitset<160> bitset_hash(const std::string &element) {
        auto digest_str = hash(element);
        auto bit_iterator = StringBitIterator(digest_str);
        auto ret = std::bitset<160>();
        for(int i=0; i<160; i++) {
            if (!bit_iterator.has_next()) {
                throw new std::runtime_error("POCO Digest produced malformed string.(<160 bytes)");
            }
            ret[i] = bit_iterator.next();
        }
        return ret;
    }

    std::string hash(const std::string &element) {
        digestEngine.reset();
        digestEngine.update(element);
        auto digest = digestEngine.digest();
        return std::string(digest.begin(), digest.end());
    }

private:
    Poco::SHA1Engine digestEngine;
};
