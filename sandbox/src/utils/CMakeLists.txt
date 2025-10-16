file(GLOB utils_SRC 
    "*.cpp"
)
add_library(utils ${utils_SRC})


target_include_directories(utils PUBLIC
    ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/vendors
)
