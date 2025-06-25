#include "../headers/receiver.h"


void http::MessageReceiver::processNextPacket(SOCKET connection) {
    http::Packet packet;
    memset(buffer, 0, BUFFER_SIZE * sizeof(char));
    int bytes_received = recv(connection, buffer, BUFFER_SIZE, 0);
    std::cout << "Received bytes: " << bytes_received << std::endl;
    if (bytes_received > 0) {
        std::string tmp(buffer, bytes_received);
        splitIntoPackets(tmp);
    }
    if(split_packets.size() > 1)
        int i = 0;

    while(!split_packets.empty()){
        auto stream = new std::stringstream(split_packets.front());
        int result = packet.deserialize(stream);
        if (result == EXIT_FAILURE)
            packet.clear();

        if (!receiving && packet.isMsgStart()){
            receiving = true;
            last_message->clear();
            auto msg_info = packet.getServiceInfo();
            si = msg_info;
            packet.clear();
        } else if (receiving && packet.isMsgEnd()) {
            receiving = false;
            composeMessage();
            packet.clear();
            si = nullptr;
            packets.clear();
        } else{
            packets.push_back(packet);
        }
        split_packets.pop();
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
    if(last_message->getSize() != si->size){
        std::cout << "Received msg is corrupted, expected " << si->size << " bytes, got " <<
                    last_message->getSize() << " bytes." << std::endl;
        last_message->clear();
    }
}


void http::MessageReceiver::splitIntoPackets(std::string &s) {
    int i = 0;
    while(i < s.size()){
        auto begin = s.find(http::Safeguards::PACKET_BEGIN, i);
        auto end = s.find(http::Safeguards::PACKET_END, i);
        if(begin != std::string::npos && end != std::string::npos){
            split_packets.push(s.substr(begin, end + http::Safeguards::PACKET_END.size() - begin));
            i = end + http::Safeguards::PACKET_END.size();
        } else
            break;
    }
}
