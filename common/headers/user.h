
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
        std::string id;
    };

    class User {
        UserServiceData data;
        void attachSocket(GeneralSocket *socket);

    public:
        User(GeneralSocket *socket);
        User() = default;
        void setId(std::string id);
        std::string getId();
        UserServiceData* getUserData();
        ~User() = default;
    };
}

#endif
