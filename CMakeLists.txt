cmake_minimum_required(VERSION 3.31)
project(sandbox)

set(CMAKE_CXX_STANDARD 14)


set(HEADERS_SERVER server/headers/main_server.h
        server/headers/http_server.h
        server/headers/server.h
        common/headers/Socket.h
)

set(SOURCES_SERVER server/src/main_server.cpp
        server/src/http_server.cpp
        server/src/server.cpp
        common/src/Socket.cpp
)


add_executable(server ${HEADERS_SERVER} ${SOURCES_SERVER})
target_link_libraries(server PRIVATE ws2_32)



set(HEADERS_CLIENT
        client/headers/http_client.h
        client/headers/client.h
        common/headers/Socket.h
)

set(SOURCES_CLIENT client/src/main_client.cpp
        client/src/http_client.cpp
        client/src/client.cpp
        common/src/Socket.cpp
        common/src/message.cpp
)


add_executable(client ${HEADERS_CLIENT} ${SOURCES_CLIENT})
target_link_libraries(client PRIVATE ws2_32)