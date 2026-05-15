idf_component_register(
    SRC_DIRS "."
    INCLUDE_DIRS "." ../include 
    REQUIRES unity jsoneat)


 
component_compile_options("SHELL:-include sdkconfig.h")
