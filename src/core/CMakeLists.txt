project(core)
file(GLOB CORE_SRC "**.cpp")
add_library(${PROJECT_NAME} ${CORE_SRC})
target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_SOURCE_DIR}/include
)