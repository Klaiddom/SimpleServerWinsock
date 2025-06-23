
#include "../headers/message.h"

http::Message::Message(std::string &content) {
    this->content = content;
    msg_size = content.size();
}

http::Message::Message(http::Message &msg) {
    content = msg.content;
    msg_size = content.size();
}

int http::Message::getSize() {
    return msg_size;
}

std::string http::Message::getChunk(int chunk_size) {
    if(char_pointer >= content.size()){
        char_pointer = 0;
        return "";
    }
    std::string chunk = content.substr(char_pointer, std::min(chunk_size, (int)content.size() - char_pointer));
    char_pointer += chunk_size;
    return chunk;
}

std::string* http::Message::getContent(){
    return &content;
}

void http::Message::clear() {
    content.clear();
    msg_size = 0;
}

void http::Message::update(std::string &new_content) {
    content = new_content;
    msg_size = content.size();
}