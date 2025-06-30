#include "../headers/poll_receiver.h"

http::POLLReceiver::POLLReceiver() {
    process_thread = new std::thread(&POLLReceiver::process, this);
}

void http::POLLReceiver::start() {
    process_thread->detach();
}

void http::POLLReceiver::addConnection(GeneralSocket *listening_socket) {
    if(connected_users >= max_clients){
        std::cerr << "Max client count is reached" << std::endl;
        return;
    }
    msgs.insert({listening_socket->getRaw(), std::queue<Message* >()});
    socket_map[listening_socket->getRaw()] = connected_users;

    poll_group[connected_users].fd = listening_socket->getRaw();
    poll_group[connected_users].events = POLLRDNORM;
    connected_users++;
}

void http::POLLReceiver::removeConnection(SOCKET raw_socket) {
    if(socket_map.find(raw_socket) == socket_map.end())
        return;

    auto last_socket = poll_group[connected_users - 1];
    int removed_position = socket_map[raw_socket];
    socket_map[last_socket.fd] = removed_position;
    socket_map.erase(raw_socket);
    msgs.erase(raw_socket);
    poll_group[removed_position] = last_socket;
    connected_users--;
}

void http::POLLReceiver::process() {
    int received_bytes;
    std::vector<SOCKET> removed_list;
    while(true){
        removed_list.clear();
        int result = WSAPoll(poll_group, connected_users, wait_millisecond);
        if(result > 0){
            for(int i=0; i < connected_users; i++){
                if(poll_group[i].revents & POLLRDNORM){
                    memset(input_buffer, 0, buffer_size);
                    received_bytes = recv(poll_group[i].fd, input_buffer, buffer_size, 0);
                    if(received_bytes <= 0){
                        removed_list.push_back(poll_group[i].fd);
                    } else{
                        extractMessages(poll_group[i].fd, received_bytes);
                    }
                }
            }
        }
        if(!removed_list.empty()) {
            for (auto &elem: removed_list)
                removeConnection(elem);
        }
    }
}


void http::POLLReceiver::extractMessages(SOCKET socket, int received_bytes) {
    auto split_buffer =  splitIntoPackets(received_bytes);
    for(auto p: *split_buffer)
        packets[socket].push_back(p);

    delete split_buffer;

    Message* msg = composeMessage(socket);
    if(msg){
        msgs[socket].push(msg);
        packets[socket].clear();
    }
}


std::queue<http::Message* >* http::POLLReceiver::getMessages(GeneralSocket *socket) {
    if(msgs.find(socket->getRaw()) != msgs.end())
        return &msgs[socket->getRaw()];
    return nullptr;
}

std::vector<http::Packet* >* http::POLLReceiver::splitIntoPackets(int received_bytes) {
    int i = 0;
    auto split_packets = new std::vector<http::Packet* >();
    std::string s(input_buffer, received_bytes);
    std::string packet_string;
    std::stringstream ss;
    while(i < s.size()){
        auto begin = s.find(http::Safeguards::PACKET_BEGIN, i);
        auto end = s.find(http::Safeguards::PACKET_END, i);
        if(begin != std::string::npos && end != std::string::npos){
            ss.clear();
            ss << s.substr(begin, end + http::Safeguards::PACKET_END.size() - begin);
            auto packet = new Packet();
            packet->deserialize(&ss);
            split_packets->push_back(packet);
            i = end + http::Safeguards::PACKET_END.size();
        } else
            break;
    }
    return split_packets;
}

http::Message* http::POLLReceiver::composeMessage(SOCKET socket) {
    std::string msg_content;

    int msg_begin = -1, msg_end = -1;

    for(int i =0; i < packets[socket].size(); i++){
        if(packets[socket][i]->isMsgStart())
            msg_begin = i + 1;
        if(packets[socket][i]->isMsgEnd())
            msg_end = i - 1;
    }

    if(msg_begin == -1 || msg_end == -1)
        return nullptr;

    auto msg = new Message();

    for(int i=msg_begin; i <= msg_end; i++) {
            auto packet = packets[socket][i];
            if(packet->isFrom())
                msg->setFrom(packet->body);
            else if(packet->isTo())
                msg->setTo(packet->body);
            else
                msg_content += packet->body;
    }
    msg->update(msg_content);
    return msg;
}