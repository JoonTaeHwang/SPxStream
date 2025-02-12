/*********************************************************************
*
* (c) Copyright 2010, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNoMFC.h,v $
* ID: $Id: SPxNoMFC.h,v 1.1 2010/06/14 13:19:34 rew Exp $
*
* Purpose:
*	Top-level header for SPx libraries when the user application
*	will not be using MFC (as opposed to the normal SPx.h header).
*
*
* Revision Control:
*   14/06/10 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

/* Don't multiply include this. */
#ifndef _SPX_NO_MFC_H
#define	_SPX_NO_MFC_H

/* Define the "NO MFC" flag, then include the normal header. */
#define	SPX_OMIT_MFC	1
#include "SPx.h"

#endif /* _SPX_NO_MFC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
