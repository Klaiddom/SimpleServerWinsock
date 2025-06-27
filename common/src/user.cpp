#include "../headers/user.h"


http::User::User(GeneralSocket* socket) {
    attachSocket(socket);
    sender = MessageSender();
    receiver = MessageReceiver();
    receive_thread = new std::thread(&http::User::receiveMsg, this);
    receive_thread->detach();
}

void http::User::setId(std::string id) {
    data.id = &id;
}

void http::User::attachSocket(GeneralSocket *socket) {
    if(!data.socket)
        data.socket = socket;
}


std::string* http::User::getId() {
    return data.id;
}

void http::User::receiveMsg() {
    while(true) {
        auto msg = receiver.retrieveLastMessage(data.socket->getRaw());
        incoming.push(msg);
    }
}

http::Message* http::User::getLastMsg() {
    if(incoming.empty())
        return nullptr;
    auto msg = incoming.front();
    incoming.pop();
    return msg;
}

void http::User::sendResponse(http::Message& msg) {
    sender.send(msg, data.socket);
}

void http::Message::setFrom(std::string &user_id) {
    from = user_id;
}