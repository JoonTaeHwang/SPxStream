/*********************************************************************
*
* (c) Copyright 2007 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Common definitions across all SPx libraries.
*
* Revision Control:
*   18/10/21 v1.49   AGC	Increase FD_SETSIZE on Windows to 1024.
*
* Previous Changes:
*   03/09/21 1.48   AGC	Improve SPX_GTK_SUPPORTED flag set.
*   02/09/21 1.47   AGC	Add SPX_GTK_SUPPORTED flag.
*   07/06/21 1.46   AGC	Improve detection of no X11 support for ARM64.
*   16/03/21 1.45   AGC	Move SPX_SOCK_INVALID here from SPxScNet.h.
*   11/09/20 1.44   AGC	Include afxdtctl header for MFC builds.
*   07/08/20 1.43   AGC	Improve support without X11.
*   01/12/15 1.42   AGC	Add Windows Sync message definition.
*   16/02/15 1.41   AGC	Avoid warnings from 64-bit build with newer Windows SDKs.
*   05/02/15 1.40   AGC	Disable warnings from newer MFC Windows SDKs.
*   26/01/15 1.39   AGC	Disable deprecated warnings in windows headers (newer SDKs).
*   13/01/15 1.38   AGC	Avoid more warnings in windows headers (newer SDKs).
*   15/12/14 1.37   AGC	Avoid warnings in windows headers (newer SDKs).
*   26/03/14 1.36   AGC	Disable MFC MBCS deprecated warnings.
*   21/10/13 1.35   AGC	Add afxext.h when building for MFC.
*   04/10/13 1.34   AGC	Move platform independent types to SPxTypes.h.
*   27/09/13 1.33   AGC	Include afxcmn.h when building for MFC.
*   24/05/13 1.32   AGC	Fix WINVER undefined warnings.
*   14/01/13 1.31   AGC	Do not set INT32 if Xmd.h header included.
*			Add compile time size checks for types.
*   21/12/12 1.30   AGC	Add SPX_MPS_PER_KPH and SPX_KPH_PER_MPS.
*   18/10/12 1.29   AGC	Do not set SPX_X11_SUPPORTED on ARM.
*   11/09/12 1.28   AGC	Move SPxPoint here from SPxProcessRegion.h.
*   02/08/12 1.27   AGC	Move SOCKET_TYPE and SOCKLEN_TYPEs here from SPxScNet.h.
*   18/07/12 1.26   AGC	Support fns returning int in SPX_RETURN_IF_FAIL.
*   29/03/12 1.25   AGC	Add SPX_RETURN_IF_NULL and SPX_RETURN_IF_FAIL macros.
*			Add SPX_FUNCTION_NAME.
*   28/02/12 1.24   REW	Add SPX_KNOTS_PER_MPS.
*   22/02/12 1.23   SP 	Define SPX_X11_SUPPORTED if X Windows is supported.
*   13/01/12 1.22   REW	Define various windows versions if not set.
*   03/01/12 1.21   REW	Add SPX_METRES_PER_FOOT and FEET_PER_METRES.
*   02/12/11 1.20   AGC	Add SPxMetadataOptions_t enumeration.
*   19/08/11 1.19   SP 	Add SPX_UINT8_MAXVAL and friends.
*   09/06/11 1.18   AGC	Add SPxMetadata_t enumeration.
*   25/03/11 1.17   SP 	Support Solaris 10 build.
*   17/12/10 1.16   AGC	Add "Shlwapi.h" header for path manipulation 
*			functions on windows.
*   04/10/10 1.15   AGC	Add SPxFileFormat_t enumeration.
*   15/09/10 1.14   REW	Exclude Windows.h if RC_INVOKED (from AGC).
*   16/12/09 1.13   REW	Protect _CRT_SECURE_NO_WARNINGS with ifndef.
*   27/02/09 1.12   REW	Add SPX_MPS_PER_KNOT.
*   07/08/08 1.11   REW	Add SPX_METRES_PER_NM.
*   17/03/08 1.10   REW	Add SPX_SPEED_OF_LIGHT_MPS.
*   10/12/07 1.9    REW	Add INT64 and UINT64.
*   06/09/07 1.8    DGJ	Add SPX_DEGREES_TO_16BITAZIMUTH
*   20/08/07 1.7    DGJ	Remove Confirm().
*   26/07/07 1.6    REW	INT8 needs to be signed char for MSVC.
*   16/07/07 1.5    REW	Add 8-bit typedefs.
*   07/06/07 1.4    REW	Remove unused defs, add others. Comments etc.
*			Include winsock2.h before windows.h for WIN32.
*   04/06/07 1.3    DGJ	Added common fixed size typedefs.
*   24/05/07 1.2    REW	Added header, changed to SPx etc.
*			Move error defs to SPxError.h
*   20/05/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_COMMON_H
#define _SPX_COMMON_H

/* Platform independent types now in SPxTypes.h */
#include "SPxTypes.h"

