
#ifndef SANDBOX_CLIENT_H
#define SANDBOX_CLIENT_H

#include "packet.h"
#include "Socket.h"
#include "receiver.h"
#include "sender.h"
#include "thread"

namespace http {

    struct UserServiceData {
        GeneralSocket *socket = nullptr;
        std::string *id = nullptr;
    };

    class User {
        UserServiceData data;
        MessageReceiver receiver;
        MessageSender sender;
        std::queue<Message*> incoming;
        std::thread* receive_thread;


    public:
        User(GeneralSocket *socket);
        User() = default;
        void setId(std::string id);
        void attachSocket(GeneralSocket *socket);
        std::string* getId();
        void receiveMsg();
        Message* getLastMsg();
        void sendResponse(http::Message& msg);
        ~User() = default;
    };
}

#endif
