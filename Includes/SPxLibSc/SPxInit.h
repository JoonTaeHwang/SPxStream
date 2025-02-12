/*********************************************************************
*
* (c) Copyright 2009, 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPx init functions.
*
*
* Revision Control:
*   05/04/22 v1.4    AGC	Avoid header/library version check triggering warning.
*
* Previous Changes:
*   08/12/21 1.3    AGC	Move header/library version check here.
*   04/10/13 1.2    AGC	Simplify headers.
*   24/04/09 1.1    REW	Initial Version from SPx.h.
**********************************************************************/

/* Don't multiply include this. */
#ifndef _SPX_INIT_H
#define	_SPX_INIT_H

#include <stdio.h>
#include "SPxVersion.h"

/* Macros for generating version specific symbol. */
#define SPX_CREATE_VERSION_VARIABLE2(major, minor, micro, nano) \
    SPx_Header_Library_Version_Mismatch_##major##_##minor##_##micro##_##nano
#define SPX_CREATE_VERSION_VARIABLE(major, minor, micro, nano) \
    SPX_CREATE_VERSION_VARIABLE2(major, minor, micro, nano)

/* Variable that checks header/library version match. */
extern const int SPX_CREATE_VERSION_VARIABLE(SPX_VERSION_MAJOR,
					     SPX_VERSION_MINOR,
					     SPX_VERSION_MICRO,
					     SPX_VERSION_NANO);

const int SPX_VERSION_VARIABLE =
SPX_CREATE_VERSION_VARIABLE(SPX_VERSION_MAJOR,
			    SPX_VERSION_MINOR,
			    SPX_VERSION_MICRO,
			    SPX_VERSION_NANO);

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Main initialisation function for the libraries.  Calling this
 * function will in turn call the initialisation functions for each
 * individual sub-library.
 *
 * The side-effect of this is that the application will drag in many of
 * the sub-library modules.  If that is undesired, the application
 * should call the appropriate initialisation functions separately.
 */
extern int SPxInit(int verbose=0, FILE *logFile=NULL,
		   int dummy=SPX_VERSION_VARIABLE);
extern int SPxIsInitialised(void);

#endif /* _SPX_INIT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
