/*********************************************************************
*
* (c) Copyright 2012, 2017, 2019, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for general network utility functions.
*
* Revision Control:
*   12/02/21 v1.7    AGC	Support returning status code from SPxDownloadFile().
*
* Previous Changes:
*   28/02/19 1.6    AGC	Move URI functions to SPxNetUri.
*   27/02/19 1.5    AGC	Add SPxPercentEncode/Decode().
*   18/01/19 1.4    AGC	Add SPxUriEncode/SPxUriDecode().
*   13/11/17 1.3    AGC	Support HTTP POST with SPxDownloadFile().
*   09/11/12 1.2    AGC	Tidy line endings.
*   26/10/12 1.1    AGC	Initial version.
* 
**********************************************************************/

#ifndef _SPX_NET_UTILS_H
#define _SPX_NET_UTILS_H

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

extern SPxErrorCode SPxDownloadFile(const char *url,
                                    unsigned char *buffer,
                                    unsigned int *bufferSizeBytes,
                                    unsigned int timeoutMsecs,
                                    const char *headers=NULL,
                                    const char *post=NULL,
                                    unsigned int *statusCodeRet=NULL);

#endif /* _SPX_NET_UTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
