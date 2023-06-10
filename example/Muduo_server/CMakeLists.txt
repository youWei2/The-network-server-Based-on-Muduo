cmake_minimum_required(VERSION 3.10)

set(SRC_LIST  MuduoServer.cc)

add_executable(MuduoServer ${SRC_LIST})

target_link_libraries(MuduoServer mymuduo pthread)

add_definitions(-std=c++17)
