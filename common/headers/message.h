#ifndef SANDBOX_MESSAGE_H
#define SANDBOX_MESSAGE_H

#include <string>

namespace http{

    class Message{
        std::string content;
        int msg_size;
        int char_pointer = 0;

    public:
        Message() = default;
        Message(std::string& content);
        Message(Message& msg);
        ~Message() = default;
        std::string getChunk(int chunk_size);
        int getSize();
        void clear();
        std::string* getContent();
        void update(std::string& new_content);
    };

}

#endif
