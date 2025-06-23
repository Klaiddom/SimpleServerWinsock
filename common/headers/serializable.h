
#ifndef SANDBOX_SERIALIZEBLE_H
#define SANDBOX_SERIALIZEBLE_H

#include <cstring>
#include <sstream>
#include <utility>

class Serializable{
public:
    virtual std::stringstream* serialize() = 0;
    virtual int deserialize(std::stringstream* stream) = 0;
};

#endif