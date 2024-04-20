# JsoNeat

  Some C++ classes and templates to read JSON into objects in a flexible and robust way.
  
  The underlying parsing work is done by [JSMN](https://github.com/zserge/jsmn).
  
  The class is a template which allows json to be of type `char *`, `const char *` or others (?).
  There is a class to create the parser on stack.


#### Documentation
   *  View [API documentation](https://zwiebert.github.io/JsoNeat) 
   *  Create API documentation `make doxy-api-view`
   *  Crate source documentation `make doxy-dev-view`

#### Usage
   * `#include` [jsoneat/jsoneat.hh](components/jsmn/include/jsoneat/jsoneat.hh) to use wrapper classes. 
   * `#include jsmn/jsmn.h` to use underlying Jsmn submodule directly at some places. 
   
#### Build

#####  Build in ESP-IDF Project
   * The component(s) are located in subdirectory `./components`
   * The required external components are located in subdirectory `./external`
   * I place this repository into project subdirectory `./external`. Then I use file(GLOB ..)
     to add all components directories to `EXTRA_COMPONENTS_DIR` like so:
       * `file(GLOB extra_dirs external/*/components external/*/external/*/components)`      
       * `list(APPEND EXTRA_COMPONENTS_DIR ${extra_dirs})`
       * `REQUIRE ... jsmn ...`

##### Build in CMake Project
   * source-CMakeLists.txt: `target_link_libraries(main PUBLIC ... jsmn ...)`
   * project-CMakeLists.txt: `add_subdirectory(external/jsmnit/components/jsmn)`


##### Build in IDE like STM32CubeIde
   * add include path: `external/jsmnit/components/jsmn/include`
   * add source path: `external/jsmnit/components/jsmn/src`


#### Testing
   * work in progress
   * for now tests are run from  application or a separate test package. Using git submodule to get all dependencies was not feasable.
   

   
#### Online Git Repositories
  * This repository on [GitHub](https://github.com/zwiebert/jsmnit)
  * JSON parser/tokenizer submodule: [jsmn](https://github.com/zserge/jsmn)
