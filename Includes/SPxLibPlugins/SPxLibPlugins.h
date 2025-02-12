/*********************************************************************
*
* (c) Copyright 2009, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLibPlugins.h,v $
* ID: $Id: SPxLibPlugins.h,v 1.2 2009/07/31 12:40:11 rew Exp $
*
* Purpose:
*   Top-level header for SPxLibPlugins library.
*
*   This should be the only header that needs to be included in
*   code that uses this library. It includes all the other public
*   headers for the library, specifying the full (relative) pathname
*   to the headers.
*
* Revision Control:
*   31/07/09 v1.2    SP 	Add SPxPluginProcess.h and SPxPluginUtils.h
*
* Previous Changes:
*   21/07/09 1.1    SP 	Initial version.
**********************************************************************/

#ifndef _SPX_LIBPLUGINS_H_
#define	_SPX_LIBPLUGINS_H_

/* Include all the public headers from this library, assuming they
 * are in a subdirectory of one of the directories in the include
 * path (i.e. so you don't need to add each subdirectory to the include
 * path, just the parent of them all).
 */
#include "SPxLibPlugins/SPxPlugin.h"
#include "SPxLibPlugins/SPxPluginProcess.h"
#include "SPxLibPlugins/SPxPluginUtils.h"
#include "SPxLibPlugins/SPxPluginHandler.h"

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_LIBPLUGINS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
