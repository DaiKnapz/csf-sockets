cmake_minimum_required(VERSION 3.3)
project(csf_sockets)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

set(SOURCE_FILES
    MultiQuoteClient.c
    quotes)

add_executable(csf_sockets ${SOURCE_FILES})