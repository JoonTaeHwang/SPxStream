/*********************************************************************
*
* (c) Copyright 2007 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Top-level header for SPxLibSc library.
*
*   This should be the only header that needs to be included in
*   code that uses this library.  It includes all the other public
*   headers for the library.
*
* Revision Control:
*   11/08/20 v1.13   AGC	Fix last change for Windows builds.
*
* Previous Changes:
*   07/08/20 1.12   AGC	Improve support without X11.
*   04/10/13 1.11   AGC	Move functions to SPxLibSc.h.
*   26/04/13 1.10   SP 	Omit OpenGL files on unsupported platforms.
*   25/04/13 1.9    SP 	Add SPxScDestTexture.h.
*   24/04/09 1.8    REW	Include SPxInit.h
*   18/08/08 1.7    REW	Include SPxScDestMcast.h and SPxScSourceMcast.h
*   09/07/07 1.6    REW	Include SPxScDestBitmap.h. Args to SPxLibScInit().
*   02/07/07 1.5    REW	Include SPxScDestScServer.h.
*   28/06/07 1.4    DGJ	Include SPxScRemote.h
*   11/06/07 1.3    REW	SPxLibSC name changed to SPxLibSc.
*   07/06/07 1.2    REW	Headers are in subdir. Init returns int.
*   24/05/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_LIBSC_H_
#define	_SPX_LIBSC_H_

/* Include all the public headers from this library, assuming they
 * are in a subdirectory of one of the directories in the include
 * path (i.e. so you don't need to add each subdirectory to the include
 * path, just the parent of them all).
 */
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibSc/SPxScInit.h"
#include "SPxLibSc/SPxScSourceLocal.h"
#include "SPxLibSc/SPxScSourceNet.h"
#include "SPxLibSc/SPxScSourceMcast.h"
#include "SPxLibSc/SPxScDestBitmap.h"
#include "SPxLibSc/SPxScDestMcast.h"
#include "SPxLibSc/SPxScDestNet.h"
#include "SPxLibSc/SPxScDestScServer.h"
#include "SPxLibSc/SPxScRemote.h"
#include "SPxLibSc/SPxInit.h"

/* Omit these files on platforms that do not support OpenGL. */
#if defined(_WIN32) || SPX_X11_SUPPORTED
#include "SPxLibSc/SPxScDestTexture.h"
#endif

#endif /* _SPX_LIBSC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
