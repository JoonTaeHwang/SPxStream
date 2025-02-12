/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxWinInit.h,v $
* ID: $Id: SPxWinInit.h,v 1.1 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header containing initialisation functions for Windows part
*   of SPx library.
*
* Revision Control:
*   04/10/13 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_WIN_INIT_H
#define _SPX_WIN_INIT_H

#include <stdio.h>

#include "SPxLibUtils/SPxCommon.h"

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* From SPxWinInit.cpp */
extern int SPxLibWinInit(int verbose=0, FILE *logFile=NULL);

/*********************************************************************
*
*   Public Variables
*
**********************************************************************/

/* Set to TRUE if running on a 64-bit version of Windows */
extern BOOL SPxIsWin64Bit;

#endif /* _SPX_WIN_INIT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
