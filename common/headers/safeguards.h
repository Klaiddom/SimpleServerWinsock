#ifndef SANDBOX_SAFEGUARDS_H
#define SANDBOX_SAFEGUARDS_H
#include <string>
namespace http {
    struct Safeguards {
        static const std::string PACKET_BEGIN;
        static const std::string PACKET_END;
        static const std::string MSG_BEGIN;
        static const std::string MSG_END;
        static const std::string INFO_FROM;
        static const std::string INFO_TO;
    };
}
#endif
