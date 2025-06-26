#include "../headers/user.h"


http::User::User(GeneralSocket* socket) {
    attachSocket(socket);
}

void http::User::setId(std::string id) {
    data.id = &id;
}

void http::User::attachSocket(GeneralSocket *socket) {
    if(!data.socket)
        data.socket = socket;
}

GeneralSocket* http::User::getSocket() {
    return data.socket;
}



std::string* http::User::getId() {
    return data.id;
}

void http::User::receiveMsg() {
    msgs.push(receiver.retrieveLastMessage(data.socket->getRaw()));
}

http::Message* http::User::getLastMsg() {
    if(msgs.empty())
        return nullptr;
    auto msg = msgs.front();
    msgs.pop();
    return msg;
}