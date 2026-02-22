#include "jsoneat.hh"

namespace jsoneat::from_json::jsmn {



/**
 * \brief         overloaded function template for de-serializing from json-string into char-array
 * \tparam ITER   JSMN iterator type
 * \tparam SIZE   char array size
 * \param it     JSMN iterator object reference
 * \param dst    data-member reference
 * \param key    json key to match against key name in iterator. or nullptr, for any key (?)
 * \return       true if key has matched and
 */
template<class ITER, size_t SIZE>
bool take(ITER &it, char (&dst)[SIZE], const char *key) {
  return it.takeValue(dst, key);
}

/**
 * \brief         overloaded function template for de-serializing integral types
 * \tparam ITER   JSMN iterator type
 * \tparam SIZE   char array size
 * \param it     JSMN iterator object reference
 * \param dst    data-member reference
 * \param key    json key to match against key name in iterator. or nullptr, for any key (?)
 * \return       true if key has matched and
 */
template<class ITER, typename INT, typename std::enable_if<std::is_integral<std::decay_t<INT>>::value>::type* = nullptr>
bool take(ITER &it, INT &dst, const char *key) {
  return it.takeValue(dst, key);
}

/**
 * \brief         overloaded function template for de-serializing floating point types
 * \tparam ITER   JSMN iterator type
 * \tparam SIZE   char array size
 * \param it     JSMN iterator object reference
 * \param dst    data-member reference
 * \param key    json key to match against key name in iterator. or nullptr, for any key (?)
 * \return       true if key has matched and
 */
template<class ITER, typename INT, typename std::enable_if<std::is_floating_point<std::decay_t<INT>>::value>::type* = nullptr>
bool take(ITER &it, INT &dst, const char *key) {
  return it.takeValue(dst, key);
}

/**
 * \brief         overloaded function template for de-serializing object types
 * \tparam ITER   JSMN iterator type
 * \tparam SIZE   char array size
 * \param it     JSMN iterator object reference
 * \param dst    data-member reference
 * \param key    json key to match against key name in iterator. or nullptr, for any key (?)
 * \return       true if key has matched and
 */
template<class ITER, class C, typename std::enable_if<std::is_class<std::decay_t<C>>::value>::type* = nullptr>
bool take(ITER &it, C &dst, const char *key) {
  return it.takeObject(dst, key);
}

/**
 * \brief         overloaded function template for de-serializing an array integral type values
 * \tparam ITER   JSMN iterator type
 * \tparam INT    type of array members
 * \tparam SIZE   array length
 * \param it     JSMN iterator object reference
 * \param dst    data-member reference
 * \param key    json key to match against key name in iterator. or nullptr, for any key (?)
 * \return       true if key has matched and
 */
template<class ITER, typename INT, size_t SIZE, typename std::enable_if<std::is_integral<std::decay_t<INT>>::value>::type* = nullptr>
bool take(ITER &it, INT (&dst)[SIZE], const char *key) {
  return it.takeValueArray(dst, key);
}

/**
 * \brief         overloaded function template for de-serializing an array floating point type values
 * \tparam ITER   JSMN iterator type
 * \tparam INT    type of array members
 * \tparam SIZE   array length
 * \param it     JSMN iterator object reference
 * \param dst    data-member reference
 * \param key    json key to match against key name in iterator. or nullptr, for any key (?)
 * \return       true if key has matched and
 */
template<class ITER, typename INT, size_t SIZE, typename std::enable_if<std::is_floating_point<std::decay_t<INT>>::value>::type* = nullptr>
bool take(ITER &it, INT (&dst)[SIZE], const char *key) {
  return it.takeValueArray(dst, key);
}

/**
 * \brief         overloaded function template for de-serializing an array of struct type objects
 * \tparam ITER   JSMN iterator type
 * \tparam INT    type of array members
 * \tparam SIZE   array length
 * \param it     JSMN iterator object reference
 * \param dst    data-member reference
 * \param key    json key to match against key name in iterator. or nullptr, for any key (?)
 * \return       true if key has matched and
 */
template<class ITER, class C, size_t SIZE, typename std::enable_if<std::is_class<std::decay_t<C>>::value>::type* = nullptr>
bool take(ITER &it, C (&dst)[SIZE], const char *key) {
  return it.takeObjectArray(dst, key);
}




/**
 * \brief          function template used as terminator in variadic template
 * \tparam ITER    type of iterator class embedded in JsoNeat
 * \param it       unused
 * \return         returns always false
 */
template<class ITER>
bool take_one_of(ITER &it) {
  return false;
}

/**
 * \brief         variadic function template for de-serializing data of current JSMN iterator into the matching pair from our args-list
 * \tparam ITER   type of iterator class embedded in JsoNeat
 * \tparam T      type of struct (pair) containing a reference to a data-member and the json-key name used for this member
 * \tparam Args   more types of these pairs like T but, which are only different by the type of the data-member referenced
 * \param it      iterator
 * \param pair    the first data-member-reference / json-key pair object
 * \param args    rest of the pair objects
 * \return        true if all key in JSMN object matched a data-member-reference in the pair objects
 */
template<class ITER, typename T, typename ... Args>
bool take_one_of(ITER &it, T pair, Args ... args) {
  if (jsoneat::from_json::jsmn::take(it, pair.val, pair.key))
    return true;
  return take_one_of(it, args...);
}

/**
 * \brief         variadic function template for de-serializing all members of an object in one call
 * \tparam ITER   type of iterator class embedded in JsoNeat
 * \tparam T      type of struct (pair) containing a reference to a data-member and the json-key name used for this member
 * \tparam Args   more types of these pairs like T but, which are only different by the type of the data-member referenced
 * \param it      iterator
 * \param pair    the first data-member-reference / json-key pair object
 * \param args    rest of the pair objects
 * \return        true if all key in JSMN object matched a data-member-reference in the pair objects
 */
template<class ITER, typename T, typename ... Args>
bool take_all_from_object(ITER &it, T pair, Args ... args) {
  assert(it->type == JSMN_OBJECT);

  auto count = it->size;
  for (++it; count > 0 && it; --count) {
    if (!take_one_of(it, pair, args...))
      return false; // fail for unknown keys
  }
  return true;
}





}
