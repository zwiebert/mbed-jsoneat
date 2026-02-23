/**
 * \brief Example program. Uses a single macro to create working from_json() and to_json() member function for a struct or class.
 *
 */
#include "jsmn/jsmn.h"
#include "jsmn/jsmn_iterate.hh"
#include "jsoneat/from_to_json_jsmn_cbuf.hh"


#include <type_traits>
#include <stdio.h>

//////////////////// Example: data class object ////////////////////////////////////////

struct data_struct {
  int a = -42, b = 2, c = 3;

  // this macro creates the from_json and to_json  member functions
  JSONEAT_SER_FROM_TO(JSONEAT_KvPairs(a, b, c));
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
  JSONEAT_SER_FROM_TO( //
       JSONEAT_KvPairs(a, b, c, f, s, ia, ba, ua, ds, nmb10), // create key/val pairs for up to 10 members (TODO: increase number)
       JSONEAT_KvPairs(nmb11, nmb12), // ...the macro is limited to 10 arguments for now, but a second macro call works just fine
        jsoneat::KvPair("name", m_name), jsoneat::KvPair("id", m_id) // here the JSON names differ from actual member-names, so we provide them both
            );
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

  if (res > 0 && dst[res -1] == ',')
    dst[--res] = '\0'; // remove trailing comma



  puts("\n#### generated JSON string for serialized object:\n");
  puts(dst);

  // Object with a from_json() member template function
  data_class json_data = {};
  memset(&json_data, 0, sizeof json_data);

  puts("\n#### de-serialize a nulled object from this JSON and print member values:");
  if (jsoneat::from_json_member<512>(json_data, dst) || true) {
    printf("data_class object: a=%d, b=%d, c=%d, f=%f,  s=%s, ia=[%d, %d, %d, %d]\n", json_data.a, json_data.b, json_data.c, json_data.f, json_data.s,
        json_data.ia[0], json_data.ia[1], json_data.ia[2], json_data.ia[3]);
  }


  puts("\n#### serialize the de-serialized object again and compare JSON strings for both objects. They should match");
  char buf2[sizeof buf] = { };
  dst = buf2;
  dst_size = sizeof buf2 - 1;


  res = jsoneat::to_json::cbuf::to_json_val(dst, dst_size, data);
  if (res >= dst_size) {
    fprintf(stderr, "JSON string buffer too small: %lu.\n"
        "Try again with bigger buffer.\n"
        "Unlike snprintf(3), we do not know how much bigger the buffer needs to be.\n", dst_size);
    return;
  }

  if (res > 0 && dst[res -1] == ',')
    dst[--res] = '\0'; // remove trailing comma

  puts(dst);

  if (0 == strcmp(buf, buf2))
    puts("JSON strings match");

}

int main() {
  example_data_class();
}
