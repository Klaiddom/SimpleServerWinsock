#include "../headers/user.h"


http::User::User(GeneralSocket* socket) {
    attachSocket(socket);
}


void http::User::setId(std::string id) {
    data.id = id;
}

void http::User::attachSocket(GeneralSocket *socket) {
    if(!data.socket)
        data.socket = socket;
}

http::UserServiceData* http::User::getUserData(){
    return &data;
}


std::string http::User::getId() {
    return data.id;
}

