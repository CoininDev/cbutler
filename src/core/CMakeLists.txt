file(GLOB CORE_SRC "**.cpp")
add_library(core ${CORE_SRC})
target_link_libraries(core PUBLIC tiny-process-library::tiny-process-library)

target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/vendors
)
