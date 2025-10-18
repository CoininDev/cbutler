cmake_minimum_required(VERSION 3.24.0)
project(cbutler)
include(FetchContent)

#%<sets>%
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#%</sets>%

#%<modules>%
add_subdirectory(src/core)
#%</modules>%

#%<dependencies>%
add_subdirectory(vendors/tiny-process-library)
#%<dependency id="CLI11" src="https://github.com/CLIUtils/CLI11.git">
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.5.0
)
FetchContent_MakeAvailable(CLI11)
#%</dependencies>%

add_executable(${PROJECT_NAME} src/main.cpp src/subcommand.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    tiny-process-library::tiny-process-library
    CLI11::CLI11
)

target_include_directories(${PROJECT_NAME} PUBLIC include vendors)
