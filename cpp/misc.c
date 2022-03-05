//    MISC.C
//
//    Started:       May 3, 2007
//    Last Modified: April 8, 2013
//    Version 1.0
//    By Robert W. Bryce

#include "misc.h"
#include <ctype.h>

#define nullptr 0					// oops - not part of the C standard, just C++

int isinteger(const TCHAR *string) {
	const TCHAR *s = string;
	while ((*s) && (isspace(*s)))
		s++;						// skip any leading whitespace
	if (!(*s))
		return 0;
	if (((*s) == '+') || ((*s) == '-'))
		s++;
	while ((*s) && (isdigit(*s)))				// go over the digits
		s++;
	while ((*s) && (isspace(*s)))				// skip any trailing whitespace
		s++;
	if (!(*s))
		return 1;					// we're at the end of the string now
	return 0;						// something other than digits and whitespace were encountered
}


int isfloat(const TCHAR *string) {
	const TCHAR *s = string;
	while ((*s) && (isspace(*s)))
		s++;						// skip any leading whitespace
	if (!(*s))
		return 0;
	if (((*s) == '+') || ((*s) == '-'))
		s++;
	while ((*s) && ((isdigit(*s) || ((*s) == '.'))))	// go over the digits
		s++;
	while ((*s) && (isspace(*s)))				// skip any trailing whitespace
		s++;
	if (!(*s))
		return 1;					// we're at the end of the string now.
	return 0;						// something other than digits and whitespace were encountered
}


void strcpy_strip_s(TCHAR * _Dst, unsigned long _SizeInBytes, const TCHAR * _Src, const TCHAR * _Strip) {
	if (_SizeInBytes) {
		unsigned long _in = 0;
		unsigned long _out = 0;
		const char *s;

		while ((_Src[_in]) && (_out < (_SizeInBytes - 1))) {
			s = _Strip;
			while (*s) {
				if (*s == _Src[_in]) {
					_in++;
					goto loop;
				}
				s++;
			}
			_Dst[_out++] = _Src[_in++];

		    loop:
			;
		}
		_Dst[_out] = '\0';
	}
}


TCHAR *  strtok_se(TCHAR * _String, const TCHAR * _Control, TCHAR ** _Context) {
    TCHAR *token, *retval;
    const TCHAR *ctl;

    /* validation section */
    if (!_Context)
	    return nullptr;
    if (!_Control)
	    return nullptr;
    if ((!_String) && (!*_Context))
	    return nullptr;

    /* If string==NULL, continue with previous string */
    if (!_String)
    {
        _String = *_Context;
    }

    /* Find beginning of token (skip over leading delimiters). Note that
    * there is no token iff this loop sets string to point to the terminal null. */
/*
    for ( ; *_String != 0 ; _String++)
    {
        for (ctl = _Control; *ctl != 0 && *ctl != *_String; ctl++)
            ;
        if (*ctl == 0)
        {
            break;
        }
    }
*/
    token = _String;

    if (!(*token))
	    retval = nullptr;
    else
	    retval = token;
    /* Find the end of the token. If it is not the end of the string,
    * put a null there. */
    for ( ; *_String != 0 ; _String++)
    {
        for (ctl = _Control; *ctl != 0 && *ctl != *_String; ctl++)
            ;
        if (*ctl != 0)
        {
            *_String++ = 0;
            break;
        }
    }

    /* Update the context */
    *_Context = _String;

    /* Determine if a token has been found. */
/*
    if (0 == *token)
    {
        return NULL;
    }
    else
    {
        return token;
    }
*/
    return retval;
}
