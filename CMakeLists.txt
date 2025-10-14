cmake_minimum_required(VERSION 3.7.0)
project(cbutler)

set(CMAKE_CXX_STANDARD 26)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(src/core)
target_link_libraries(${PROJECT_NAME} PRIVATE core)

target_include_directories(${PROJECT_NAME} PUBLIC include)