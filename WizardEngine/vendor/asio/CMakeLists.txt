cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE ASIO_SRC
        asio/src/asio.cpp
        asio/src/asio_ssl.cpp
        )
add_library(asio ${ASIO_SRC})

include_directories(asio/include)