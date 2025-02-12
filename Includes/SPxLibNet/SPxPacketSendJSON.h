/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPacketSenderJSON class.
*
*	This class is used to send JSON packets across the network.
*
* Revision Control:
*   17/07/20 v1.1    SP 	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#ifndef _SPX_PACKET_SEND_JSON_H
#define _SPX_PACKET_SEND_JSON_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibNet/SPxPacketSend.h"

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

/* Forward-declare classes we may use. */
class SPxCompress;

/*
 * Define our class.
 */
class SPxPacketSenderJSON :public SPxPacketSender
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxPacketSenderJSON(int isTCP=FALSE, SOCKET_TYPE sock=SPX_SOCK_INVALID);
    virtual ~SPxPacketSenderJSON(void);

    /* Sending. */
    virtual SPxErrorCode SendPacketBJSON(UINT16 packetType,
				SPxTime_t ts,
				const void *payload,
				unsigned int payloadSizeBytes,
				int mutexAlreadyLocked=FALSE);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

    /* Buffers. */
    unsigned char *m_sendBuffer;	/* Send buffer. */
    unsigned int m_sendBufferSizeBytes;	/* Size of send buffer. */
    unsigned char *m_zlibBuffer;	/* ZLIB compressed buffer. */
    unsigned int m_zlibBufferSizeBytes;	/* Size of compressed buffer. */
    SPxCompress *m_zlib;		/* ZLIB compressor. */

    /*
     * Private functions.
     */
    SPxErrorCode ensureSendBufferBigEnough(unsigned int sizeBytes);
    SPxErrorCode ensureZLibBufferBigEnough(unsigned int sizeBytes);

}; /* SPxPacketSenderJSON */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_PACKET_SEND_JSON_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