/* We need windows.h on all Windows builds, so just include it here, unless
 * we are being invoked by the resource compiler (because the .rc files can
 * include SPxVersion.h which ultimately includes this, which causes warnings
 * about very long identifiers in the resource compiler on MSVC 2010). Also,
 * disable the CRT security warnings.
 */
#ifdef _WIN32

#ifndef	_CRT_SECURE_NO_WARNINGS
#define	_CRT_SECURE_NO_WARNINGS
#endif

#ifndef NO_WARN_MBCS_MFC_DEPRECATION
#define NO_WARN_MBCS_MFC_DEPRECATION
#endif

#ifndef	RC_INVOKED		/* See note above */

/* Define various windows versions, in case we haven't already had these
 * set (e.g. by stdafx.h).  This should be done before Windows.h.
 */
#ifndef WINVER
#define	WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define	_WIN32_WINNT 0x0501
#endif

#ifndef _WIN32_WINDOWS
#define	_WIN32_WINDOWS 0x0501
#endif

/* Increase size of socket descriptor array on Windows to maximum supported value. */
#ifndef FD_SETSIZE
#define FD_SETSIZE 1024
#endif

#ifdef SPX_AFXWIN
#pragma warning(push)
#pragma warning(disable: 4068 4996)
#ifdef _WIN64
#pragma warning(disable: 4163)
#endif
#include "afxwin.h"
#include "afxcmn.h"
#include "afxext.h"
#include "afxdtctl.h"
#include "winsock2.h"
#pragma warning(pop)
#else
/* Disable warnings from windows headers. */
#pragma warning(push)
#pragma warning(disable: 4311 4312 4201 4068 4996)
#ifdef _WIN64
#pragma warning(disable: 4163)
#endif
#include "winsock2.h"
#include "Windows.h"
#pragma warning(pop)
#endif
#include "Shlwapi.h"

#endif	/* RC_INVOKED */

/* Window message for syncing parent dialog based on child changes. */
#define SPX_WIN_SYNC (WM_APP + 8)
#define ON_SPX_SYNC(memberFxn) \
	{ WM_NOTIFY, (WORD)(int)SPX_WIN_SYNC, (WORD)0, (WORD)0, AfxSigNotify_v, \
		(AFX_PMSG) \
		(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*) > \
		(memberFxn)) },

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif	/* _WIN32 */

/* Check if X Windows/GTK is supported on this platform. */
#if !defined(WIN32) && !defined(__BORLAND__)
#if !defined(__sun) && !defined(__QNX__)
#if !defined(__arm) && !defined(__aarch64__)
#ifndef SPX_X11_SUPPORTED
#define SPX_X11_SUPPORTED 1  /* X Windows is supported. */
#endif
#ifndef SPX_GTK_SUPPORTED
#define SPX_GTK_SUPPORTED 1  /* GTK is supported. */
#endif
#endif /* !__arm && !__aarch64__ */
#endif /* !WIN32 && !__BORLAND__ */
#endif /* !__sun && !__QNX__ */

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Enumerations
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/
/*
 * Define a platform-independent
 * SOCKET_TYPE, SOCKLEN_TYPE and SPX_SOCK_INVALID.
 */
#ifdef _WIN32
typedef SOCKET SOCKET_TYPE;
typedef int SOCKLEN_TYPE;
#define	SPX_SOCK_INVALID    INVALID_SOCKET
#else
typedef int SOCKET_TYPE;
typedef socklen_t SOCKLEN_TYPE;
#define	SPX_SOCK_INVALID    -1
#endif /* WIN32 */

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_COMMON_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
