#pragma once

class BitIterator {
public:
    virtual bool next() = 0;
    virtual bool has_next() const = 0;
    virtual ~BitIterator() { };
};