# JsoNeat

  Some C++ classes and templates to read JSON into objects in a flexible and robust way.
  
  The underlying parsing work is done by [JSMN](https://github.com/zserge/jsmn).
  
  The class is a template which allows json to be of type `char *`, `const char *` or others (?).
  There is a class to create the parser on stack.


#### Documentation
   *  View [API documentation](https://zwiebert.github.io/mbed-jsoneat/api) 
   *  Create and view API documentation `make doxy-api-view`
   *  Crate and view source documentation `make doxy-dev-view`

#### Usage
   * `#include` [jsoneat/jsoneat.hh](components/jsoneat/include/jsoneat/jsoneat.hh) to use wrapper classes. 
   * `#include jsmn/jsmn.h` to use underlying Jsmn submodule directly at some places. 
   

#### Testing
   * work in progress
   * for now tests are run from  application or a separate test package. Using git submodule to get all dependencies was not feasable.
   

   
#### Online Git Repositories
  * This repository on [GitHub](https://github.com/zwiebert/mbed-jsoneat)
  * JSON parser/tokenizer submodule: [jsmn](https://github.com/zserge/jsmn)
