/**
 * \file     jsoneat/jsoneat.hh
 * \brief    helper class to iterate the JSMN token array and extract data
 * \author   Bert Winkelmann (github.com/zwiebert)
 */

#pragma once

#include "jsmn/jsmn.h"

#include <iterator>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <string>

/**
 * \brief  Parse JSON into tokens and iterate over it. Will handle all allocations.
 *
 * \tparam input_type        type of JSON input buffer. should be char* or const char*
 */
template<typename input_type = const char*>
class JsoNeat {
  using pointer = jsmntok_t *;

protected:
  JsoNeat(input_type json, jsmntok_t *tok, unsigned tok_max) :
      m_json(json), m_tok_heap_alloc(nullptr), m_tok(tok), m_tok_max(tok_max), m_nmb_tok(do_parse(json)) {
  }
public:

  JsoNeat(input_type json, unsigned tok_max) :
      m_json(json), m_tok_heap_alloc(new jsmntok_t[tok_max]), m_tok(m_tok_heap_alloc), m_tok_max(tok_max), m_nmb_tok(do_parse(json)) {
  }

  ~JsoNeat() {
    delete[] m_tok_heap_alloc;
  }

  operator bool() const {
    return m_json && m_nmb_tok > 0;
  }

  /**
   * \brief get json buffer passed to ctor
   *
   * \return pointer to json text buffer
   */
  input_type get_json() {
    return m_json;
  }

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = jsmntok_t;
    using pointer = value_type *;
    using reference = value_type &;
    using container_type = JsoNeat;

    Iterator(pointer ptr, container_type &container) :
        m_ptr(ptr), m_container(container) {
    }


  public:

    /**
     * \brief        Test type and character string of value for being equal
     * \param s      expected string
     * \param type   expected type
     * \return       true for match
     */
    bool value_st_equal(const char *s, jsmntype_t type) const {
      const unsigned slen = strlen(s);
      return m_ptr->type == type && slen == m_ptr->end - m_ptr->start // same length
      && strncmp(m_container.get_json() + m_ptr->start, s, slen) == 0; // same content

    }
    /**
     * \brief  test for value being null pointer
     * \return  true for match
     */
    bool value_equals_null() const {
      return value_st_equal("null", JSMN_PRIMITIVE);
    }

    /**
     * \brief  test for value being boolean false
     * \return  true for match
     */
    bool value_equals_false() const {
      return value_st_equal("false", JSMN_PRIMITIVE);
    }

    /**
     * \brief  test for value being boolean true
     * \return  true for match
     */
    bool value_equals_true() const {
      return value_st_equal("true", JSMN_PRIMITIVE);
    }

    /**
     * \brief get json text buffer
     */
    input_type get_json() {
      return m_container.get_json();
    }
    /**
     * \brief        test if key matches
     * \param  key   key to match
     * \return       false if key does not match or type is not JSMN_STRING
     */
    bool keyIsEqual(const char *key) const {
      return value_st_equal(key, JSMN_STRING); // JSON keys are strings
    }

    /**
     * \brief           test if both key and value-type matches
     * \param  key      string to match key or nullptr to match any key
     * \param  val_type type of value to match
     * \return          false for mismatch or key type not JSMN_STRING
     */
    bool keyIsEqual(const char *key, jsmntype_t val_type) const {
      if (val_type != m_ptr[1].type)
        return false;
      return !key || keyIsEqual(key);
    }

    /**
     * \brief        test if key matches
     * \param  key   key to match
     * \return       false if key does not match or type is not JSMN_STRING
     */
    bool keyStartsWith(const char *key) const {
      return m_ptr->type == JSMN_STRING // JSON keys are strings
      && strlen(key) < m_ptr->end - m_ptr->start // key should be smaller
      && strncmp(m_container.get_json() + m_ptr->start, key, strlen(key)) == 0; // same content
    }

    /**
     * \brief           test if both key and value-type matches
     * \param  key      string to match key or nullptr to match any key
     * \param  val_type type of value to match
     * \return          false for mismatch or key type not JSMN_STRING
     */
    bool keyStartsWith(const char *key, jsmntype_t val_type) const {
      if (val_type != m_ptr[1].type)
        return false;
      return !key || keyStartsWith(key);
    }

    /**
     * \brief        Get value
     * \param dst          value will be written to dst
     * \return       false if value type is not JSMN_PRIMITIVE Or JSMN_STRING
     */
    template<typename T>
    bool getValue(T &dst) const {
      return m_container.get_value(dst, m_ptr);
    }
    /**
     * \brief        Get value of key/value pair and advance iterator
     * \param  dst   value will be written to dst
     * \return       false if value type is not JSMN_PRIMITIVE Or JSMN_STRING
     */
    template<typename T>
    bool takeValue(T &dst) {
      if (!getValue(dst))
        return false;
      *this += 1;
      return true;
    }

