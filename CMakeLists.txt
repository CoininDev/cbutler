cmake_minimum_required(VERSION 3.7.0)
project(cbutler)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_subdirectory(src/core)
add_subdirectory(vendors/tiny-process-library)
add_executable(${PROJECT_NAME} src/main.cpp src/subcommand.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    tiny-process-library::tiny-process-library
)


target_include_directories(${PROJECT_NAME} PUBLIC include vendors)
