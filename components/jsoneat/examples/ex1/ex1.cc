#include "jsmn/jsmn.h"
#include "jsmn/jsmn_iterate.hh"

#include <type_traits>



///////////////////////////  Example for unnamed json object ///////////////////////////

// data struct without a from_json() member. This will make it impossible to have
// it in a JSON array and using it.takeObjectArray(), because that function needs to call
// the from_json() member for each array element.
// see examples below for how it should be done
struct data {
  int a;
  bool b;
  unsigned c;
  char s[32];
};


static void example_unnamed_json_object();
static bool from_json(char *json_src, data &data_dst);
static bool from_json(Jsmn_String::Iterator &it, data &data_dst);

static void example_unnamed_json_object() {
  char json_string[] = R"({"a":-1, "b":true, "c":3, "s":"hello unnamed"})";
  data json_data = { };
  if (from_json(json_string, json_data) || true) {
    printf("unamed json object: a=%d, b=%d, c=%d, s=%s\n", json_data.a, json_data.b, json_data.c, json_data.s);
  }
}

/**
 * \brief parses json string (json_src) and write data to an object (data_dst)
 * \param json_src     unnamed object "{...}" instead of "name:{...}")
 * \param data_dst     the struct should have the same member names and types
 * \return success
 */
static bool from_json(char *json_src, data &data_dst) {
  auto jsmn = JsoNeat<char*>(json_src, 128);
  if (!jsmn)
    return false;
  auto it = jsmn.begin();
  return from_json(it, data_dst);
}

/**
 * \brief  copy data from jsmn object to a C struct object
 * \param it
 * \param data_dst  data is written here. Members should have same name and types as in jsmn object
 * \return success
 */
static bool from_json(Jsmn_String::Iterator &it, data &data_dst) {
  int err;
  if (it->type == JSMN_OBJECT) {
    auto count = it->size;
    for (it += 1; count > 0 && it; --count) {
      if (!(it.takeValue(data_dst.a, "a") //
      || it.takeValue(data_dst.b, "b") //
          || it.takeValue(data_dst.c, "c") //
          || it.takeValue(data_dst.s, "s") //
      )) {
        ++err;
        it.skip_key_and_value();
      }
    }
  } else
    return false;

  return err == 0;

}

//////////////////// Example: data class object ////////////////////////////////////////

class data_class {
public:
  int a;
  bool b;
  unsigned c;
  char s[32];
  int ia[4];

public:


  /**
   * \brief        Public member template overload to de-serialize from JSMN tokens
   * \tparam jsmn_iterator  Type of iterator (depending on the type of token array/JSON string type)
   * \param it     Iterator of a JSMN_OBJECT token in a token array
   * \return       success
   */
  template<typename jsmn_iterator>
  bool from_json(jsmn_iterator &it) {
    assert(it->type == JSMN_OBJECT);

    auto count = it->size;
    for (++it; count > 0 && it; --count) {
      if (!(it.takeValue(a, "a") //
      || it.takeValue(b, "b") //
          || it.takeValue(c, "c") //
          || it.takeValue(s, "s") //
          || it.takeValueArray(ia, "ia") //
      ))
        return false; // fail for unknown keys
    }
    return true;
  }
};

static void example_data_class() {
  // serialized object data in JSON format
  char json_string[] = R"({"a":-1, "b":true, "c":3, "s":"hello dc root", "ia":[1, 2, 3, 4]})";

  // Object with a from_json() member template function
  data_class json_data = { };

  if (from_json(json_data, json_string)) {
    printf("data_class object: a=%d, b=%d, c=%d, s=%s, ia=[%d, %d, %d, %d]\n", json_data.a, json_data.b, json_data.c, json_data.s,
        json_data.ia[0], json_data.ia[1],json_data.ia[2],json_data.ia[3]);
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
  bool from_json(jsmn_iterator &it) {
    assert(it->type == JSMN_OBJECT);

    auto count = it->size;
    for (++it; count > 0 && it; --count) {
      if (!(it.takeValue(a, "a") //
      || it.takeValue(b, "b") //
          || it.takeValue(c, "c") //
          || it.takeValue(s, "s") //
          || it.takeObject(da, "da") //
          || it.takeObject(db, "db") //
          || it.takeObjectArray(darr, "darr") //
      ))
        return false; // fail for unknown keys
    }
    return true;

  }
};

static void example_nested_data_class() {
  char json_string[] =
      R"({"da":{"a":-1, "b":true, "c":3, "s":"hello da"}, "db":{"a":-1, "b":true, "c":3, "s":"hello db"}, "darr":[{"a":-1, "b":true, "c":3, "s":"hello darr[0]"},{"a":-1, "b":true, "c":3, "s":"hello darr[1]"},{"a":-1, "b":true, "c":3, "s":"hello darr[2]"},{"a":-1, "b":true, "c":3, "s":"hello darr[3]"}], "a":-1, "b":true, "c":3, "s":"hello ndc rooot"})";

  nested_data_class json_data = { };

  if (from_json<128>(json_data, json_string)) {
    printf("nested_data_class object:da.a=%d, da.b=%d, da.c=%d, da.s=%s\n", json_data.da.a, json_data.da.b, json_data.da.c, json_data.da.s);
    printf("nested_data_class object:db.a=%d, db.b=%d, db.c=%d, db.s=%s\n", json_data.db.a, json_data.db.b, json_data.db.c, json_data.db.s);
    printf("nested_data_class object:darr[0].a=%d, darr[0].b=%d, darr[0].c=%d, darr[0].s=%s\n", json_data.darr[0].a, json_data.darr[0].b, json_data.darr[0].c, json_data.darr[0].s);
    printf("nested_data_class object:darr[1].a=%d, darr[1].b=%d, darr[1].c=%d, darr[1].s=%s\n", json_data.darr[1].a, json_data.darr[1].b, json_data.darr[1].c, json_data.darr[1].s);
    printf("nested_data_class object:darr[2].a=%d, darr[2].b=%d, darr[2].c=%d, darr[2].s=%s\n", json_data.darr[2].a, json_data.darr[2].b, json_data.darr[2].c, json_data.darr[2].s);
    printf("nested_data_class object:darr[3].a=%d, darr[3].b=%d, darr[3].c=%d, darr[3].s=%s\n", json_data.darr[3].a, json_data.darr[3].b, json_data.darr[3].c, json_data.darr[3].s);
    printf("nested_data_class object: a=%d, b=%d, c=%d, s=%s\n", json_data.a, json_data.b, json_data.c, json_data.s);
  }
}

int main() {
  example_unnamed_json_object();
  example_data_class();
  example_nested_data_class();

}
