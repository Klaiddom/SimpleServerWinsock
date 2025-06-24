#ifndef SANDBOX_SENDER_H
#define SANDBOX_SENDER_H

#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include "message.h"
#include "packet.h"
#include "Socket.h"

namespace http{

    class MessageSender{

    public:
        static const int packet_size_bytes = 50;

        MessageSender() = default;
        void send(Message& msg, GeneralSocket* connected_socket);
        ~MessageSender() = default;
    };

}

#endif