    /**
     * \brief        Get value as string instead of number or boolean
     * \param dst    value will be written to dst
     * \return       false if value type is not JSMN_PRIMITIVE Or JSMN_STRING
     */
    template<size_t size>
    bool getValueAsString(char (&dst)[size]) const {
      return m_container.get_value_as_string(dst, size, m_ptr);
    }
    /**
     * \brief        Get value as string instead of number or boolean
     * \param dst    value will be written to dst
     * \param size   space available in dst
     * \return       false if value type is not JSMN_PRIMITIVE Or JSMN_STRING
     */
    bool getValueAsString(char *dst, size_t size) const {
      return m_container.get_value_as_string(dst, size, m_ptr);
    }

    /**
     * \brief        Get value as string null terminated string (in place)
     *
     *               This works only for non constant input_type, because it writes
     *               into the json text buffer to terminate strings.
     *
     * \return       pointer to null terminated string. If not JSMN_STRING or JSMN_PRIMITIVE it returns null
     */
    char* getValueAsString() const {
      return m_container.get_value_as_string(m_ptr);
    }

    /**
     * \brief        Get value of key/value pair if key
     * \param  key   key to match or nullptr to match any key
     * \param  dst   value will be written to dst
     * \return       false if key does not match, or value type is not JSMN_PRIMITIVE or JSMN_STRING
     */
    template<typename T>
    bool getValue(T &dst, const char *key) const {
      return keyIsEqual(key) && m_container.get_value(dst, m_ptr + 1);
    }
    /**
     * \brief        Get value of key/value pair and advance iterator
     * \param  key   key to match or nullptr to match any key
     * \param  dst   value will be written to dst
     * \return       false if key does not match, or value type is not JSMN_PRIMITIVE or JSMN_STRING
     */
    template<typename T>
    bool takeValue(T &dst, const char *key) {
      if (!getValue(dst, key))
        return false;
      *this += 2;
      return true;
    }

  public: // operators
    reference operator*() const {
      return *m_ptr;
    }
    pointer operator->() {
      return m_ptr;
    }

    Iterator& operator+=(int n) {
      m_ptr += n;
      return *this;
    }

    Iterator& operator-=(int n) {
      m_ptr -= n;
      return *this;
    }

    // Prefix increment
    Iterator& operator++() {
      m_ptr++;
      return *this;
    }

    // Postfix increment
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    reference operator[](int idx) {
      return m_ptr[idx];
    }

    operator bool() const {
      return m_ptr >= &m_container.m_tok[0] && &m_container.m_tok[0] + m_container.m_nmb_tok > m_ptr;
    }

    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.m_ptr == b.m_ptr;
    }

    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.m_ptr != b.m_ptr;
    }

    friend bool operator<(const Iterator &a, const Iterator &b) {
      return a.m_ptr < b.m_ptr;
    }

    friend bool operator>(const Iterator &a, const Iterator &b) {
      return a.m_ptr > b.m_ptr;
    }

  public:
    /**
     * \brief skip this value (of an array)
     *
     * \return success.
     */
    bool skip_value() {
      return m_container.skip_value(*this);
    }
    /**
     * \brief skip this key/value pair (in and object)
     *
     * \return success.
     */
    bool skip_key_and_value() {
      return m_container.skip_key_and_value(*this);
    }

  private:
    pointer m_ptr;
    container_type &m_container;
  };

  /**
   * \brief get JSON root object iterator
   *
   * \return iterator pointing to root object
   */
  Iterator begin() {
    return Iterator(&m_tok[0], *this);
  }
  /**
   * \brief get JSON root object end iterator
   *
   * \return iterator pointing one behind root object
   */
  Iterator end() {
    return Iterator(&m_tok[m_nmb_tok], *this);
  }

  /**
   * \brief skip value (of an array)
   *
   * \param it   iterator pointing to the value to skip
   * \return     success
   */
  static bool skip_value(Iterator &it) {
    if (!it)
      return false;

    if (it->type == JSMN_OBJECT) {
      auto count = it->size;
      for (++it; count > 0 && it; --count) {

        assert(it->type == JSMN_STRING);
        skip_key_and_value(it);
      }
      return true;
    }

    if (it->type == JSMN_ARRAY) {
      auto count = it->size;
      for (++it; count > 0 && it; --count) {
        skip_value(it);
      }
      return true;
    }

    ++it;
    return true;
  }

  /**
   * \brief      skip a key/value pair (in an object)
   *
   * \param it   iterator pointing to the key/value pair to skip
   * \return     success
   */
  static bool skip_key_and_value(Iterator &it) {
    ++it;
    if (!it)
      return false;
    return skip_value(it);
  }

