/*********************************************************************
*
* (c) Copyright 2007 - 2016, 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for time handling in SPx.
*
* Revision Control:
*   22/06/22 v1.18   REW	Add SPxTimeGetBuildEpoch().
*
* Previous Changes:
*   17/09/20 1.17   REW	Add SPxTimeGetSignedDiff().
*   03/10/16 1.16   AGC	Add thread-safe SPxLocaltime/SPxGmtime().
*   22/10/15 1.15   AGC	Add INT64 SPxTimeAddEpoch() overload.
*   21/11/14 1.14   SP 	Add SPxTimeAddSecsEpoch().
*   24/09/14 1.13   AGC	Add precise ticker.
*   08/10/13 1.12   AGC	Include time.h to fix QNX error.
*   04/10/13 1.11   AGC	Simplify headers.
*   12/10/12 1.10   REW	Add SPxTimeGetEpochFromTm().
*   19/09/11 1.9    REW	Support Solaris build (need time.h).
*   02/09/11 1.8    SP 	Support QNX build.
*   30/09/10 1.7    SP 	Support Borland build.
*   21/05/10 1.6    REW	Add SPxTimeGetTmFromEpoch().
*   16/09/08 1.5    REW	Add SPxTimeAddEpoch().
*   08/01/08 1.4    REW	Use const where appropriate.
*   04/01/08 1.3    REW	Add SPxTimeGetEpoch() etc.
*   08/08/07 1.2    REW	Add SPxTimeGetDiff().
*   07/06/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_TIME_H
#define _SPX_TIME_H

/* For tm structure (which cannot be forward declared under QNX). */
#include <time.h>

/* Make sure we have the common definitions and headers. */
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Define a type to hold the composite time since the epoch. */
typedef struct SPxTime_tag {
    UINT32 secs;			/* Whole seconds since epoch */
    UINT32 usecs;			/* Microseconds to add */
} SPxTime_t;


/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Extern the functions we provide. */
extern SPxErrorCode SPxTimeSleepMsecs(unsigned int msecs);

/* Ticker (milliseconds since unknown point, with wraparounds) */
extern UINT32 SPxTimeGetTickerMsecs(void);
extern UINT32 SPxTimeGetDiff(UINT32 firstMsecs, UINT32 secondMsecs);
extern INT32 SPxTimeGetSignedDiff(UINT32 aMsecs, UINT32 bMsecs);

/* Precise ticker (microseconds since unknown point, with wraparounds) */
extern UINT64 SPxTimeGetTickerPrecise(void);
extern UINT64 SPxTimeGetDiffPrecise(UINT64 first, UINT64 second);

/* Epoch times (composite time since 1st Jan 1970). */
extern int SPxTimeGetEpoch(SPxTime_t *now);
extern double SPxTimeGetDiffEpoch(const SPxTime_t *first,
				  const SPxTime_t *second);
extern int SPxTimeCopyEpoch(SPxTime_t *dest, const SPxTime_t *src);
extern int SPxTimeAddEpoch(SPxTime_t *dest, int usecs);
extern int SPxTimeAddEpoch(SPxTime_t *dest, INT64 usecs);
extern int SPxTimeAddSecsEpoch(SPxTime_t *dest, double secs);
extern SPxErrorCode SPxTimeGetTmFromEpoch(struct tm *tmPtr,
					  const SPxTime_t *epochTime);
extern SPxErrorCode SPxTimeGetEpochFromTm(SPxTime_t *epochTime,
					  struct tm *tmPtr);
extern SPxErrorCode SPxTimeGetBuildEpoch(SPxTime_t *epochTime);

extern struct tm *SPxLocaltime(struct tm *tmPtr, const time_t *timePtr);
extern struct tm *SPxGmtime(struct tm *tmPtr, const time_t *timePtr);

#endif /* _SPX_TIME_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
