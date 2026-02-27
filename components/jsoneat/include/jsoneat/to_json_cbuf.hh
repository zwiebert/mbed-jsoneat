/**
 * \brief Serialize objects to JSON using char array buffers. For smaller objects which
 * fit into such a buffer.
 */

#pragma once

#include "jsoneat.hh"

namespace jsoneat::to_json::cbuf {

/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
inline int to_json_val(char *dst, size_t dst_size, bool val) {
  return snprintf(dst, dst_size, //
      R"(%s,)", val ? "true" : "false");
}

/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
inline int to_json_val(char *dst, size_t dst_size, int val) {
  return snprintf(dst, dst_size, //
      R"(%d,)", val);
}
/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
inline int to_json_val(char *dst, size_t dst_size, long val) {
  return snprintf(dst, dst_size, //
      R"(%ld,)", val);
}
/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
inline int to_json_val(char *dst, size_t dst_size, long long val) {
  return snprintf(dst, dst_size, //
      R"(%lld,)", val);
}
/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
inline int to_json_val(char *dst, size_t dst_size, unsigned val) {
  return snprintf(dst, dst_size, //
      R"(%u,)", val);
}

/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
inline int to_json_val(char *dst, size_t dst_size, float val) {
  return snprintf(dst, dst_size, //
      R"(%f,)", val);
}
/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
template<size_t SIZE>
int to_json_val(char *dst, size_t dst_size, const char (&val)[SIZE]) {
  return snprintf(dst, dst_size, //
      R"("%s",)", val);
}

/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value, which is an object
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
template<class C, typename std::enable_if<std::is_class_v<C>>::type* = nullptr>
int to_json_val(char *dst, size_t dst_size, const C &val, bool append_comma = true) {
  int res = 0;

  if (res < dst_size)
    dst[res] = '{';
  ++res;

  if (res < dst_size)
    res += val._to_json(dst + res, dst_size - res);
  else
    res += val._to_json(dst, 0);

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

  if (res < dst_size)
    dst[res] = '\0';

  return res;

}

/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value, which is a std::array
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
template<typename T, size_t SIZE>
int to_json_val(char *dst, size_t dst_size, const std::array<T, SIZE> &val, bool append_comma = true) {
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

  if (res < dst_size)
    dst[res] = '\0';

  return res;
}

/**
 * \brief          Print value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param val      input value, which is a C style array
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
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

  if (res < dst_size)
    dst[res] = '\0';

  return res;
}

/**
 * \brief          Print key and value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param kvp      input key/value pair
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
template<class T>
int to_json_kvp(char *dst, size_t dst_size, const jsoneat::KvPair<T> kvp) {
  auto res = snprintf(dst, dst_size, R"("%s":)", kvp.key);
  if (res < dst_size)
    return res + to_json_val(dst + res, dst_size - res, kvp.val);
  else
    return res + to_json_val(dst, 0, kvp.val);
}

/**
 * \brief          Print key and value of a JSON key/value pair and append trailing comma.
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param kvp      input key/value pair. Value is C style char array.
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
template<size_t SIZE>
int to_json_kvp(char *dst, size_t dst_size, const jsoneat::KvPair<char[SIZE]> kvp) {
  return snprintf(dst, dst_size, //
      R"("%s":"%s",)", kvp.key, kvp.val);
}

namespace {
/**
 * \brief          This overloaded function is called last in recursion
 * \return         Always 0
 */
inline int to_json_args(char*, size_t) {
  return 0;
}
} // namespace

/**
 * \brief
 * \tparam T       Prints an argument-list of key/value pairs as JSON
 * \tparam Args    list of types which should each be an instance of struct template jsoneat::KvPair
 * \param dst      output buffer
 * \param dst_size output buffer size
 * \param pair     First key/value pair in argument list
 * \param args     Rest of argument list
 * \return         Like snprintf(3), the number of characters needed. If smaller than dst_size, the output was fully written
 */
template<typename T, typename ... Args>
int to_json_args(char *dst, size_t dst_size, const T &pair, Args ... args) {
  auto res = to_json_kvp(dst, dst_size, pair);

  if (res < dst_size)
    return res + to_json_args(dst + res, dst_size - res, args...);
  else
    return res + to_json_args(dst, 0, args...);
}

}
