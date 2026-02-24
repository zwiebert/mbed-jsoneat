#include "from_json_jsmn.hh"
#include "to_json_cbuf.hh"


#define JSONEAT_SER_FROM_TO(...) \
  int _to_json(char *dst, size_t dst_size) const { \
    return jsoneat::to_json::cbuf::to_json_args(dst, dst_size, __VA_ARGS__); \
  } \
  int to_json(char *dst, size_t dst_size) const { \
    return jsoneat::to_json::cbuf::to_json_val(dst, dst_size, *this); \
  } \
  template<typename jsmn_iterator> \
  bool _from_json(jsmn_iterator &it) { \
    return jsoneat::from_json::jsmn::deserialize_object(it, __VA_ARGS__); \
  } \
  bool from_json(const char *json) { \
    return jsoneat::from_json_member(*this, json); \
  }
