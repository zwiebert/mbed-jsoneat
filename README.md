# jsmnit

  Some C++ classes and templates to read JSON into objects in a flexible and robust way.
  
  The actual parsing work is done by [JSMN](https://github.com/zserge/jsmn).
  
  The class is a template which allows json to be of type `char *`, `const char *` or others (?).
  There is a class to create the parser on stack.
   
#### How to add to a project   

<details> <summary>Using in ESP-IDF Project</summary>

   * The component(s) are located in subdirectory `./components`
   * The required external components are located in subdirectory `./external`
   * I place this repository into project subdirectory `./external`. Then I use file(GLOB ..)
     to add all components directories to `EXTRA_COMPONENTS_DIR` like so:
       * `file(GLOB extra_dirs external/*/components external/*/external/*/components)`      
       * `list(APPEND EXTRA_COMPONENTS_DIR ${extra_dirs})`
       * `REQUIRE ... jsmn ...`

</details>

<details> <summary>Using in CMake Project</summary>

   * source-CMakeLists.txt: `target_link_libraries(main PUBLIC ... jsmn ...)`
   * project-CMakeLists.txt: `add_subdirectory(external/jsmnit/components/jsmn)`

</details>

<details> <summary>Using in IDE like STM32CubeIde</summary>

   * add include path: `external/jsmnit/components/jsmn/include`
   * add source path: `external/jsmnit/components/jsmn/src`

</details>

#### Usage
   * `#include` [jsmn/jsmn_iterate.hh](components/jsmn/include/jsmn/jsmn_iterate.hh) to use wrapper class. 
   * `#include jsmn/jsmn.h` to use pure Jsmn in some places. 

#### Documentation
   *  `make doxy-usr-view` builds the doc with doxygen and opens it in web-browser
   * see make help for more

#### Testing  
   *  `make host-test-all`  configure, build and runs all tests

#### Todo
<details> <summary>class names</summary>

    * naming oft the classes could be updated to reflect some changes made on the way

</details>
<details> <summary>Add examples and tests</summary>

       * Tests are currently done in tests of components using this component. Examples can be found there also.

</details>
   
#### Git Repositories

<details> <summary>This repository </summary>

  * This repository on [GitHub](https://github.com/zwiebert/jsmnit)

</details>
<details> <summary>Required Submodules</summary>

  * JSON parser/tokenizer: [jsmn](https://github.com/zserge/jsmn)

</details>
<details> <summary>Optional Submodules</summary>

  * esp-idf/unity component to run tests on host: [host-test-idf](https://github.com/zwiebert/host-test-idf)

</details>
