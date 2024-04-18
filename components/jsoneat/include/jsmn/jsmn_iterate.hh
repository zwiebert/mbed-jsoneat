/**
 * \file     jsmn_iterate.hh
 * \brief    helper class to iterate the JSMN token array and extract data
 * \author   Bert Winkelmann (github.com/zwiebert)
 */

#pragma once

#include "jsmn/jsmn.h"
#include <jsoneat/jsoneat.hh>

using Jsmn_String = JsoNeat<char *>; ///< deprecated
using Jsmn_Cstring = JsoNeat<const char *>; ///< deprecated
using JsmnBase = Jsmn_Cstring; ///< deprecated
#define Jsmn JsoNeat_fs
