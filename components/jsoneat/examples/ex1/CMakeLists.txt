cmake_minimum_required(VERSION 3.16)
project(ex1)

set(srcs  ex1.cc)



if(COMMAND idf_component_register)
idf_component_register(SRCS ${srcs}
    INCLUDE_DIRS include
)

else()
set(CMAKE_CXX_FLAGS_DEBUG  "-g")
add_compile_options("-fconcepts")
add_subdirectory("../.." "${CMAKE_BINARY_DIR}/jsoneat")
add_executable("ex1" "${srcs}")
target_link_libraries(ex1 jsoneat)
endif()
