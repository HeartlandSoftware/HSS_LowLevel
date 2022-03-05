//    MISC.H
//
//    Started:       May 3, 2007
//    Last Modified: April 8, 2013
//    Version 1.0
//    By Robert W. Bryce

#ifndef __GENERIC_MISC_H
#define __GENERIC_MISC_H

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
