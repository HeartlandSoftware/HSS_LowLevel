//	colors.h
//
//	Started:	November 16, 2001
//	Last Modified:	November 16, 2001

#ifndef __COLORS_H
#define __COLORS_H

#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif

#include <windows.h>
#else
#include "types.h"
typedef uint32_t COLORREF;
#endif

// Code blatently taken from MSDN, Article ID: Q29240 

#define  HLSMAX   255   /* H,L, and S vary over 0-HLSMAX */
#define  RGBMAX   255   /* R,G, and B vary over 0-RGBMAX */
                        /* HLSMAX BEST IF DIVISIBLE BY 6 */
                        /* RGBMAX, HLSMAX must each fit in a byte. */

/* Hue is undefined if Saturation is 0 (grey-scale) */
/* This value determines where the Hue scrollbar is */
/* initially set for achromatic colors */
#define UNDEFINED (HLSMAX*2/3)

#ifdef __cplusplus
extern "C" { 
#endif

COLORREF HLStoRGB(WORD hue, WORD lum, WORD sat);
void RGBtoHLS(COLORREF lRGBColor, WORD *hue, WORD *lum, WORD *sat);

#ifdef __cplusplus    
    }             
#endif

#ifdef _MSC_VER
#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif
#endif

#endif
