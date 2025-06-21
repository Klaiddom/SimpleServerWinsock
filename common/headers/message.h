#ifndef SANDBOX_MESSAGE_H
#define SANDBOX_MESSAGE_H

#include <string>
#include <vector>
//#include <math.h>

namespace http{

    class Message{

        std::string header;
        std::vector<std::string> chunks;

        size_t msg_size;
        static const int max_bytes_sent = 50;
        size_t divideIntoChunks(std::string& content);

    public:
        Message(std::string& content);
        ~Message();
        std::vector<std::string>* getChunks();
        size_t size();
        static const std::string general_header;
        static const std::string tail;
        static const std::string safeguard_begin;
        static const std::string safeguard_end;
    };

}

#endif
