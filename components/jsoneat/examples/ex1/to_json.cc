/**
 * \brief Example program. Serialize an object to a JSON string using a char array as buffer.
 *
 *  The char buffer needs to be large enough for the entire JSON string to succeed.
 *
 */
#include "jsmn/jsmn.h"
#include "jsmn/jsmn_iterate.hh"
#include "jsoneat/to_json_cbuf.hh"

#include <type_traits>
#include <stdio.h>

//////////////////// Example: data class object ////////////////////////////////////////

struct data_struct {
  int a = -42, b = 2, c = 3;

  int to_json(char *dst, size_t dst_size) const {
    return jsoneat::to_json::cbuf::to_json_args(dst, dst_size, JSONEAT_KvPairs(a, b, c));
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
  int ia[4] = { -1, 2, -3, 4 };
  bool ba[4] = { true, false, false, true };
  unsigned ua[4] = { 5, 6, 7, 8 };
  data_struct ds;

private:
  // some members which have different names than appear in JSON
  char m_name[32] = "Ismael";
  int nmb10 = 10;
  int nmb11 = 11;
  int m_id = 4711;
  int nmb12 = 12;



public:
  int to_json(char *dst, size_t dst_size) const {
    return jsoneat::to_json::cbuf::to_json_args //this variadic template can take  any number of KvPair arguments
      (dst, dst_size, // the destination buffer for the created JSON cstring
       JSONEAT_KvPairs(a, b, c, f, s, ia, ba, ua, ds, nmb10), // create key/val pairs for up to 10 members (TODO: increase number)
       JSONEAT_KvPairs(nmb11, nmb12), // ...the macro is limited to 10 arguments for now, but a second macro call works just fine
       jsoneat::KvPair("name", m_name), jsoneat::KvPair("id", m_id)); // here the JSON names differ from actual member-names, so we provide them both
  }
};

static void example_data_class() {
  data_class data = { };

  char buf[256] = { };
  char *dst = buf;
  size_t dst_size = sizeof buf - 1;

  auto res = jsoneat::to_json::cbuf::to_json_val(dst, dst_size, data);
  if (res >= dst_size) {
    fprintf(stderr, "JSON string buffer too small: %lu.\n"
        "Try again with bigger buffer.\n"
        "Unlike snprintf(3), we do not know how much bigger the buffer needs to be.\n", dst_size);
    return;
  }

  if (res > 0 && buf[res -1] == ',')
    buf[--res] = '\0'; // remove trailing comma

  puts(buf);
}

int main() {
  example_data_class();
}
