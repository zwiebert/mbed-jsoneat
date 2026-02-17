#include "jsmn/jsmn.h"
#include "jsmn/jsmn_iterate.hh"

struct data {
  int a;
  bool b;
  unsigned c;
  char s[32];
};

///////////////////////////  Example for unnamed json object ///////////////////////////

static void example_unnamed_json_object();
static bool parse_and_process_unnamed_json_object(char *json_src, data &data_dst);
static bool parse_and_process_jsmn_for_unnamed_json_object(Jsmn_String::Iterator &it, data &data_dst);

static void example_unnamed_json_object() {
  char json_string[] = R"({"a":-1, "b":true, "c":3, "s":"hello unnamed"})";
  data json_data = { };
  if (parse_and_process_unnamed_json_object(json_string, json_data) || true) {
    printf("unamed json object: a=%d, b=%d, c=%d, s=%s\n", json_data.a, json_data.b, json_data.c, json_data.s);
  }
}

/**
 * \brief parses json string (json_src) and write data to an object (data_dst)
 * \param json_src     unnamed object "{...}" instead of "name:{...}")
 * \param data_dst     the struct should have the same member names and types
 * \return success
 */
static bool parse_and_process_unnamed_json_object(char *json_src, data &data_dst) {
  auto jsmn = JsoNeat<char*>(json_src, 128);
  if (!jsmn)
    return false;
  auto it = jsmn.begin();
  return parse_and_process_jsmn_for_unnamed_json_object(it, data_dst);
}

/**
 * \brief  copy data from jsmn object to a C struct object
 * \param it
 * \param data_dst  data is written here. Members should have same name and types as in jsmn object
 * \return success
 */
static bool parse_and_process_jsmn_for_unnamed_json_object(Jsmn_String::Iterator &it, data &data_dst) {
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

///////////////////////////  Example for named json object ///////////////////////////

static void example_named_json_object();
static bool parse_and_process_named_json_object(char *json_src, data &data_dst);
static bool parse_and_process_jsmn_for_named_json_object(Jsmn_String::Iterator &it, data &data_dst);

static void example_named_json_object() {
  char json_string[] = R"({"data": {"a":-1, "b":true, "c":3, "s":"hello named"}})";
  data json_data = { };
  if (parse_and_process_named_json_object(json_string, json_data) || true) {
    printf("named json object: a=%d, b=%d, c=%d, s=%s\n", json_data.a, json_data.b, json_data.c, json_data.s);
  }
}

/**
 * \brief parses json string (json_src) and write data to an object (data_dst)
 * \param json_src     named object "{...}" instead of "name:{...}")
 * \param data_dst     the struct should have the same member names and types
 * \return success
 */
static bool parse_and_process_named_json_object(char *json_src, data &data_dst) {
  auto jsmn = JsoNeat<char*>(json_src, 128);
  if (!jsmn)
    return false;
  auto it = jsmn.begin();
  ++it; // skip mandatory root object
  return parse_and_process_jsmn_for_named_json_object(it, data_dst);
}

/**
 * \brief  copy data from jsmn object to a C struct object
 * \param it
 * \param data_dst  data is written here. Members should have same name and types as in jsmn object
 * \return success
 */
static bool parse_and_process_jsmn_for_named_json_object(Jsmn_String::Iterator &it, data &data_dst) {
  int err;
  if (it.keyIsEqual("data", JSMN_OBJECT)) {
    auto count = it[1].size;
    for (it += 2; count > 0 && it; --count) {
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
  using self_type = data_class;

public:
  int a;
  bool b;
  unsigned c;
  char s[32];
  int ia[4];

public:

  /**
   * \brief       Public member template to de-serialize from a JSON string
   * \tparam T    Type of JSON string like char *, const char *, std::string, ...
   * \param json  JSON string
   * \return      success
   */
  template<typename T, typename std::enable_if<!std::is_class<T> { }, bool>::type = true>
  bool from_json(T json) {
    // tokenize JSON string using JSMN
    auto jsmn = Jsmn<32, T>(json);
    if (!jsmn)
      return false;

    // pass the rest of the work to an overloaded from_json() member
    auto it = jsmn.begin();
    return from_json(it);
  }

  /**
   * \brief        Public member template overload to de-serialize from JSMN tokens
   * \tparam jsmn_iterator  Type of iterator (depending on the type of token array/JSON string type)
   * \param it     Iterator of a JSMN_OBJECT token in a token array
   * \return       success
   */
  template<typename jsmn_iterator = Jsmn_String::Iterator, typename std::enable_if<std::is_class<jsmn_iterator> { }, bool>::type = true>
  bool from_json(jsmn_iterator &it) {
    assert(it->type == JSMN_OBJECT);

    auto count = it->size;
    for (++it; count > 0 && it; --count) {
      if (!(it.takeValue(a, "a") //
      || it.takeValue(b, "b") //
          || it.takeValue(c, "c") //
          || it.takeValue(s, "s") //
          || it.takeValueArray(ia, "ia") //
          // || it.skip_key_and_value() // skip unknown key/value pairs here or fail
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

  if (json_data.from_json(json_string)) {
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
  template<typename T, typename std::enable_if<!std::is_class<T> { }, bool>::type = true>
  bool from_json(T json) {
    auto jsmn = Jsmn<128, T>(json);

    if (!jsmn)
      return false;

    auto it = jsmn.begin();
    return from_json(it);
  }

  template<typename jsmn_iterator = Jsmn_String::Iterator, typename std::enable_if<std::is_class<jsmn_iterator> { }, bool>::type = true>
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
          // || it.skip_key_and_value() // skip unknown key/value pairs here or fail
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

  if (json_data.from_json(json_string) || true) {
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
  example_named_json_object();
  example_data_class();
  example_nested_data_class();

}
