//	colors.c
//
//	Started:	November 16, 2001
//	Last Modified:	November 16, 2001

#include "types.h"
#include "colors.h"

void  RGBtoHLS(COLORREF lRGBColor, WORD *H, WORD *L, WORD *S)
{ 
   WORD R,G,B;          /* input RGB values */
   BYTE cMax,cMin;      /* max and min RGB values */
   WORD  Rdelta,Gdelta,Bdelta; /* intermediate value: % of spread from max
*/

   /* get R, G, and B out of DWORD */
   R = GetRValue(lRGBColor);
   G = GetGValue(lRGBColor);
   B = GetBValue(lRGBColor);

   /* calculate lightness */
   cMax = (BYTE)max( max(R,G), B);
   cMin = (BYTE)min( min(R,G), B);
   *L = (WORD)( (((cMax+cMin)*HLSMAX) + RGBMAX)/(2*RGBMAX) );

   if (cMax == cMin) {           /* r=g=b --> achromatic case */
      *S = 0;                     /* saturation */
      *H = UNDEFINED;             /* hue */
   }
   else {                        /* chromatic case */
      /* saturation */
      if (*L <= (HLSMAX/2))
         *S = (WORD) (( ((cMax-cMin)*HLSMAX) + ((cMax+cMin)/2) ) / (cMax+cMin) );
      else
         *S = (WORD)(  ( ((cMax-cMin)*HLSMAX) + ((2*RGBMAX-cMax-cMin)/2) )
            / (2*RGBMAX-cMax-cMin)  );

      /* hue */
      Rdelta = (WORD)(  ( ((cMax-R)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin)  );
      Gdelta = (WORD)(  ( ((cMax-G)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin)  );
      Bdelta = (WORD)(  ( ((cMax-B)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin)  );

      if (R == cMax)
         *H = (WORD)(Bdelta - Gdelta);
      else if (G == cMax)
         *H = (WORD)((HLSMAX/3) + Rdelta - Bdelta);
      else /* B == cMax */
         *H = (WORD)(((2*HLSMAX)/3) + Gdelta - Rdelta);

      if (((short)*H) < 0)
         *H += HLSMAX;
      if (*H > HLSMAX)
         *H -= HLSMAX;
   }
}

/* utility routine for HLStoRGB */

static WORD HueToRGB(WORD n1, WORD n2, WORD hue) { 

   /* range check: note values passed add/subtract thirds of range */
   if (((short)hue) < 0)
      hue += HLSMAX;

   if (hue > HLSMAX)
      hue -= HLSMAX;

   /* return r,g, or b value from this tridrant */
   if (hue < (HLSMAX/6))
      return (WORD)(  ( n1 + (((n2-n1)*hue+(HLSMAX/12))/(HLSMAX/6)) )  );
   if (hue < (HLSMAX/2))
      return (WORD)( n2 );
   if (hue < ((HLSMAX*2)/3))
      return (WORD)( n1 + (((n2-n1)*(((HLSMAX*2)/3)-hue)+(HLSMAX/12))/(HLSMAX/6))

); 
   else
      return ( n1 );

} 

COLORREF HLStoRGB(WORD hue, WORD lum, WORD sat) { 

   WORD R,G,B;                /* RGB component values */
   WORD  Magic1,Magic2;       /* calculated magic numbers (really!) */

   if (sat == 0) {            /* achromatic case */
      R=G=B=(WORD)((lum*RGBMAX)/HLSMAX);
      if (hue != UNDEFINED) {
         /* ERROR */
      }
   }
   else  {                    /* chromatic case */
      /* set up magic numbers */
      if (lum <= (HLSMAX/2))
         Magic2 = (WORD)(  (lum*(HLSMAX + sat) + (HLSMAX/2))/HLSMAX  );
      else
         Magic2 = (WORD)(  lum + sat - ((lum*sat) + (HLSMAX/2))/HLSMAX  );
      Magic1 = (WORD)(2*lum-Magic2);

      /* get RGB, change units from HLSMAX to RGBMAX */
      R = (WORD)(  (HueToRGB(Magic1,Magic2,(WORD)( hue+(HLSMAX/3) ))*RGBMAX +(HLSMAX/2))/HLSMAX  ); 
      G = (WORD)(  (HueToRGB(Magic1,Magic2,hue)*RGBMAX + (HLSMAX/2)) / HLSMAX  );
      B = (WORD)(  (HueToRGB(Magic1,Magic2,(WORD)( hue-(HLSMAX/3) ))*RGBMAX +(HLSMAX/2))/HLSMAX  ); 
   }

   return(RGB(R,G,B));

} 