private:
  int do_parse(const char *json) {
    if (!json)
      return false;

    jsmn_parser parser;

    jsmn_init(&parser);
    int r = jsmn_parse(&parser, json, strlen(json), m_tok, m_tok_max);
    if (r < 0) {
      return 0;
    }
    if (r < 1 || m_tok[0].type != JSMN_OBJECT) {
      return 0;
    }
    return r;
  }
  /**
   * \brief            copy string from a token to a buffer
   *
   * \param buf        destination buffer to copy to
   * \param buf_size   size of destination buffer
   * \param ptr        pointer to token
   * \return           pointer to buffer on success. null-pointer on failure
   */
  char* copy_string(char *buf, size_t buf_size, pointer ptr) const {
    size_t str_length = ptr->end - ptr->start;
    if (buf_size <= str_length)
      return NULL;
    memcpy(buf, m_json + ptr->start, str_length);
    buf[str_length] = '\0';
    return buf;
  }

  /**
   * \brief      try to read token value into a float reference
   *
   * \param dst  destination variable reference
   * \param ptr  pointer to token
   * \return     success
   */
  bool get_value(float &dst, pointer ptr) const {
    char buf[32];
    if (ptr->type == JSMN_PRIMITIVE && copy_string(buf, sizeof buf, ptr)) {
      dst = atof(buf);
      return true;
    }
    return false;
  }

  /**
   * \brief      try to read token value into a bool reference
   *
   * \param dst  destination variable reference
   * \param ptr  pointer to token
   * \return     success
   */
  bool get_value(bool &dst, pointer ptr) const {
    char buf[32];
    if (ptr->type == JSMN_PRIMITIVE && copy_string(buf, sizeof buf, ptr)) {
      if (0 ==strcmp(buf, "true") || 0==strcmp(buf, "1")){
        dst = true;
        return true;
      }
      if (0 ==strcmp(buf, "false") || 0==strcmp(buf, "0")){
        dst = false;
        return true;
      }
    }
    return false;
  }

  /**
   * \brief      try to read token value into a std::string reference
   *
   * \param dst  destination variable reference
   * \param ptr  pointer to token
   * \return     success
   */
  bool get_value(std::string &dst, pointer ptr) const {
    if (ptr->type == JSMN_STRING) {
      size_t str_length = ptr->end - ptr->start;
      dst = std::string(ptr->start, str_length);
      return true;
    }
    return false;
  }

  /**
   * \brief  try to read token value into an integer reference
   *
   *         the value is converted by standard atol() and casted
   *
   * \tparam T   type of integer
   * \param dst  destination reference
   * \param ptr  pointer to token
   * \return     success
   */
  template<typename T>
  bool get_value(T &dst, pointer ptr) const {
    char buf[32];
    if (ptr->type == JSMN_PRIMITIVE && copy_string(buf, sizeof buf, ptr)) {
      dst = static_cast<T>(atol(buf));
      return true;
    }
    return false;
  }

  /**
   * \brief       try to read token value into a char array
   *
   * \tparam size array size
   * \param dst   destination array
   * \param ptr  pointer to token
   * \return     success
   */
  template<size_t size>
  bool get_value(char (&dst)[size], pointer ptr) const {
    if (ptr->type == JSMN_STRING && copy_string(dst, size, ptr)) {
      return true;
    }
    return false;
  }

  /**
   * \brief       Copy token value string into buffer
   *
   * \param dst   destination buffer
   * \param size  destination buffer size
   * \param ptr   pointer to token
   * \return      success
   */
  bool get_value_as_string(char *dst, size_t size, pointer ptr) const {
    if ((ptr->type == JSMN_STRING || ptr->type == JSMN_PRIMITIVE) && copy_string(dst, size, ptr)) {
      return true;
    }
    return false;
  }

  /**
   * \brief         Terminate token value with a null and return the resulting string
   *
   *                This will modify the parsed JSON buffer
   *
   * \param ptr     pointer to token
   * \return        null terminated token value string
   */
  char* get_value_as_string(pointer ptr) {
    if (ptr->type != JSMN_STRING && ptr->type != JSMN_PRIMITIVE)
      return nullptr;

    m_json[ptr->end] = '\0';
    return m_json + ptr->start;
  }

private:
  input_type m_json; ///<  the JSON data we work on
  jsmntok_t *m_tok_heap_alloc; ///< token array pointer on heap used for realloc(), free() only. use \ref m_tok for anything else.
  jsmntok_t *m_tok;  ///< token array we work with
  unsigned m_tok_max; ///< how many elements \ref m_tok can hold
  unsigned m_nmb_tok; ///< the actual number of tokens Jsmn has parsed from /ref m_json

};

/**
 * \brief  Parse JSON into fixed size token array.
 *
 * \tparam JSON_MAX_TOKENS   number of elments in token array
 * \tparam input_type        type of JSON input buffer. should be char* or const char*
 */
template<size_t JSON_MAX_TOKENS, typename input_type = const char*>
class JsoNeat_fs: public JsoNeat<input_type> {
public:
  JsoNeat_fs(input_type json) :
      JsoNeat<input_type>(json, &m_tok_arr[0], JSON_MAX_TOKENS) {
  }
private:
  jsmntok_t m_tok_arr[JSON_MAX_TOKENS];  ///< fixed size token array, suitable for stack
};

using JsonNeat_cp = JsoNeat<char *>; ///<  working on non-const char JSON. this allows null terminating strings in place with \ref get_value_as_string
using JsonNeat_ccp = JsoNeat<const char *>; ///< working on const char JSON.
