/*********************************************************************
*
* (c) Copyright 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetSendP372.h,v $
* ID: $Id: SPxNetSendP372.h,v 1.1 2017/11/22 11:17:34 rew Exp $
*
* Purpose:
*	Header for P372 network sending object.
*
*
* Revision Control:
*   22/11/17 v1.1    AJH	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_NET_SEND_P372_H
#define _SPX_NET_SEND_P372_H

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
#define	SPX_NET_P372_DEFAULT_PORT	31002

/*
 * Define the sizes of the headers.
 */
#define	SPX_NET_P372_SIZEOF_MSG_HDR	(11 * 4)

/*
 * Define the various ID values for the message header.
 */
#define SPX_NET_P372_VERSION		2U

#define	SPX_NET_P372_VIDEO_SEA_RAW	2U
#define	SPX_NET_P372_VIDEO_SEA_PROC	3U


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
class SPxNetworkSendP372 : public SPxNetworkSend
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxNetworkSendP372(void);
    virtual ~SPxNetworkSendP372();

    /* Type of network sender. */
    virtual UINT32 GetProjectCode(void) const { return 372; }

    /* Configuration functions. */
    SPxErrorCode SetVideoType(UINT8 videoType);
    UINT8 GetVideoType(void) const		{ return(m_videoType); }

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
    unsigned char *m_p372buffer;	/* Buffer for building messages */
    unsigned int m_p372bufSizeBytes;	/* Size of buffer in bytes */

    /* Convenience pointers into buffer. */
    unsigned char *m_p372data;		/* Start of data payload */
    unsigned int m_p372dataSize;	/* Size of data area in bytes */

    /* Values for the stream. */
    UINT8 m_videoType;			/* SPX_NET_P372_VIDEO_... */
    UINT32 m_packetNum;			/* Packet number */
    unsigned int m_azimuthDim;		/* Manually specified azimuth dimension. */

    /* CRC lookup table. */
    UINT8 m_crcTable[256];

    /*
     * Private functions.
     */
    virtual SPxErrorCode sendPacket(void *data, unsigned int numBytes);
    void setupCRC8();
    UINT8 computeCRC8(unsigned char *data, unsigned int len);

}; /* SPxNetworkSendP372 class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NET_SEND_P372_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
