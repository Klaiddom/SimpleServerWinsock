#ifndef SANDBOX_MESSAGE_RECIEVER_H
#define SANDBOX_MESSAGE_RECIEVER_H
#include <string>
#include <vector>
#include <queue>
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
        ServiceInfo* si = nullptr;
        std::queue<std::string> split_packets;

        void processNextPacket(SOCKET connection);
        void composeMessage();
        void splitIntoPackets(std::string& s);

    public:
        MessageReceiver() = default;
        ~MessageReceiver() = default;
        Message* retrieveLastMessage(SOCKET connection);
    };

}

#endif
