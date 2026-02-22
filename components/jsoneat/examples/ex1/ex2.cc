#include "jsmn/jsmn.h"
#include "jsmn/jsmn_iterate.hh"
#include "jsoneat/to_json_cbuf.hh"

#include <type_traits>
#include <stdio.h>

//////////////////// Example: data class object ////////////////////////////////////////



struct data_struct {
  int a = -42, b = 2, c = 3;

    int to_json(char *dst, size_t dst_size) const {
      return  jsoneat::to_json::cbuf::to_json_args(dst, dst_size, JSONEAT_XNSPs(a, b, c));
  }


};

class data_class {
public:
  // some data members
  int a = 1;
  bool b = true;
  unsigned c = 3;
  float f = -1.234;
  char s[32] = "hello data class object";
  int ia[4] = {-1,2,-3,4};
  bool ba[4] = {true, false, false, true};
  unsigned ua[4] = {5,6,7,8};
  data_struct ds;


public:
  int to_json(char *dst, size_t dst_size) const {
    return jsoneat::to_json::cbuf::to_json_args(dst, dst_size, JSONEAT_XNSPs(a, b, c, f, s, ia, ba, ua, ds));
  }
};



static void example_data_class() {
  data_class data = {};
    char buf[256] = {};
    jsoneat::to_json::cbuf::to_json_val(buf, sizeof buf -1, data);

    puts(buf);
}

int main() {
  example_data_class();
}
