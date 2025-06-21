
#include "../headers/message.h"

const std::string http::Message::general_header =  "CMPMS->";
const std::string http::Message::tail = "<-CMPME\\n";
const std::string  http::Message::safeguard_begin = "CMPSGB->";
const std::string  http::Message::safeguard_end = "<-CMPSGE";

http::Message::Message(std::string &content) {
    //TODO: Refactor, ugly code
    msg_size = divideIntoChunks(content);
    chunks.push_back(tail);
    msg_size += general_header.size() + tail.size() + 12;
    std::string size_str = "0000000000";
    int i = size_str.size() - 1;
    std::string actual_size = std::to_string(msg_size);
    int j = actual_size.size() - 1;
    while(i > -1 && j > -1){
        size_str[i] = actual_size[j];
        i--; j--;
    }
    header = general_header + "(" + size_str + ")";
    chunks.insert(chunks.begin(), header);
}

size_t http::Message::divideIntoChunks(std::string &content) {
    int i = 0;
    size_t size = 0;
    std::string s;
    while(i < content.size()){
        s.clear();
        s = safeguard_begin;
        s += content.substr(i, std::min(max_bytes_sent - safeguard_begin.size() - safeguard_end.size(),
                                        content.size()));
        s += safeguard_end;
        chunks.push_back(s);
        i += max_bytes_sent - safeguard_begin.size() - safeguard_end.size();
        size += s.size();
    }
    return size;
}

std::vector<std::string>* http::Message::getChunks() {
    return &chunks;
}

size_t http::Message::size() {
    return msg_size;
}

http::Message::~Message(){
}