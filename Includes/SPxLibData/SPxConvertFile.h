/*********************************************************************
 *
 * (c) Copyright 2020 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  Header for CPR file conversion functions.
 *
 * Revision Control:
 *  25/01/21 v1.2    SP 	Support Ethernet PCAP format.
 *
 * Previous Changes:
 *  17/11/20 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CONVERT_FILE_H
#define _SPX_CONVERT_FILE_H

#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxChannel.h"

/*
 * Public types.
 */

/* PCAP file format.
 * See here: https://www.tcpdump.org/linktypes.html
 */
enum SPxPCAPFormat_t
{
    SPX_PCAP_FORMAT_RAW        = 1,     /* DLT_RAW (No physical link info). */
    SPX_PCAP_FORMAT_ETHERNET   = 2      /* DLT_EN10MB (As used by Wireshark for captures). */
};

/* A channel in a CPR file to convert. */
struct SPxConvertFileChannel_t
{
    UINT8 chanID;       /* Channel ID. */
    UINT32 srcAddr;     /* Source address. */
    UINT16 srcPort;     /* Source port. */
    UINT32 destAddr;    /* Destination address. */
    UINT16 destPort;    /* Destination port. */
};

/* Progress callback. */
typedef SPxErrorCode (*SPxConvertFileStatusFn)(void *userArg, double percent);

/*
 * Public functions.
 */

/* Conversion to PCAP. */
extern int SPxIsPCAPSupported(char *buffer=NULL, int bufLen=0);
extern int SPxIsPCAPChannelSupported(SPxChannel::Type_t type,
                                     SPxChannel::Media_t media);
extern int SPxIsPCAPPacketSupported(UINT32 packetType);
extern SPxErrorCode SPxConvertFileToPCAP(SPxPCAPFormat_t format,
                                         const char *srcFilename,
                                         const char *dstFilename,
                                         const SPxConvertFileChannel_t channels[],
                                         unsigned numChannels,
                                         SPxConvertFileStatusFn statusFn=NULL,
                                         void *statusUserArg=NULL);

#endif /* _SPX_CONVERT_FILE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
