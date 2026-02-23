cmake_minimum_required(VERSION 3.16)
project(ex1)

add_subdirectory("../.." "${CMAKE_BINARY_DIR}/jsoneat")

add_executable(from_json from_json.cc)
target_link_libraries(from_json jsoneat)

add_executable(to_json to_json.cc)
target_link_libraries(to_json jsoneat)

add_executable(from_to_json  from_to_json.cc)
target_link_libraries(from_to_json jsoneat)

