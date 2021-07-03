cmake_minimum_required(VERSION 3.6)
project(Async_TCP_Server VERSION 1.0.0 LANGUAGES CXX)

find_package(Boost COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp Socket_server.cpp Socket_server.h Socket_server_delegate.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Boost::thread)
