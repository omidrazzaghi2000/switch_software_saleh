cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME concatinator_lib)
project(${PROJECT_NAME} VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        src/concatinator.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    channel_lib
        tcp_client_server
)

target_include_directories(${PROJECT_NAME} PUBLIC
        include
)