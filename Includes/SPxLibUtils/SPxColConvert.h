/*********************************************************************
*
* (c) Copyright 2011 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxColConvert class - a class that
*	contains conversion functions between colour spaces.
*
* Revision Control:
*   16/11/20 v1.19   AGC	Add UYVYtoRGB24/32 functions.
*
* Previous Changes:
*   28/08/20 1.18   AGC	Move critical section to source.
*   11/06/20 1.17   AGC	Add I420toRGB32 function.
*   21/05/20 1.16   AGC	Add alpha argument to NV12toRGB32().
*   08/01/20 1.15   AGC	Correct more behaviour across SPxDeleteAll().
*   21/02/19 1.14   AGC	Add RGB24toNV12 function.
*			Add RGB24toYV12 function.
*			Add RGB32toNV12 function.
*   20/02/19 1.13   AGC	Add NV12toRGB32 function.
*			Add YV12toRGB32 function.
*   05/02/15 1.12   AGC	Add YUVtoRGB32 function.
*   10/04/14 1.11   AGC	Correct behaviour across SPxDeleteAll().
*   05/03/14 1.10   AGC	Add RGB32toYV12 function.
*			Add AlphaToYV12 function.
*   18/11/13 1.9    AGC	Fix clang warning.
*   04/10/13 1.8    AGC	Simplify headers.
*   15/07/13 1.7    AGC	Add YtoRGB32 function.
*   18/01/12 1.6    AGC	Remove yuvToG table to avoid cache misses.
*   13/07/11 1.5    AGC	Rename function names to match format names.
*			Add RGB32toRGB24 function.
*   07/07/11 1.4    AGC	Move to SPx library and rename to SPxColConvert.
*   29/06/11 1.3    AGC	Use int instead of bool.
*   27/06/11 1.2    AGC	Replace conversion process with static
*			functions.
*   09/06/11 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_COL_CONVERT_H
#define _SPX_COL_CONVERT_H

/*
 * Other headers required.
 */

/* For UINT8 types etc . */
#include "SPxLibUtils/SPxTypes.h"

/* For SPxLUT8/16. */
#include "SPxLibData/SPxLUT.h"

/* For SPxObjPtr. */
#include "SPxLibUtils/SPxObjPtr.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * Define our class.
 */
class SPxColConvert
{
public:
    /* Public functions. */
    static void AllocateLookupTables(void);
    static void FreeLookupTables(void);

    /* Conversion functions. */
    static void YUY2toRGB24(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void YUY2toRGB32(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void UYVYtoRGB24(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void UYVYtoRGB32(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void NV12toRGB32(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels,
			    UINT8 alpha=0);

    static void YV12toRGB32(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void I420toRGB32(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void YUVtoRGB32(const UINT8 *srcPtr, long srcStride,
			   UINT8 *destPtr, long destStride,
			   int widthInPixels, int heightInPixels);

    static void YtoRGB32(const UINT8* src, long srcStride, 
			 UINT8* dest, long destStride,
			 int widthInPixels, int heightInPixels);

    static void RGB24toRGB32(const UINT8 *srcPtr, long srcStride,
			     UINT8 *destPtr, long destStride,
			     int widthInPixels, int heightInPixels);

    static void RGB24toNV12(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void RGB24toYV12(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void RGB32toRGB24(const UINT8 *srcPtr, long srcStride,
			     UINT8 *destPtr, long destStride,
			     int widthInPixels, int heightInPixels);

    static void RGB32toNV12(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void RGB32toYV12(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels);

    static void AlphaToYV12(const UINT8 *srcPtr, long srcStride,
			    UINT8 *destPtr, long destStride,
			    int widthInPixels, int heightInPixels,
			    UINT8 r, UINT8 g, UINT8 b);

private:
    /* Private variables. */
    static SPxObjPtr<SPxLUT16> m_yvToR;
    static SPxObjPtr<SPxLUT16> m_yuToB;
    static SPxObjPtr<SPxLUT8> m_alphaToY;
    static SPxObjPtr<SPxLUT8> m_alphaToU;
    static SPxObjPtr<SPxLUT8> m_alphaToV;
    static UINT8 m_lastR;
    static UINT8 m_lastG;
    static UINT8 m_lastB;

    /* Private constructor to prevent instantiation. */
    SPxColConvert(void) {}
    static inline void YUY2toRGB(const UINT8* src, long srcStride, 
				 UINT8* dest, long destStride,
				 int widthInPixels, int heightInPixels,
				 int alpha);
    static inline void UYVYtoRGB(const UINT8* src, long srcStride,
				 UINT8* dest, long destStride,
				 int widthInPixels, int heightInPixels,
				 int alpha);
    static inline UINT8 yuvToG(UINT8 y, UINT8 u, UINT8 v);
    static inline UINT8 clip(int clr);
    static inline UINT8 rgbToY(UINT8 r, UINT8 g, UINT8 b);
    static inline UINT8 rgbToU(UINT8 r, UINT8 g, UINT8 b);
    static inline UINT8 rgbToV(UINT8 r, UINT8 g, UINT8 b);

}; /* SPxColConvert */

#endif /* _SPX_COL_CONVERT_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
