#include "../headers/receiver.h"


void http::MessageReceiver::processNextPacket(SOCKET connection) {
    http::Packet packet;
    memset(buffer, 0, BUFFER_SIZE * sizeof(char));
    int bytes_received = recv(connection, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        std::string tmp(buffer, bytes_received);
        auto stream = new std::stringstream(tmp);
        int result = packet.deserialize(stream);
        if (result == EXIT_FAILURE)
            packet.clear();
    }

    if (!receiving && packet.body == http::Safeguards::MSG_BEGIN) {
        receiving = true;
        last_message->clear();
        packet.clear();
    } else if (receiving && packet.body == http::Safeguards::MSG_END) {
        receiving = false;
        composeMessage();
        packet.clear();
        packets.clear();
    } else{
        packets.push_back(packet);
    }
}

http::Message* http::MessageReceiver::retrieveLastMessage(SOCKET connection) {
    last_message->clear();
    while (last_message->getSize() == 0) {
        processNextPacket(connection);
    }
    auto msg_copy = new Message(*last_message);
    return msg_copy;
}

void http::MessageReceiver::composeMessage() {
    auto msg_content = new std::string();
    for (auto &packet: packets) {
        *msg_content += packet.body;
    }
    last_message->update(*msg_content);
}