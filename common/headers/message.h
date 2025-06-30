#ifndef SANDBOX_MESSAGE_H
#define SANDBOX_MESSAGE_H

#include <string>
#include "safeguards.h"

namespace http{

    class Message{
        std::string content;
        int msg_size;
        int char_pointer = 0;
        std::string from;
        std::string to;

    public:
        Message() = default;
        Message(std::string& content);
        Message(std::string& content, std::string& from);
        Message(Message& msg);
        ~Message() = default;
        std::string getChunk(int chunk_size);
        int getSize();
        void clear();
        std::string& getFrom();
        void setFrom(std::string& user_id);
        void setTo(std::string& user_id);
        std::string* getContent();
        void update(std::string& new_content);
    };

}

#endif
