/*	TSTRING.H
//
//	Started:	January 27, 2013
//	Last Modified:	January 27, 2013
//	Version 1.0
//
//	Purpose: to simply extract unicode away from std::string vs. std::wstring
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
