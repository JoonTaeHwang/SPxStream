/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Top-level header for SPx libraries.
*
*	Applications wishing to use the SPx libraries may include
*	just this file and it will include all the headers for all
*	the sub-libraries.  Alternatively, applications may include
*	just the specific headers they required.
*
*
* Revision Control:
*   26/08/22 v1.23   BTB	Add SPxLibOA.h.
*
* Previous Changes:
*   15/07/21 1.22   AGC	Support VS2022 version check.
*   07/06/21 1.21   AGC	Omit SPxLibPlugins.h on ARM64.
*   07/08/20 1.20   AGC	Support compilation without X11.
*   05/12/18 1.19   AGC	Support VS2019 version check.
*   16/05/17 1.18   AGC	Support VS2017 version check.
*   01/09/14 1.17   AGC	Support VS2015 version check.
*   04/07/14 1.16   AGC	Support VS2013 in version checks.
*			Include some standard headers.
*   27/09/13 1.15   AGC	Define SPX_AFXWIN in Windows MFC builds.
*   25/01/13 1.14   AGC	Support VS2012 in version checks.
*   18/10/12 1.13   AGC	Omit SPxLibX11.h and SPxLibPlugins.h on ARM.
*   08/05/12 1.12   AGC	Add Visual Studio version check.
*   02/09/11 1.11   SP 	Omit SPxLibX11.h on QNX.
*   13/04/11 1.10   SP 	Omit SPxLibX11.h and SPxLibPlugins.h
*			on Solaris.
*   30/09/10 1.9    SP 	Move SPX_OMIT_MFC to SPxLibWin.h.
*			Include SPxLibWin.h for all Windows builds.
*   14/06/10 1.8    REW	Exclude SPxLibWin if SPX_OMIT_MFC.
*   31/07/09 1.7    SP 	Add SPxLibPlugins.h
*   24/04/09 1.6    REW	Move functions to other headers.
*   29/05/08 1.5    REW	Add SPxIsInitialised().
*   12/02/08 1.4    REW	Add SPxGetBuildString() etc.
*   07/01/08 1.3    DGJ	Ensure WINVER is defined.
*   25/07/07 1.2    REW	Add SPxLibNet.h
*   09/07/07 1.1    REW	Initial Version.
**********************************************************************/

/* Don't multiply include this. */
#ifndef _SPX_H
#define	_SPX_H

/* Require Windows XP or later, if windows. */
#ifdef	WIN32
#ifndef	WINVER
#define	WINVER 0x0501
#endif	/* WINVER */
#endif	/* WIN32 */

/* Define SPX_AFXWIN for MFC includes when using MFC. */
#ifdef _WIN32
#ifndef SPX_OMIT_MFC
#define SPX_AFXWIN
#endif
#endif

/* Include the top level headers for each sub-library.  This assumes they
 * are in a subdirectory of one of the directories in the include
 * path (i.e. so you don't need to add each subdirectory to the include
 * path, just the parent of them all).
 */
#include "SPxLibUtils/SPxLibUtils.h"
#include "SPxLibData/SPxLibData.h"
#include "SPxLibSc/SPxLibSc.h"
#include "SPxLibProc/SPxLibProc.h"
#include "SPxLibNet/SPxLibNet.h"
#include "SPxLibOA.h"

#if !defined(__sun) && !defined(__arm)
#if !defined(__arm__) && !defined(__aarch64__) /* Omit this file on Solaris/ARM */
#include "SPxLibPlugins/SPxLibPlugins.h"
#endif
#endif

#ifdef _WIN32
#include "SPxLibWin/SPxLibWin.h"
#else
#if SPX_X11_SUPPORTED
#include "SPxLibX11/SPxLibX11.h"
#endif
#endif /* _WIN32 */

/* Also include the version information. */
#include "SPxVersion.h"

/* Include standard headers that used to be
 * included by various SPx headers.
 */
#include <sstream>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

/* Check the Visual Studio version is correct under MFC Windows. */
#ifdef _WIN32
#ifndef SPX_OMIT_MFC

/* Include Visual Studio version header under Windows. */
#include "SPxMscVer.h"

/* Check Visual Studio version matches SPX_HEADER_MSC_VER. */
#if (SPX_HEADER_MSC_VER == 2005)
#if (_MSC_VER < 1400 || _MSC_VER >= 1500)
#error "This SPx package is built for Visual Studio 2005"
#endif
#endif
#if (SPX_HEADER_MSC_VER == 2008)
#if (_MSC_VER < 1500 || _MSC_VER >= 1600)
#error "This SPx package is built for Visual Studio 2008"
#endif
#endif
#if (SPX_HEADER_MSC_VER == 2010)
#if (_MSC_VER < 1600 || _MSC_VER >= 1700)
#error "This SPx package is built for Visual Studio 2010"
#endif
#endif
#if (SPX_HEADER_MSC_VER == 2012)
#if (_MSC_VER < 1700 || _MSC_VER >= 1800)
#error "This SPx package is built for Visual Studio 2012"
#endif
#endif
#if (SPX_HEADER_MSC_VER == 2013)
#if (_MSC_VER < 1800 || _MSC_VER >= 1900)
#error "This SPx package is built for Visual Studio 2013"
#endif
#endif
#if (SPX_HEADER_MSC_VER == 2015)
#if (_MSC_VER < 1900 || _MSC_VER >= 2000)
#error "This SPx package is built for Visual Studio 2015"
#endif
#endif
#if (SPX_HEADER_MSC_VER == 2017)
#if (_MSC_VER < 1910 || _MSC_VER >= 2000)
#error "This SPx package is built for Visual Studio 2017"
#endif
#endif
#if (SPX_HEADER_MSC_VER == 2019)
#if (_MSC_VER < 1920 || _MSC_VER >= 2000)
#error "This SPx package is built for Visual Studio 2019"
#endif
#endif
#if (SPX_HEADER_MSC_VER == 2022)
#if (_MSC_VER < 1930 || _MSC_VER >= 2000)
#error "This SPx package is built for Visual Studio 2022"
#endif
#endif

#endif /* _WIN32 */
#endif /* NOT SPX_OMIT_MFC */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
