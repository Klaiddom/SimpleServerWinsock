
#ifndef SANDBOX_CLIENT_H
#define SANDBOX_CLIENT_H

#include "packet.h"
#include "Socket.h"
#include "receiver.h"

namespace http {

    struct UserServiceData {
        GeneralSocket *socket = nullptr;
        std::string *id = nullptr;
    };

    class User {
        UserServiceData data;
        MessageReceiver receiver;
        std::queue<Message*> msgs;

    public:
        User(GeneralSocket *socket);

        User() = default;

        void setId(std::string id);

        void attachSocket(GeneralSocket *socket);

        std::string* getId();

        void receiveMsg();

        Message* getLastMsg();

        GeneralSocket* getSocket();

        ~User() = default;
    };
}

#endif
