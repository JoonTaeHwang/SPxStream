/*********************************************************************
*
* (c) Copyright 2007, 2008, 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLibProc.h,v $
* ID: $Id: SPxLibProc.h,v 1.6 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Top-level header for SPxLibProc library.
*
*   This should be the only header that needs to be included in
*   code that uses this library.  It includes all the other public
*   headers for the library, specifying the full (relative) pathname
*   to the headers, i.e. it expects the headers to be in an SPxLibProc
*   subdirectory.
*
*
* Revision Control:
*   04/10/13 v1.6    AGC	Move functions to SPxProcInit.h.
*
* Previous Changes:
*   29/05/12 1.5    REW	Add proximity detection header.
*   02/01/08 1.4    REW	Add tracking/blob-detection headers.
*   09/07/07 1.3    REW	Args to SPxLibProcInit().
*   07/06/07 1.2    REW	Init function returns status value.
*			Headers are in subdirectories.
*   24/05/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_LIBPROC_H_
#define	_SPX_LIBPROC_H_

/* Include all the public headers from this library, assuming they
 * are in a subdirectory of one of the directories in the include
 * path (i.e. so you don't need to add each subdirectory to the include
 * path, just the parent of them all).
 */
#include "SPxLibProc/SPxProcInit.h"
#include "SPxLibProc/SPxProcStd.h"
#include "SPxLibProc/SPxTargetDB.h"
#include "SPxLibProc/SPxBlobDetect.h"
#include "SPxLibProc/SPxTracker.h"
#include "SPxLibProc/SPxProximityDetect.h"

#endif /* _SPX_LIBPROC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
