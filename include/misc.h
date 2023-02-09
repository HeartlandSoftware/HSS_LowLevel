/**
 * misc.h
 *
 * Copyright 2007-2023 Heartland Software Solutions Inc.
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

#ifndef __HSS_GENERIC_MISC_H
#define __HSS_GENERIC_MISC_H

#include "types.h"

#ifdef __cplusplus
extern "C" { 
#endif

int __cdecl isinteger(const TCHAR *string);
int __cdecl isfloat(const TCHAR *string);

	// just like _tcscpy_s, but skips any characters in _Strip during the copy
void __cdecl strcpy_strip_s(TCHAR * _Dst, unsigned long _SizeInBytes, const TCHAR * _Src, const TCHAR * _Strip);

// like strtok_s but instead will return empty tokens rather than skipping them.
TCHAR *  __cdecl strtok_se(TCHAR * _Str, _In_z_ const TCHAR * _Delim, TCHAR ** _Context);

#ifdef __cplusplus    
    }             
#endif

#endif
