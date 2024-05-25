cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME oswitch)
project(${PROJECT_NAME} VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(receiver_socket)
add_subdirectory(sender_socket)

# file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})



add_executable(${PROJECT_NAME} 
    main.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        receiver_lib
        sender_lib
        pthread
)