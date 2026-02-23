#include "from_json_jsmn.hh"
#include "to_json_cbuf.hh"


#define JSONEAT_SER_FROM_TO(...) \
  int to_json(char *dst, size_t dst_size) const { \
    return jsoneat::to_json::cbuf::to_json_args(dst, dst_size, __VA_ARGS__); \
  } \
  template<typename jsmn_iterator> \
  bool from_json(jsmn_iterator &it) { \
    return jsoneat::from_json::jsmn::deserialize_object(it, __VA_ARGS__); \
  }
