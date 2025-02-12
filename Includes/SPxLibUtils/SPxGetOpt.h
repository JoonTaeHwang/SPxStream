/*********************************************************************
*
* (c) Copyright 2007 - 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxGetOpt.h,v $
* ID: $Id: SPxGetOpt.h,v 1.4 2014/09/01 15:00:01 rew Exp $
*
* Purpose:
*   Header file for SPx getopt().
*
* Revision Control:
*   01/09/14 v1.4    AGC	Make optstring arg const.
*
* Previous Changes:
*   17/03/14 1.3    SP	Extern optarg as SPX_CHAR not char.
*   27/09/13 1.2    AGC	Suport Unicode.
*   07/08/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_GETOPT_H
#define _SPX_GETOPT_H

#include "SPxLibUtils/SPxWideString.h"

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/* Extern the variables. */
extern SPX_CHAR *optarg;
extern int optind, opterr, optopt;

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Extern the functions we provide. */
extern int getopt(int argc, SPX_CHAR *argv[], const SPX_CHAR *optstring);

#endif /* _SPX_GETOPT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
