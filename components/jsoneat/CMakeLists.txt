cmake_minimum_required(VERSION 3.16)

set(srcs src/jsmn.c)



if(COMMAND idf_component_register)
idf_component_register(SRCS ${srcs}
    INCLUDE_DIRS include
)

else()
add_library(jsoneat STATIC ${srcs})
target_include_directories(jsoneat PUBLIC include)

endif()
