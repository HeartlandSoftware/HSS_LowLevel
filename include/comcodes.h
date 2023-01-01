//	ComCodes.h
//
//	Started:	March 24, 2006
//	Last Modified:	March 24, 2006
//	By Robert W. Bryce

#ifndef __COMCODES_H
#define __COMCODES_H

#include "types.h"

extern "C" HRESULT ComError(int _errno_);			// translates a C error return code to a (we think reasonable) COM HRESULT

#endif
