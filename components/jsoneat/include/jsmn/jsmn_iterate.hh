/**
 * \file     jsmn_iterate.hh
 * \brief    deprecated header for backward compatibility
 */

#pragma once

#include "jsmn/jsmn.h"
#include <jsoneat/jsoneat.hh>

namespace jsoneat {
using Jsmn_String = JsoNeat<char *>; ///< deprecated
using Jsmn_Cstring = JsoNeat<const char *>; ///< deprecated
using JsmnBase = Jsmn_Cstring; ///< deprecated
#define Jsmn jsoneat::JsoNeat_fs
}
