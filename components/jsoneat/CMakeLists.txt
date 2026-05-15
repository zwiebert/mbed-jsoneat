cmake_minimum_required(VERSION 3.16)

set(srcs src/jsmn.c)
set(platform_srcs "") 
include("${CMAKE_CURRENT_LIST_DIR}/../../source_filter.cmake" OPTIONAL)



if(COMMAND idf_component_register)
idf_component_register(SRCS ${srcs}
    INCLUDE_DIRS include
)

else()
add_library(jsoneat STATIC ${srcs})
target_include_directories(jsoneat PUBLIC include)

endif()
