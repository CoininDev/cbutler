file(GLOB <__MOD_NAME__>_SRC 
    "*.cpp"
)
add_library(<__MOD_NAME__> ${<__MOD_NAME__>_SRC})


target_include_directories(<__MOD_NAME__> PUBLIC
    ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/vendors
)
