#include "../headers/sender.h"

void http::MessageSender::send(http::Message &msg, GeneralSocket* connected_socket) {
    http::Packet packet;
    std::string serialize_packet;
    int bytes_send;
    std::string service_information = http::Safeguards::MSG_BEGIN;
    service_information += "<size>: " + std::to_string(msg.getSize());
    packet.update(service_information);
    serialize_packet = packet.serialize()->str();
    bytes_send = ::send(connected_socket->getRaw(), serialize_packet.c_str(), serialize_packet.size(), 0);
//    std::cout << "Packet size: " << serialize_packet.size() << ", Bytes send: " << bytes_send << std::endl;

    bool msg_can_be_processed = true;
    int expected_packet_size = packet_size_bytes + http::Safeguards::PACKET_BEGIN.size() +
                               http::Safeguards::PACKET_END.size();

    while(msg_can_be_processed){
        //Need sleep for tcp not squash multiple msg into one...
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        packet.clear();
        serialize_packet.clear();

        packet.update(msg.getChunk(packet_size_bytes));
        serialize_packet = packet.serialize()->str();
        bytes_send = ::send(connected_socket->getRaw(), serialize_packet.c_str(), serialize_packet.size(), 0);
//        std::cout << "Packet size: " << serialize_packet.size() << ", Bytes send: " << bytes_send << std::endl;

        if(serialize_packet.size() < expected_packet_size)
            msg_can_be_processed = false;
    }

    packet.clear();
    packet.update(http::Safeguards::MSG_END);
    serialize_packet.clear();
    serialize_packet = packet.serialize()->str();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    bytes_send = ::send(connected_socket->getRaw(), serialize_packet.c_str(), serialize_packet.size(), 0);
//    std::cout << "Packet size: " << serialize_packet.size() << ", Bytes send: " << bytes_send << std::endl;
}