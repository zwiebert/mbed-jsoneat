/**
 * \brief Serialize objects to JSON using char array buffers. For smaller objects which
 * fit into such a buffer.
 */

#pragma once

#include "jsoneat.hh"

namespace jsoneat::to_json::cbuf {

inline int to_json_val(char *dst, size_t dst_size, bool val) {
  return snprintf(dst, dst_size, //
      R"(%s,)", val ? "true" : "false");
}

inline int to_json_val(char *dst, size_t dst_size, int val) {
  return snprintf(dst, dst_size, //
      R"(%d,)", val);
}
inline int to_json_val(char *dst, size_t dst_size, unsigned val) {
  return snprintf(dst, dst_size, //
      R"(%u,)", val);
}

inline int to_json_val(char *dst, size_t dst_size, float val) {
  return snprintf(dst, dst_size, //
      R"(%f,)", val);
}
template<size_t SIZE>
int to_json_val(char *dst, size_t dst_size, const char (&val)[SIZE]) {
  return snprintf(dst, dst_size, //
      R"("%s",)", val);
}

template<class C, typename std::enable_if<std::is_class<std::decay_t<C>>::value>::type* = nullptr>
int to_json_val(char *dst, size_t dst_size, const C &val, bool append_comma = true) {
  int res = 0;

  if (res < dst_size)
    dst[res] = '{';
  ++res;

  if (res < dst_size)
    res += val.to_json(dst + res, dst_size - res);
  else
    res += val.to_json(dst, 0);

  //if (dst[res - 1] == ',')
  --res;

  if (res < dst_size)
    dst[res] = '}';
  ++res;

  if (append_comma) {
    if (res < dst_size)
      dst[res] = ',';
    ++res;
  }

  return res;

}

template<typename T, size_t SIZE>
int to_json_val(char *dst, size_t dst_size, const T (&val)[SIZE], bool append_comma = true) {
  int res = 0;

  if (res < dst_size)
    dst[res] = '[';
  ++res;

  for (int i = 0; i < SIZE; ++i) {
    if (res < dst_size)
      res += to_json_val(dst + res, dst_size - res, val[i]);
    else
      res += to_json_val(dst, 0, val[i]);

  }
  // if (dst[res - 1] == ',')
  --res;

  if (res < dst_size)
    dst[res] = ']';
  ++res;

  if (append_comma) {
    if (res < dst_size)
      dst[res] = ',';
    ++res;
  }

  return res;
}

template<class T>
int to_json_kvp(char *dst, size_t dst_size, const jsoneat::KvPair<T> kvp) {
  auto res = snprintf(dst, dst_size, R"("%s":)", kvp.key);
  if (res < dst_size)
    return res + to_json_val(dst + res, dst_size - res, kvp.val);
  else
    return res + to_json_val(dst, 0, kvp.val);
}

template<size_t SIZE>
int to_json_kvp(char *dst, size_t dst_size, const jsoneat::KvPair<char[SIZE]> kvp) {
  return snprintf(dst, dst_size, //
      R"("%s":"%s",)", kvp.key, kvp.val);
}

int to_json_args(char *dst, size_t dst_size) {
  return 0;
}

template<typename T, typename ... Args>
int to_json_args(char *dst, size_t dst_size, const T &pair, Args ... args) {
  auto res = to_json_kvp(dst, dst_size, pair);

  if (res < dst_size)
    return res + to_json_args(dst + res, dst_size - res, args...);
  else
    return res + to_json_args(dst, 0, args...);
}

}
