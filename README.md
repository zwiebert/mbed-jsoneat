# JsoNeat

  Some C++ classes and templates to read JSON into objects in a flexible and robust way.
  
  The underlying parsing work is done by [JSMN](https://github.com/zserge/jsmn).
  
  The class is a template which allows json to be of type `char *`, `const char *` or others (?).
  There is a class to create the parser on stack.
  
  Its written for usage in my own projects and may not be useful to others. The new documentation and example code 
  is mainly there to help me use it myself and improve the API to make it easier to use.
  


#### Documentation
   *  View [API documentation](https://zwiebert.github.io/mbed-jsoneat/api) 
   *  Create and view API documentation `make doxy-api-view`
   *  Crate and view source documentation `make doxy-dev-view`

#### Usage
   * View [Example code ex1](components/jsoneat/examples/ex1/ex1.cc) for code which compiles and runs. 
   * `#include` [jsoneat/jsoneat.hh](components/jsoneat/include/jsoneat/jsoneat.hh) to use wrapper classes. 
   * `#include jsmn/jsmn.h` to use underlying Jsmn submodule directly at some places. 
   
##### To de-serialize an object, it would like this:

```cpp
struct MyStruct {
  int id;
  char name[32];

  template<typename jsmn_iterator>
  bool from_json(jsmn_iterator &it) {
    return jsoneat::take_all_from_object(it, JSONEAT_XNSPs(id, name));
  }
};

class MyClass {
  public:
    int n;
    int narr[4];
    float f;
    float farr[5];
    bool b;
    bool barr[b];
    class MyStruct o;
    class MyStruct oarr[6];
    
  public:
  
  template<typename jsmn_iterator>
  bool from_json(jsmn_iterator &it) {
    return jsoneat::take_all_from_object(it, JSONEAT_XNSPs(n, narr, f, farr, b, barr, o, oarr));
  }
   
};

bool deserialize_my_object_from_json_string(const char *json) {

//MyClass should contain a from_json function (see example ex1)
MyClass obj = {}; // create local object with default values inside



  if (jsoneat::from_json_member(obj, json)) {
     // successful copied data from json into object
     // the JSMN token array contains 32 elements by default
     // ...and is placed on stack

     g(obj) //  here we can use the restored object

    return true;
  }

  // We are in this code line, because something went wrong
  // The numer of elements in the JSMN token-array may be to small
  // for big classes with lots of data
  // Try 128 instead of the default 32 elements...  

  if (jsoneat::from_json_member<128>(obj, json)) {
     // successful copied data from json into object
     //  here we can use the object

     g(obj) //  here we can use the restored object

    return true;
  }
  
  // its also possible to place the array in RAM allocated by "new".
  // the code can try to enlarge the array until it succeeds.

}

```


#### Testing
   * work in progress
   * for now tests are run from  application or a separate test package. Using git submodule to get all dependencies was not feasable.
   

   
#### Online Git Repositories
  * This repository on [GitHub](https://github.com/zwiebert/mbed-jsoneat)
  * JSON parser/tokenizer submodule: [jsmn](https://github.com/zserge/jsmn)