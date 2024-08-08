#pragma once

#include <cstdint>

// C/C++ type names are annoying, here are some more concise ones

using uint8 = unsigned char;
using int8 = signed char;
using byte = uint8;
using sbyte = int8;

using uint16 = unsigned short;
using int16 = signed short;
using ushort = uint16;

using uint32 = unsigned int;
using int32 = signed int;
using uint = uint32;

using uint64 = unsigned long long;
using int64 = signed long long;
using ulong = uint64;
// rip using long = int64;

using nint = intptr_t;
using nuint = uintptr_t;
