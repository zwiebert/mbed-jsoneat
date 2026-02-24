/**
 * \brief Example programm. De-serialize objects
 *
 * For struct/class types he user should add from_json() member functions.
 * The JSON string and the root object are then passed to a library function jsoneat::from_json_member
 */

#include "jsmn/jsmn.h"
#include "jsmn/jsmn_iterate.hh"
#include "jsoneat/from_json_jsmn.hh"
#include <type_traits>

//////////////////// Example: data class object ////////////////////////////////////////

class data_class {
public:
  // some data members
  int a;
  bool b;
  unsigned c;
  float f;
  char s[32];
  int ia[4];

public:
  /**
   * \brief        Public member template overload to de-serialize object from JSMN tokens
   * \tparam jsmn_iterator  Type of iterator (depending on the type of token array/JSON string type)
   * \param it     Iterator of a JSMN_OBJECT token in a JSMN token array
   * \return       success
   */
  template<typename jsmn_iterator>
  bool _from_json(jsmn_iterator &it) {
    return jsoneat::from_json::jsmn::deserialize_object(it, JSONEAT_KvPairs(a, b, c, f, s, ia));
  }
};

static void example_data_class() {
  // serialized object data in JSON format
  char json_string[] = R"({"a":-1, "b":true, "c":3, "f":1.234, "s":"hello dc root", "ia":[1, 2, 3, 4]})";

  // Object with a from_json() member template function
  data_class json_data = { };

  if (jsoneat::from_json_member(json_data, json_string)) {
    printf("data_class object: a=%d, b=%d, c=%d, f=%f,  s=%s, ia=[%d, %d, %d, %d]\n", json_data.a, json_data.b, json_data.c, json_data.f, json_data.s,
        json_data.ia[0], json_data.ia[1], json_data.ia[2], json_data.ia[3]);
  }
}

//////////////////// Example: nested data class object ////////////////////////////////////////

class nested_data_class {
  using self_type = nested_data_class;

public:
  data_class da;
  data_class db;
  data_class darr[4];
  int a;
  bool b;
  unsigned c;
  char s[32];

public:

  template<typename jsmn_iterator>
  bool _from_json(jsmn_iterator &it) {
    return jsoneat::from_json::jsmn::deserialize_object(it, JSONEAT_KvPairs(a, b, c, s, da, db, darr));
  }
};

static void example_nested_data_class() {
  char json_string[] =
      R"({"da":{"a":-1, "b":true, "c":3, "s":"hello da"}, "db":{"a":-1, "b":true, "c":3, "s":"hello db"}, "darr":[{"a":-1, "b":true, "c":3, "s":"hello darr[0]"},{"a":-1, "b":true, "c":3, "s":"hello darr[1]"},{"a":-1, "b":true, "c":3, "s":"hello darr[2]"},{"a":-1, "b":true, "c":3, "s":"hello darr[3]"}], "a":-1, "b":true, "c":3, "s":"hello ndc rooot"})";

  nested_data_class json_data = { };

  if (jsoneat::from_json_member<128>(json_data, json_string)) {
    printf("nested_data_class object:da.a=%d, da.b=%d, da.c=%d, da.s=%s\n", json_data.da.a, json_data.da.b, json_data.da.c, json_data.da.s);
    printf("nested_data_class object:db.a=%d, db.b=%d, db.c=%d, db.s=%s\n", json_data.db.a, json_data.db.b, json_data.db.c, json_data.db.s);
    printf("nested_data_class object:darr[0].a=%d, darr[0].b=%d, darr[0].c=%d, darr[0].s=%s\n", json_data.darr[0].a, json_data.darr[0].b, json_data.darr[0].c,
        json_data.darr[0].s);
    printf("nested_data_class object:darr[1].a=%d, darr[1].b=%d, darr[1].c=%d, darr[1].s=%s\n", json_data.darr[1].a, json_data.darr[1].b, json_data.darr[1].c,
        json_data.darr[1].s);
    printf("nested_data_class object:darr[2].a=%d, darr[2].b=%d, darr[2].c=%d, darr[2].s=%s\n", json_data.darr[2].a, json_data.darr[2].b, json_data.darr[2].c,
        json_data.darr[2].s);
    printf("nested_data_class object:darr[3].a=%d, darr[3].b=%d, darr[3].c=%d, darr[3].s=%s\n", json_data.darr[3].a, json_data.darr[3].b, json_data.darr[3].c,
        json_data.darr[3].s);
    printf("nested_data_class object: a=%d, b=%d, c=%d, s=%s\n", json_data.a, json_data.b, json_data.c, json_data.s);
  }
}

///////////////////////////  Example for unnamed json object ///////////////////////////

// This example shows how to have the from_json() function outside the data struct or class.
// this does not work together with things like it.takeObjectArray()

struct data {
  int a;
  bool b;
  unsigned c;
  char s[32];
};

/**
 * \brief  copy data from jsmn object to a C struct object
 * \param it
 * \param data_dst  data is written here. Members should have same name and types as in jsmn object
 * \return success
 */
static bool _from_json(jsoneat::Jsmn_String::Iterator &it, data &dst) {
  return jsoneat::from_json::jsmn::deserialize_object(it, jsoneat::KvPair("a", dst.a), jsoneat::KvPair("b", dst.b), jsoneat::KvPair("c", dst.c),
      jsoneat::KvPair("s", dst.s));
}

/**
 * \brief parses json string (json_src) and write data to an object (data_dst)
 * \param json_src     unnamed object "{...}" instead of "name:{...}")
 * \param data_dst     the struct should have the same member names and types
 * \return success
 */
static bool from_json_friend(data &data_dst, char *json_src) {

#if 0
 // allocate JSMN token array with 128 elements on heap
  auto jsmn = jsoneat::JsoNeat<char*>(json_src, 128);
 #else
  // allocate JSMN token array with 32 elements on stack
  auto jsmn = jsoneat::JsoNeat_fs<32, char*>(json_src);
#endif

  if (!jsmn)
    return false; // token-array was too small, or JSON was invalid, or JSON not matching the object

  auto it = jsmn.begin(); // get an iterator pointing to first token

  // call our from_json() function.
  // if that function was instead a member function of the data object
  // we would not need this function here, but could instead call
  // the template function jsoneat::from_json(T &obj, const char *json)
  return _from_json(it, data_dst);
}

static void example_unnamed_json_object() {
  char json_string[] = R"({"a":-1, "b":true, "c":3, "s":"hello unnamed"})";
  data json_data = { };

  if (from_json_friend(json_data, json_string)) {
    printf("unamed json object: a=%d, b=%d, c=%d, s=%s\n", json_data.a, json_data.b, json_data.c, json_data.s);
  }
}

int main() {
  example_unnamed_json_object();
  example_data_class();
  example_nested_data_class();

}
