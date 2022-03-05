#pragma once

#include "types.h"
#include <string>

#ifdef __GNUC__
#define COPYRIGHT_SYMBOL "\u00A9"
#define DEGREE_SYMBOL "\u00B0"
#define POWER2_SYMBOL "\u00B2"
#define POWER3_SYMBOL "\u00B3"
#else
#define COPYRIGHT_SYMBOL "\xA9"
#define DEGREE_SYMBOL "\xB0"
#define POWER2_SYMBOL "\xB2"
#define POWER3_SYMBOL "\xB3"
#endif