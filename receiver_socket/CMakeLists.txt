cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME receiver_lib)
project(${PROJECT_NAME} VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} 
    src/receiver_socket.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE

)

target_include_directories(${PROJECT_NAME} PUBLIC
    include 
)