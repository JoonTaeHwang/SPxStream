/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for network URI functions.
*
* Revision Control:
*   28/10/12 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_NET_URI_H
#define _SPX_NET_URI_H

/*
 * Other headers required.
 */

 /* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

extern SPxErrorCode SPxUriEncode(const char *uri,
				 char *buf, unsigned int bufLen);
extern SPxErrorCode SPxUriDecode(const char *uri,
				 char *buf, unsigned int bufLen);

extern SPxErrorCode SPxPercentEncode(const char *str,
				     const char *allowedChars,
				     char *buf, unsigned int bufLen);
extern SPxErrorCode SPxPercentDecode(const char *str,
				     char *buf, unsigned int bufLen);

#endif /* _SPX_NET_URI_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

