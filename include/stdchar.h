/**
 * stdchar.h
 *
 * Copyright 2021-2023 Heartland Software Solutions Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the license at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the LIcense is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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