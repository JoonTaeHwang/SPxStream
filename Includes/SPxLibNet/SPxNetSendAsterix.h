/*********************************************************************
*
* (c) Copyright 2011 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for ASTERIX Cat-240 network sending object.
*
* Revision Control:
*   04/06/20 v1.19   AGC	Add parameter for max backwards jump skip.
*
* Previous Changes:
*   13/11/19 1.18   AGC	Fix more data races.
*   11/11/19 1.17   AGC	Fix data race on shutdown when testing.
*   24/04/19 1.16   SP 	Support sending 1, 2 and 4-bit video.
*   12/06/18 1.15   AGC	Avoid errors with custom SP field.
*   04/06/18 1.14   AGC	Start summary thread from Create().
*   17/05/18 1.13   AGC	Support custom SP field.
*   18/10/17 1.12   AGC	Add IsEncodeFormatSupported().
*   09/05/17 1.11   AGC	Add SendData() function for testing.
*   04/05/17 1.10   AGC	Add range resolution control.
*   12/04/17 1.9    AGC	Improve would block checks.
*   10/12/15 1.8    AGC	Add virtual GetProjectCode().
*   17/03/15 1.7    AGC	Remove compress (accessible from base class).
*   10/04/14 1.6    SP 	Add Set/IsSummaryAlwaysEnabled().
*   04/10/13 1.5    AGC	Simplify headers.
*   25/04/13 1.4    AGC	Use new SPxCompress class.
*   12/02/13 1.3    SP 	Support fragmentation (TODO #132).
*   14/11/11 1.2    SP 	Rename some items and tidy.
*   02/11/11 1.1    SP 	Initial Version.
*
**********************************************************************/

#ifndef _SPX_NET_SEND_ASTERIX_H
#define _SPX_NET_SEND_ASTERIX_H

/*
 * Other headers required.
 */

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need the base class headers (which includes several others as well). */
#include "SPxLibNet/SPxNetSend.h"
#include "SPxLibNet/SPxNetAsterix.h"


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

/* Forward declarations. */
class SPxCompress;
struct SPxTime_tag;
class SPxThread;
struct SPxReturn_tag;
struct SPxReturnHeader_tag;


/*
 * Define our project-specific network sender class, derived from the
 * standard one.
 */
class SPxNetworkSendAsterix : public SPxNetworkSend, public SPxNetAsterix
{
public:
    /*
     * Public types.
     */
    enum RangeResolution
    {
	RANGE_RES_AUTO = 0,
	RANGE_RES_FEMTO = 1,
	RANGE_RES_NANO = 2
    };

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxNetworkSendAsterix(void);
    virtual ~SPxNetworkSendAsterix();
    virtual SPxErrorCode Create(const char *addr=NULL,int port=0,const char *ifAddr=NULL);

    /* Type of network sender. */
    virtual UINT32 GetProjectCode(void) const { return 240; }

    /* Configuration functions. */
    SPxErrorCode SetSIC(UINT8 sic);
    UINT8 GetSIC(void) const { return m_sic; }

    SPxErrorCode SetSAC(UINT8 sac);
    UINT8 GetSAC(void) const { return m_sac; }

    SPxErrorCode SetSummaryText(const char *text);
    SPxErrorCode GetSummaryText(char *bufPtr, unsigned int bufSizeBytes) const;
       
    SPxErrorCode SetSummaryIntervalMsecs(unsigned int msecs);
    unsigned int GetSummaryIntervalMsecs(void) const
    {
        return m_summaryIntervalMsecs;
    } 

    SPxErrorCode SetSummaryAlwaysEnabled(int isEnabled);
    int IsSummaryAlwaysEnabled(void) const { return m_isSummaryAlwaysEnabled; }

    SPxErrorCode SetAzimuthLimitDegs(double degs);
    double GetAzimuthLimitDegs(void) const { return m_aziLimitDegs; }

    SPxErrorCode SetSkipBackJumpMaxDegs(double degs);
    double GetSkipBackJumpMaxDegs(void) const { return m_skipBackJumpMaxDegs; }

    SPxErrorCode SetRangeResolution(RangeResolution rangeRes);
    RangeResolution GetRangeResolution(void) const { return m_rangeRes; }

    SPxErrorCode SetSPFEnabled(int isEnabled);
    int IsSPFEnabled(void) const { return m_isSPFEnabled; }

    /* Override the base class function used to set encode format. */
    virtual void SetEncodeFormat(int format);
    virtual int IsEncodeFormatSupported(int format) const;

    /* Parameter setting functions. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected functions.
     */
    void Shutdown(void);

    /* Overwrite the base class encode and send functions. */
    unsigned char *EncodeReturn(const SPxReturn_tag *rtn,
					unsigned char *destBuf,
					unsigned int destBufSizeBytes,
					unsigned int *encodedSizeBytesPtr,
					unsigned int *formatUsedPtr);

    int SendReturn(const SPxReturnHeader_tag *rtnHeader,
                   unsigned char *srcData,
                   unsigned int numBytes,
                   unsigned int formatUsed);

    /* Function to send data over the network. May be overridden for testing. */
    virtual SPxErrorCode SendData(const char *data,
				  int mesgSizeBytes,
				  const char *errorMessage);

    /*
     * Protected fields.
     */

private:

    /*
     * Private fields.
     */
    SPxCriticalSection m_infoMutex;

    /* Configurable items. */
    UINT8 m_sac;                       /* System Area Code. */
    UINT8 m_sic;                       /* System Identification Code. */
    unsigned int m_summaryIntervalMsecs; /* Summary mesg interval in ms. */
    double m_aziLimitDegs;             /* Azimuth sector size limit. */
    double m_skipBackJumpMaxDegs;      /* Skip backwards jumps up to this size. */
    RangeResolution m_rangeRes;	       /* Range resolution type. */
    int m_isSPFEnabled;                /* Is custom SPx SPF enabled? */

    /* Video Summary Message packing buffer. */
    UINT8 *m_summaryBuf;                /* Buffer for packing summary msg. */
    unsigned int m_summaryBufSize;      /* Size of mesg1 buffer in bytes. */
    SPxThread *m_summaryThread;         /* Thread used to send summary msg. */
    UINT32 m_lastSummaryTimeMsecs;      /* Time last mesg1 was sent. */
    int m_isSummaryAlwaysEnabled;       /* Output even when source disabled?*/

    /* Video Message packing buffer. */
    UINT8 *m_videoBuf;                /* Buffer for packing video msg. */
    unsigned int m_videoBufSize;      /* Size of video buffer in bytes. */
    UINT8 *m_videoDataStartPtr;           /* Start of video data. */ 
    unsigned int m_videoDataSizeBytes;    /* Size of video data area. */

    /* Internal data. */
    UINT32 m_mesgIndex;               /* Message sequence number. */
    UINT16 m_lastAzimuth;             /* Last azimuth index. */
    UINT16 m_lastAziExtent;           /* Last azimuth extent. */
    char *m_summaryText;              /* Copy of summary text. */
    UINT32 m_lastRangeResErrTime;     /* Time of last range resolution error. */

    /*
     * Private functions.
     */

    SPxErrorCode sendSummaryMesg(void);
    UINT32 get128thSecsSinceMidnight(SPxTime_tag *timestamp);
    unsigned int getVideoMesgSizeBytes(unsigned int numBytes,
                                       int includeTod=TRUE,
                                       int includeSPF=FALSE);

    /*
     * Private static functions.
     */

    static void *summaryMesgSender(SPxThread *thread);

}; /* SPxNetworkSendAsterix class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NET_SEND_ASTERIX_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
