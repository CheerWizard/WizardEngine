cmake_minimum_required(VERSION 3.20)

include_directories(asio/include)
add_library(asio asio/include/*.hpp asio/src/*.cpp)