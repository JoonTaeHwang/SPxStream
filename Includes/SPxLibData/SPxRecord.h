/*********************************************************************
*
* (c) Copyright 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for recording object
*
* Revision Control:
*   18/02/22 v1.5    AGC	Add public CloseFile().
*
* Previous Changes:
*   09/02/22 1.4    AGC	Use SPxClock.
*   19/01/22 1.3    AGC	Add timestamp arg to RecordPacketB().
*   19/08/21 1.2    AGC	Optionally use packet B data for stats.
*   02/06/21 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_RECORD_H_
#define _SPX_RECORD_H_

/*
 * Other headers required.
 */
/* Include SPx headers we need. */
#include "SPxLibUtils/SPxObj.h"

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

/*
 * Forward-declare other classes in case headers are in wrong order.
 */
class SPxNavData;
class SPxClock;
struct SPxTime_tag;
struct SPxReturn_tag;
struct SPxReturnHeader_tag;
struct SPxPacketHeaderB_tag;

/*
 * Define our class.
 */
class SPxRecord : public SPxObj
{
public:
    SPxRecord(void);
    virtual ~SPxRecord(void);

    /* Configuration. */
    SPxErrorCode SetEncodeFormat(UINT8 encodeFormat);
    UINT8 GetEncodeFormat(void) const;
    SPxErrorCode SetFilePrefix(const char *filePrefix);
    SPxErrorCode GetFilePrefix(char *buf, unsigned int bufLen) const;
    SPxErrorCode SetMaxFileSizeMB(int maxFileSizeMB);
    int GetMaxFileSizeMB(void) const;
    SPxErrorCode SetMaxFileSizeSeconds(int maxFileSizeSeconds);
    int GetMaxFileSizeSeconds(void) const;
    SPxErrorCode SetTOCWriteIntervalSecs(int tocWriteIntervalSecs);
    int GetTOCWriteIntervalSecs(void) const;
    SPxErrorCode SetIncludeConfig(int includeConfig);
    int GetIncludeConfig(void) const;
    SPxErrorCode SetIncludeErrors(int includeErrors);
    int GetIncludeErrors(void) const;

    SPxErrorCode SetNavData(SPxNavData *navData);

    SPxErrorCode SetClock(SPxClock *clock);

    SPxErrorCode RecordPacketB(UINT16 packetType,
			       const void *payload,
			       unsigned int numBytes,
			       UCHAR channelIndex,
			       int includeInStats=FALSE,
			       const SPxTime_tag *ts=NULL);
    SPxErrorCode RecordPacketB(UINT16 packetType,
			       const void *payloadHdr,
			       unsigned int payloadHdrBytes,
			       const void *payload,
			       unsigned int numBytes,
			       const void *payload2,
			       unsigned int numBytes2,
			       UCHAR channelIndex,
			       int includeInStats=FALSE,
			       const SPxTime_tag *ts=NULL);
    SPxErrorCode RecordPacketB(const SPxPacketHeaderB_tag *packetHdr,
			       const void *payloadHdr,
			       unsigned int payloadHdrBytes,
			       const void *payload,
			       unsigned int payloadBytes,
			       UCHAR channelIndex,
			       int includeInStats=FALSE);
    SPxErrorCode RecordPacketB(const SPxPacketHeaderB_tag *packetHdr,
			       const void *payloadHdr,
			       unsigned int payloadHdrBytes,
			       const void *payload,
			       unsigned int payloadBytes,
			       const void *payload2,
			       unsigned int payloadBytes2,
			       UCHAR channelIndex,
			       int includeInStats=FALSE);
    SPxErrorCode RecordReturn(SPxReturn_tag *rtn,
			      UCHAR channelIndex,
			      SPxTime_tag *rtnTime,
			      int includeInStats=TRUE);

    SPxErrorCode CloseFile(void);

    SPxErrorCode RecordRadarSourceChange(void);
    SPxErrorCode UpdateInternals(UINT16 packetType);
    int GetAndClearStatsChangeFlag(void);

    /* Retrieve status information. */
    double GetStatusBandwidth(void) const;
    double GetStatusCompressionRatio(void) const;
    int GetStatusDiskSpaceMB(void) const;
    SPxErrorCode GetFilename(char *buf, unsigned int bufLen) const;
    SPxErrorCode GetFirstFilename(char *buf, unsigned int bufLen) const;
    UINT32 GetFileStartSeconds(void) const;
    UINT32 GetFileDurationSeconds(void) const;
    UINT32 GetSessionStartSeconds(void) const;
    UINT32 GetSessionDurationSeconds(void) const;
    unsigned int GetSessionNumFiles(void) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value) SPX_VIRTUAL_OVERRIDE;
    virtual int GetParameter(char *name, char *valueBuf, int bufLen) SPX_VIRTUAL_OVERRIDE;

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode checkFile(SPxTime_tag *epochTime,
			   SPxReturnHeader_tag *rtnHeader,
			   UCHAR channelIndex);
    SPxErrorCode writeToFileB(const SPxPacketHeaderB_tag *packetHdr,
			      const void *payloadHdr,
			      unsigned int payloadHdrSize,
			      const void *payload,
			      unsigned int payloadSize,
			      const void *payload2=NULL,
			      unsigned int payloadSize2=0,
			      int isPacketRewrite=FALSE);
    SPxErrorCode updateStatus(const SPxTime_tag *epochTime);
    SPxErrorCode considerNewTOCEntry(UINT32 packetSecs);
    SPxErrorCode writeTOCToFile(void);
    SPxErrorCode closeTheFile(void);
    SPxErrorCode byteSwapReturnData(SPxReturn_tag *rtn,
				    unsigned int bytesPerSample);
    SPxErrorCode checkAllocReturnBuf(void);
    SPxErrorCode updatePrevLinkPacket(UINT16 packetType,
				      INT64 fromPos,
				      INT64 toPos);
    SPxErrorCode updatePrevLinkPacketComplete(FILE *fileHandle,
					      INT64 initialPos,
					      SPxErrorCode prevErr);

    static int navDataDeleteCallback(void *invokingObject,
				     void *userObject,
				     void *arg);
    static int navDataCallback(void *navObj, void *recProcess,
			       void *arg);
    static SPxErrorCode recordPacketBWrapper(void *userArg,
					     unsigned short packetType,
					     const void *payload,
					     unsigned int numBytes,
					     unsigned char channelIndex);

}; /* SPxRecord */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_RECORD_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
