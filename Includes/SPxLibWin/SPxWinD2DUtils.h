/*********************************************************************
 *
 * (c) Copyright 2014, 2015, 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Windows Direct2D utility functions.
 *
 * Revision Control:
 *  24/08/22 v1.3    AGC	Add SPxWinD3D11CreateDevice().
 *
 * Previous Changes:
 *  14/04/15 1.2    AGC	Support retrieving reason for no support.
 *  11/12/14 1.1    AGC	Initial version.
 **********************************************************************/
#ifndef _SPX_WIN_D2D_UTILS_H
#define _SPX_WIN_D2D_UTILS_H

/* Other headers. */
#include "SPxLibUtils/SPxCommon.h"
#ifdef _WIN32
#include "ntverp.h"
#endif

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*
 * Determine compile-time support for Direct2D.
 *
 * Direct2D requires the Windows 7 SDK or Visual Studio 2010 (or later).
 * Direct2D 1.1 requires the Windows 8 SDK or Visual Studio 2012 (or later).
 */
#if defined(_WIN32) && defined(_WIN32_WINNT_VISTA)
#if defined(VER_PRODUCTMAJORVERSION) && defined(VER_PRODUCTMINORVERSION)
#if (VER_PRODUCTMAJORVERSION > 6) || ((VER_PRODUCTMAJORVERSION == 6) && (VER_PRODUCTMINORVERSION >= 1))
#define SPX_SUPPORT_D2D
#endif
#if (VER_PRODUCTMAJORVERSION > 6) || ((VER_PRODUCTMAJORVERSION == 6) && (VER_PRODUCTMINORVERSION >= 2))
#define SPX_SUPPORT_D2D_1_1
#endif
#endif
#endif

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Forward declarations. */
struct ID3D11Device;
struct ID3D11DeviceContext;

/*
 * Determine run-time support for Direct2D.
 *
 * Direct2D requires Windows Vista or later.
 * Direct2D 1.1 requires Windows 7 (with Platform Update) or later.
 */
extern int SPxWinD2DIsSupported(char *buffer=NULL, int bufLen=0);
extern int SPxWinD2D1_1IsSupported(char *buffer=NULL, int bufLen=0);

extern HRESULT SPxWinD3D11CreateDevice(ID3D11Device **device,
				       ID3D11DeviceContext **deviceContext);

#endif /* _SPX_WIN_D2D_UTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
