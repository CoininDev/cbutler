cmake_minimum_required(VERSION 3.7.0)
project(cbutler)

#%<sets>%
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#%</sets>%

#%<modules>%
add_subdirectory(src/core)
#%</modules>%

#%<dependencies>%
add_subdirectory(vendors/tiny-process-library)
#%</dependencies>%

add_executable(${PROJECT_NAME} src/main.cpp src/subcommand.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    tiny-process-library::tiny-process-library
)

target_include_directories(${PROJECT_NAME} PUBLIC include vendors)
