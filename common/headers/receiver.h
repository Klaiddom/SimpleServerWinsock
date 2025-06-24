#ifndef SANDBOX_MESSAGE_RECIEVER_H
#define SANDBOX_MESSAGE_RECIEVER_H
#include <string>
#include <vector>
#include "message.h"
#include "Socket.h"
#include "safeguards.h"
#include "packet.h"

namespace http{

    class MessageReceiver{
        bool receiving = false;
        static const int BUFFER_SIZE = 1024;
        char* buffer = new char[BUFFER_SIZE];
        Message* last_message = new Message();
        std::vector<Packet> packets;
        ServiceInfo* si;

        void processNextPacket(SOCKET connection);
        void composeMessage();

    public:
        MessageReceiver() = default;
        ~MessageReceiver() = default;
        Message* retrieveLastMessage(SOCKET connection);
    };

}

#endif
