/*********************************************************************
*
* (c) Copyright 2013 - 2016, 2018 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Initialisation and license functions for SPxLibUtils.
*
* Revision Control:
*   31/03/21 v1.11   AGC	Add silent flag to SPxLicInit().
*
* Previous Changes:
*   25/01/20 1.10   REW	Support uniqueKey in SPxLicLM functions etc.
*   14/01/20 1.9    SP 	Add SPxLicGetVersion().
*   22/08/19 1.8    REW	Start support for License Manager option.
*   06/06/18 1.7    AGC	Add counts argument to SPxLicBuildReport().
*   31/10/16 1.6    AGC	Add AV bits argument to SPxLicBuildReport().
*   08/07/16 1.5    REW	Add MBT check.
*   07/01/15 1.4    AGC	Improve comments.
*   01/07/14 1.3    AGC	Add more options to SPxLicBuildReport().
*   26/06/14 1.2    AGC	Add SPxLicBuildReport() function.
*   04/10/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_UTILS_INIT_H_
#define _SPX_UTILS_INIT_H_

#include <stdio.h>

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

extern int SPxLibUtilsInit(int verbose=0, FILE *logFile=NULL);

/* Licensing support. */
extern int SPxLicInit(int silent=FALSE);
extern void SPxLicReport(FILE *logFile);
extern UINT32 SPxLicGetID(int silent=FALSE);
extern UINT32 SPxLicGetVersion(int silent=FALSE);
extern int SPxLicBuildReport(char *buffer, int bufLen, UINT32 appBits=0,
			     UINT32 featureBits=0xFFFFFFFF,
			     UINT32 feature2Bits=0xFFFFFFFF,
			     int incLicNum=TRUE, int incLicExpiry=TRUE,
			     int incCounts=TRUE);

extern int SPxLicGetFrameworkStatus(void);

/* License Manager support. */
typedef SPxErrorCode (*SPxLicLMRegFn_t)(const void *msg,
					    unsigned int msgSizeBytes);
extern SPxErrorCode SPxLicLMInstallRegFn(SPxLicLMRegFn_t fn);
extern SPxErrorCode SPxLicLMDefaultRegFn(const void *msg,
					    unsigned int msgSizeBytes);
extern SPxErrorCode SPxLicLMProcessRequest(const void *msg,
					    unsigned int msgSizeBytes,
					    void *replyBuf,
					    unsigned int *replyBufSizeBytes,
					    const char *uniqueKey=NULL);
extern SPxErrorCode SPxLicLMProcessReply(const void *msg,
					    unsigned int msgSizeBytes);
extern SPxErrorCode SPxLicLMClientTerminating(const char *uniqueKey);

/* Convenience functions, return FALSE if not licensed, TRUE if licensed. */
extern int SPxLicIsPpiLicensed(void);
extern int SPxLicIsAScanLicensed(void);
extern int SPxLicIsBScanLicensed(void);
extern int SPxLicIsPlotExtLicensed(void);
extern int SPxLicIsTrackingLicensed(void);
extern int SPxLicIsMBTTrackingLicensed(void);
extern int SPxLicIsNetSourceLicensed(void);
extern int SPxLicIsHPxSourceLicensed(void);
extern int SPxLicIsDistributionLicensed(void);
extern int SPxLicIsRecordingLicensed(void);

#endif /* _SPX_UTILS_INIT_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
