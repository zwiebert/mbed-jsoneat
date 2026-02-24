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

  int _to_json(char *dst, size_t dst_size) const {
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
  data_struct oa[4];

private:
  // some members which have different names than appear in JSON
  char m_name[32] = "Ismael";
  int nmb10 = 10;
  int nmb11 = 11;
  int m_id = 4711;
  int nmb12 = 12;

public:
  int _to_json(char *dst, size_t dst_size) const {
    return jsoneat::to_json::cbuf::to_json_args //this variadic template can take  any number of KvPair arguments
    (dst, dst_size, // the destination buffer for the created JSON cstring
        JSONEAT_KvPairs(a, b, c, f, s, ia, ba, ua, ds, nmb10), // create key/val pairs for up to 10 members (TODO: increase number)
     JSONEAT_KvPairs(nmb11, nmb12, oa), // ...the macro is limited to 10 arguments for now, but a second macro call works just fine
        jsoneat::KvPair("name", m_name), jsoneat::KvPair("id", m_id)); // here the JSON names differ from actual member-names, so we provide them both
  }
};

static void example_data_object_to_json() {
  data_class data = { };

  char buf[512] = { };
  char *dst = buf;
  size_t dst_size = sizeof buf - 1;

  auto json_length = jsoneat::to_json::cbuf::to_json_val(dst, dst_size, data, false);
  fprintf(stderr, "reported length of JSON string: %d, buffer size: %lu.\n", json_length, dst_size);

  if (json_length >= dst_size) {
    fprintf(stderr, "JSON string buffer too small: %lu. Needs at least %d bytes\n", dst_size, json_length);
    return;
  }

  puts(buf);
}


static void test_example_data_object_to_json() {
  data_class data = { };

  char buf[256] = { };
  char *dst = buf;
  size_t dst_size = sizeof buf;
  memset(dst, 42, dst_size);
  auto predicted_json_length = jsoneat::to_json::cbuf::to_json_val(dst, 0, data, false);
  fprintf(stderr, "reported length of JSON string: %d, buffer size: %lu.\n", predicted_json_length, dst_size);
  assert(*dst == 42);  // nothing should have been written to dst if called with dst_size=0.

  auto json_length = jsoneat::to_json::cbuf::to_json_val(dst, dst_size, data, false);
  fprintf(stderr, "reported length of JSON string: %d, buffer size: %lu.\n", json_length, dst_size);
  assert(predicted_json_length == json_length);  // if predicted length is wrong, there must be bug in the library

  if (json_length >= dst_size) {
    fprintf(stderr, "JSON string buffer too small: %lu. Needs at least %d bytes\n", dst_size, json_length);
    return;
  }


  fprintf(stderr, "strlen json: %lu\n", strlen(dst));
  assert(strlen(dst) == json_length);

  puts(buf);
}

int main() {
  example_data_object_to_json();
  test_example_data_object_to_json();
}
