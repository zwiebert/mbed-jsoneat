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

public:
  template<typename T, typename std::enable_if<!std::is_class<T> { }, bool>::type = true>
  bool from_json(T json) {
    auto jsmn = Jsmn<32, T>(json);

    if (!jsmn)
      return false;

    auto it = jsmn.begin();
    return from_json(it);
  }

  template<typename jsmn_iterator = Jsmn_String::Iterator, typename std::enable_if<std::is_class<jsmn_iterator> { }, bool>::type = true>
  bool from_json(jsmn_iterator &it) {
    assert(it->type == JSMN_OBJECT);

    using token_handler_fun_type = bool (*)(self_type &self, jsmn_iterator &it, int &err);
    static const token_handler_fun_type tok_processRootChilds_funs[] = { //
        [](self_type &self, jsmn_iterator &it, int &err) -> bool {
          if ((it.takeValue(self.a, "a") //
          || it.takeValue(self.b, "b") //
              || it.takeValue(self.c, "c") //
              || it.takeValue(self.s, "s") //
          )) {
            return true;
          }
          return false;
        },

        [](self_type &self, jsmn_iterator &it, int &err) -> bool { // Throw away unwanted objects
          return it.skip_key_and_value();
        } };

    int err = 0;
    auto count = it->size;
    for (++it; count > 0 && it; --count) {
      for (auto fun : tok_processRootChilds_funs) {
        if (fun(*this, it, err))
          break;
      }
    }
    return !err;

  }
};

static void example_data_class() {
  char json_string[] = R"({"a":-1, "b":true, "c":3, "s":"hello dc root"})";
  data_class json_data = { };

  if (json_data.from_json(json_string)) {
    printf("data_class object: a=%d, b=%d, c=%d, s=%s\n", json_data.a, json_data.b, json_data.c, json_data.s);
  }
}

//////////////////// Example: nested data class object ////////////////////////////////////////

class nested_data_class {
  using self_type = nested_data_class;

public:
  data_class da;
  data_class db;
  int a;
  bool b;
  unsigned c;
  char s[32];

public:
  template<typename T, typename std::enable_if<!std::is_class<T> { }, bool>::type = true>
  bool from_json(T json) {
    auto jsmn = Jsmn<32, T>(json);

    if (!jsmn)
      return false;

    auto it = jsmn.begin();
    return from_json(it);
  }

  template<typename jsmn_iterator = Jsmn_String::Iterator, typename std::enable_if<std::is_class<jsmn_iterator> { }, bool>::type = true>
  bool from_json(jsmn_iterator &it) {
    assert(it->type == JSMN_OBJECT);

    using token_handler_fun_type = bool (*)(self_type &self, jsmn_iterator &it, int &err);
    static const token_handler_fun_type tok_processRootChilds_funs[] = { //

        [](self_type &self, jsmn_iterator &it, int &err) -> bool {
          if (it.keyIsEqual("da", JSMN_OBJECT)) {
            it.skip_key();
            if (!self.da.from_json(it)) {
              ++err;
              it.skip_value();
            }
            return true;
          }
          return false;

        },

        [](self_type &self, jsmn_iterator &it, int &err) -> bool {
          if (it.keyIsEqual("db", JSMN_OBJECT)) {
            it.skip_key();
            if (!self.db.from_json(it)) {
              ++err;
              it.skip_value();
            }
            return true;
          }
          return false;

        },

        [](self_type &self, jsmn_iterator &it, int &err) -> bool {
          if ((it.takeValue(self.a, "a") //
          || it.takeValue(self.b, "b") //
              || it.takeValue(self.c, "c") //
              || it.takeValue(self.s, "s") //
          )) {
            return true;
          }
          return false;
        },

        [](self_type &self, jsmn_iterator &it, int &err) -> bool { // Throw away unwanted objects
          return it.skip_key_and_value();
        } };

    int err = 0;
    auto count = it->size;
    for (++it; count > 0 && it; --count) {
      for (auto fun : tok_processRootChilds_funs) {
        if (fun(*this, it, err))
          break;
      }
    }
    return !err;

  }
};

static void example_nested_data_class() {
  char json_string[] =
      R"({"da":{"a":-1, "b":true, "c":3, "s":"hello da"}, "db":{"a":-1, "b":true, "c":3, "s":"hello db"}, "a":-1, "b":true, "c":3, "s":"hello ndc rooot"})";

  nested_data_class json_data = { };

  if (json_data.from_json(json_string) || true) {
    printf("nested_data_class object:da.a=%d, da.b=%d, da.c=%d, da.s=%s\n", json_data.da.a, json_data.da.b, json_data.da.c, json_data.da.s);
    printf("nested_data_class object:db.a=%d, db.b=%d, db.c=%d, db.s=%s\n", json_data.db.a, json_data.db.b, json_data.db.c, json_data.db.s);
    printf("nested_data_class object: a=%d, b=%d, c=%d, s=%s\n", json_data.a, json_data.b, json_data.c, json_data.s);
  }
}

int main() {
  example_unnamed_json_object();
  example_named_json_object();
  example_data_class();
  example_nested_data_class();

}
