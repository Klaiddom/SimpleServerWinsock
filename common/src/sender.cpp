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

    packet.clear();
    service_information.clear();
    service_information += http::Safeguards::INFO_FROM + msg.getFrom();
    packet.update(service_information);
    serialize_packet = packet.serialize()->str();
    bytes_send = ::send(connected_socket->getRaw(), serialize_packet.c_str(), serialize_packet.size(), 0);

    packet.clear();
    service_information.clear();
    service_information += http::Safeguards::INFO_TO + msg.getTo();
    packet.update(service_information);
    serialize_packet = packet.serialize()->str();
    bytes_send = ::send(connected_socket->getRaw(), serialize_packet.c_str(), serialize_packet.size(), 0);

    bool msg_can_be_processed = true;
    int expected_packet_size = packet_size_bytes + http::Safeguards::PACKET_BEGIN.size() +
                               http::Safeguards::PACKET_END.size();

    while(msg_can_be_processed){
        packet.clear();
        serialize_packet.clear();

        packet.update(msg.getChunk(packet_size_bytes));
        serialize_packet = packet.serialize()->str();
        bytes_send = ::send(connected_socket->getRaw(), serialize_packet.c_str(), serialize_packet.size(), 0);
        if(serialize_packet.size() < expected_packet_size)
            msg_can_be_processed = false;
    }

    packet.clear();
    packet.update(http::Safeguards::MSG_END);
    serialize_packet.clear();
    serialize_packet = packet.serialize()->str();
    bytes_send = ::send(connected_socket->getRaw(), serialize_packet.c_str(), serialize_packet.size(), 0);
}