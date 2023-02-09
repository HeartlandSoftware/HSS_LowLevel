/**
 * tstring.h
 *
 * Copyright 2013-2023 Heartland Software Solutions Inc.
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

#include <string>
#include <sstream>
#include "types.h"

#ifdef _STRING_
#ifdef UNICODE
#ifndef _TSTRING_DEFINED
typedef std::wstring tstring;
typedef std::wistringstream tistringstream;
#define _TSTRING_DEFINED
#endif
#else
#ifndef _TSTRING_DEFINED
typedef std::string tstring;
typedef std::istringstream tistringstream;
#define _TSTRING_DEFINED
#endif
#endif
#elif !defined _MSC_VER
#ifndef _TSTRING_DEFINED
typedef std::string tstring;
typedef std::istringstream tistringstream;
#define _TSTRING_DEFINED
#endif
#endif


std::string __cdecl toStdString(const tstring &toConvert);
std::string __cdecl toStdString(const TCHAR *toConvert);
tstring __cdecl toTString(const std::string &toConvert);
tstring __cdecl toTString(const char *toConvert);

/* don't do this 'cause you may have a pointer to contents of an immediately destroyed temporary
#ifdef UNICODE
#define ToANSI(toConvert)	toStdString(toConvert).c_str()
#else
#define ToANSI(toConvert)	toConvert
#endif
*/
