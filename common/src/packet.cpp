#include "../headers/packet.h"

http::Packet::Packet(std::string data) {
    body = data;
}

std::stringstream *http::Packet::serialize() {
    auto stream = new std::stringstream();

    int safeguard_size = http::Safeguards::PACKET_BEGIN.size();
    stream->write((const char *) (&safeguard_size), sizeof(safeguard_size));
    stream->write(http::Safeguards::PACKET_BEGIN.c_str(), safeguard_size);

    int body_size = body.size();
    stream->write((const char *) (&body_size), sizeof(body_size));
    stream->write(body.c_str(), body_size);

    safeguard_size = http::Safeguards::PACKET_END.size();
    stream->write((const char *) (&safeguard_size), sizeof(safeguard_size));
    stream->write(http::Safeguards::PACKET_END.c_str(), safeguard_size);

    return stream;
}

int http::Packet::deserialize(std::stringstream *stream) {
    int safeguard_size = http::Safeguards::PACKET_BEGIN.size();
//    stream->read(reinterpret_cast<char *>(&safeguard_size), sizeof(safeguard_size));
    std::string received_safeguard;
    received_safeguard.resize(safeguard_size);
    stream->read(&received_safeguard[0], safeguard_size);
    if (received_safeguard != http::Safeguards::PACKET_BEGIN)
        return EXIT_FAILURE;

    int body_size;
    stream->read(reinterpret_cast<char *>(&body_size), sizeof(body_size));
    body.resize(body_size);
    stream->read(&body[0], body_size);

    stream->read(reinterpret_cast<char *>(&safeguard_size), sizeof(safeguard_size));
    received_safeguard.resize(safeguard_size);
    stream->read(&received_safeguard[0], safeguard_size);
    if (received_safeguard != http::Safeguards::PACKET_END)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;

}

void http::Packet::clear() {
    body.clear();
}

void http::Packet::update(std::string new_body) {
    body = new_body;
}

bool http::Packet::isMsgStart() {
    if (body.size() < http::Safeguards::MSG_BEGIN.size())
        return false;
    return body.substr(0, http::Safeguards::MSG_BEGIN.size()) == http::Safeguards::MSG_BEGIN;
}

bool http::Packet::isMsgEnd() {
    return body == http::Safeguards::MSG_END;
}

bool http::Packet::isFrom(){
    if(body.size() < http::Safeguards::INFO_FROM.size())
        return false;
    return body.substr(0, http::Safeguards::INFO_FROM.size()) == http::Safeguards::INFO_FROM;
}

bool http::Packet::isTo(){
    if(body.size() < http::Safeguards::INFO_TO.size())
        return false;
    return body.substr(0, http::Safeguards::INFO_TO.size()) == http::Safeguards::INFO_TO;
}

http::ServiceInfo* http::Packet::getServiceInfo() {
    if(!isMsgStart())
        return nullptr;
    auto si = new http::ServiceInfo();
    int last_symbols = body.rfind(' ');
    si->size = std::stoi(body.substr(last_symbols, body.size() - last_symbols));
    return si;
}