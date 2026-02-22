cmake_minimum_required(VERSION 3.16)
project(ex1)



set(CMAKE_CXX_FLAGS_DEBUG  "-g")
add_compile_options("-fconcepts")
add_subdirectory("../.." "${CMAKE_BINARY_DIR}/jsoneat")

add_executable("ex1" "ex1.cc")
target_link_libraries(ex1 jsoneat)

add_executable("ex2" "ex2.cc")
target_link_libraries(ex2 jsoneat)
