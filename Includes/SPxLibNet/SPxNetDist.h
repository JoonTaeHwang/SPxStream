/*********************************************************************
*
* (c) Copyright 2007, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetDist.h,v $
* ID: $Id: SPxNetDist.h,v 1.2 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*	Header for common definitions across network sending/receiving
*	modules.
*
*
* Revision Control:
*   04/10/13 v1.2    AGC	Simplify headers.
*
* Previous Changes:
*   26/07/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETDIST_H
#define _SPX_NETDIST_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxTypes.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Magic number for network distribution header.  "SPXN" is 0x5350584E.
 */
#define	SPX_NETDIST_MAGIC	(('S'<<24) | ('P'<<16) | ('X'<<8) | 'N')


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
 * Define the header that goes on the start of each packet sent on the
 * network, which may be chunks of a larger message or may be messages
 * in their own right.
 *
 * All multi-byte fields should be big-endian (network) format.
 */
typedef struct SPxNetChunkHeader_tag {
    /* Word 1 */
    UINT32 magic;			/* SPX_NETDIST_MAGIC */

    /* Word 2 */
    UINT8 protocolVersion;		/* 1 */
    UINT8 headerSize;			/* Size of this header */
    UINT8 numChunks;			/* Number of packets in this msg */
    UINT8 thisChunk;			/* Chunk number, 0 to numChunks-1 */

    /* Word 3 */
    UINT16 sequenceNumber;		/* Sequence number for message */
    UINT16 thisPayloadSize;		/* Size in bytes of this payload */

    /* Word 4 */
    UINT32 totalPayloadSize;		/* Total size of all payloads */

    /* Word 5 */
    UINT32 thisPayloadOffset;		/* Offset within complete message */

    /* Word 6 */
    UINT32 sourceInfo;			/* Server-assigned code/ID */
} SPxNetChunkHeader;


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETDIST_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
