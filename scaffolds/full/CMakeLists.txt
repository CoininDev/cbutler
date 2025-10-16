cmake_minimum_required(VERSION 3.7.0)
project(<__PROJECT_NAME__>)


#%<set>%
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#%</set>%

#%<modules>%
#%</modules>%

#%<dependencies>%
#%</dependencies>%

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include vendors)