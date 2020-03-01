cmake_minimum_required(VERSION 3.15)
project(tcp_lab C)

set(CMAKE_C_STANDARD 99)

add_executable(server server.c)

add_executable(client client.c)