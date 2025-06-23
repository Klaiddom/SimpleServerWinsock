#ifndef SANDBOX_PACKET_H
#define SANDBOX_PACKET_H
#include "serializable.h"
#include "safeguards.h"
#include <string>

namespace http{

    class Packet : Serializable{
    public:

        Packet() = default;
        Packet(std::string data);
        std::stringstream* serialize() override;
        int deserialize(std::stringstream* stream) override;
        void clear();
        void update(std::string new_body);
        ~Packet() = default;

        std::string body;
    };

}

#endif
