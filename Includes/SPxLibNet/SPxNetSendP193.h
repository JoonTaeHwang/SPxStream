/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetSendP193.h,v $
* ID: $Id: SPxNetSendP193.h,v 1.6 2017/10/18 13:36:36 rew Exp $
*
* Purpose:
*	Header for P193 network sending object.
*
*
* Revision Control:
*   18/10/17 v1.6    AGC	Add IsEncodeFormatSupported().
*
* Previous Changes:
*   29/11/16 1.5    AGC	Add control for Source ID.
*   10/12/15 1.4    AGC	Add virtual GetProjectCode().
*   27/08/15 1.3    REW	Add control for CPS.
*   04/10/13 1.2    AGC	Simplify headers.
*   07/02/13 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NET_SEND_P193_H
#define _SPX_NET_SEND_P193_H

/*
 * Other headers required.
 */

/* We need the base class header (which includes several others as well). */
#include "SPxLibNet/SPxNetSend.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Define the default port number.
 */
#define	SPX_NET_P193_DEFAULT_PORT	41552

/*
 * Define the sizes of the headers.
 */
#define	SPX_NET_P193_SIZEOF_MSG_HDR		8
#define	SPX_NET_P193_SIZEOF_INFO_HDR		23
#define	SPX_NET_P193_SIZEOF_SPOKE_HDR		12

/*
 * Define the various ID values for the main message headers.
 */
#define	SPX_NET_P193_FID		0x52
#define	SPX_NET_P193_PID		0x53
#define	SPX_NET_P193_EID		0x53
#define	SPX_NET_P193_MID_RSP		0x01
#define	SPX_NET_P193_MID_RSD		0x02
#define	SPX_NET_P193_VMA		0x01
#define	SPX_NET_P193_VMI		0x01

/*
 * Define the various ID values for the spoke header.
 */
#define	SPX_NET_P193_STY_SINGLE		64
#define	SPX_NET_P193_STY_BLANK		128


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
struct SPxReturn_tag;
struct SPxReturnHeader_tag;

/*
 * Define our project-specific network sender class, derived from the
 * standard one.
 */
class SPxNetworkSendP193 : public SPxNetworkSend
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxNetworkSendP193(void);
    virtual ~SPxNetworkSendP193();

    /* Type of network sender. */
    virtual UINT32 GetProjectCode(void) const { return 193; }

    /* Configuration functions. */
    SPxErrorCode SetCPS(UINT8 cps);
    UINT8 GetCPS(void) const		{ return(m_cps); }
    SPxErrorCode SetSourceID(UINT16 sourceID);
    UINT16 GetSourceID(void) const	{ return(m_sourceID); }

    SPxErrorCode SetAzimuthDim(unsigned int azimuthDim);

    virtual int IsEncodeFormatSupported(int format) const;

    /* Parameter setting functions. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected functions.
     */

    /* Overwrite the base class encode and send functions. */
    unsigned char *EncodeReturn(const SPxReturn_tag *rtn,
					unsigned char *destBuf,
					unsigned int destBufSizeBytes,
					unsigned int *encodedSizeBytesPtr,
					unsigned int *formatUsedPtr);
    int SendReturn(const SPxReturnHeader_tag *rtnHeader,
                   unsigned char *data,
                   unsigned int numBytes,
                   unsigned int formatUsed);

    /*
     * Protected fields.
     */

private:

    /*
     * Private fields.
     */
    /* Buffer. */
    unsigned char *m_p193buffer;	/* Buffer for building messages */
    unsigned int m_p193bufSizeBytes;	/* Size of buffer in bytes */

    /* Convenience pointers into buffer. */
    unsigned char *m_p193data;		/* Start of data payload */
    unsigned int m_p193dataSize;	/* Size of data area in bytes */

    /* Values for the stream. */
    unsigned int m_azimuthDim;		/* Manually specified azimuth dimension. */
    UINT32 m_ssn;			/* Scan Sequence Number */
    UINT8 m_cps;			/* Channels Per Sample indicator */
    UINT16 m_sourceID;			/* Source ID */

    /* Period measurement etc. */
    UINT16 m_lastAzi;			/* Last azimuth. */
    UINT32 m_lastNorthTime;		/* Ticker for last north crossing */
    UINT32 m_periodMsecs;		/* Measured period */

    /* Status etc. */
    int m_warnedAboutZeroRange;		/* Flag */

    /*
     * Private functions.
     */
    SPxErrorCode sendInfo(const SPxReturn_tag *rtn);
    virtual SPxErrorCode sendPacket(void *data, unsigned int numBytes);

}; /* SPxNetworkSendP193 class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NET_SEND_P193_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
