#include <iostream>
#include <string>
#include "common/headers/serializable.h"

class SerializableTest : public Serializable{
public:
    SerializableTest() = default;
    ~SerializableTest() {};
    SerializableTest(std::string s, int n){
        this->s = s;
        number = n;
    };

    std::stringstream* serialize() override{
        std::stringstream* stream = new std::stringstream();
        int s_size = s.size();
        stream->write((const char*)(&s_size), sizeof(s_size));
        stream->write(s.c_str(), s_size);
        stream->write((const char*)(&number), sizeof(number));
        return stream;
    };

    int deserialize(std::stringstream* stream) override {
        int s_size;
        stream->read(reinterpret_cast<char*>(&s_size), sizeof(s_size));
        s.resize(s_size);
        stream->read(&s[0], s_size);
        stream->read(reinterpret_cast<char*>(&number), sizeof(number));
        return EXIT_SUCCESS;
    }

    void print(){
        std::cout << s << " " << number << " " << sizeof(*this) << std::endl;
    };

    std::string s;
    int number;
};


int main(){
    SerializableTest obj("Test string", 5);
    obj.print();
    auto serialized_obj_data = obj.serialize();
    SerializableTest recovered;
    recovered.deserialize(serialized_obj_data);
    recovered.print();
    return EXIT_SUCCESS;
}