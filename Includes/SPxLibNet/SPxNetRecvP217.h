/*********************************************************************
*
* (c) Copyright 2012, 2013, 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetRecvP217.h,v $
* ID: $Id: SPxNetRecvP217.h,v 1.4 2015/03/06 14:19:08 rew Exp $
*
* Purpose:
*	Header for project-217 network receiving module.
*
*
* Revision Control:
*   06/03/15 v1.4    REW	Start support for FASTLZ mode.
*
* Previous Changes:
*   25/04/13 1.3    AGC	Use new SPxCompress class.
*   26/09/12 1.2    REW	More access functions.
*   19/09/12 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECVP217_H
#define _SPX_NETRECVP217_H

/*
 * Other headers required.
 */
/* We need the base class header (which includes several others as well). */
#include "SPxLibNet/SPxNetRecv.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Options for video types.
 * Note that not all are supported by older protocols.
 */
#define	SPX_NET_RECV_P217_VIDEOTYPE_ZLIB_8	2	/* Protocol 2.0 */
#define	SPX_NET_RECV_P217_VIDEOTYPE_ZLIB_6	7	/* Protocol 2.2 */
#define	SPX_NET_RECV_P217_VIDEOTYPE_ZLIB_5	8	/* Protocol 2.2 */
#define	SPX_NET_RECV_P217_VIDEOTYPE_RAW_8	9	/* Protocol 2.0 */
#define	SPX_NET_RECV_P217_VIDEOTYPE_ZLIB_4	10	/* Protocol 2.2 */
#define	SPX_NET_RECV_P217_VIDEOTYPE_ZLIB_3	11	/* Protocol 2.2 */
#define	SPX_NET_RECV_P217_VIDEOTYPE_FASTLZ	13	/* Protocol 2.2 */

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

/*
 * Define our project-specific network receiver class, derived from the
 * standard one.
 */
class SPxNetworkReceiveP217 :public SPxNetworkReceive
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxNetworkReceiveP217(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveP217();

    /* Overwrite the base class functions we want to handle ourselves. */
    virtual int ThreadSocketChangedFn(void);
    virtual int ThreadReadDataFn(unsigned char *buf,
				unsigned int bufSizeBytes,
				unsigned int timeoutMsecs);
    virtual int ThreadHandleDataFn(unsigned char *data,
				unsigned int dataSizeBytes);

    /* Configuration functions. */
    SPxErrorCode SetVideoType(unsigned int videoType);
    unsigned int GetVideoType(void) const { return(m_videoType); }
    SPxErrorCode SetWatchdogMsecs(unsigned int msecs)
    {
	m_dataWatchdogMsecs = msecs;
	return(SPX_NO_ERROR);
    }
    unsigned int GetWatchdogMsecs(void)	{ return(m_dataWatchdogMsecs); }

    /* Status functions.  */
    unsigned int GetProtocolMajor(void)	const { return(m_protocolMajor); }
    unsigned int GetProtocolMinor(void)	const { return(m_protocolMinor); }
    unsigned int GetNumAzis(void) const { return(m_numAzis); }
    unsigned int GetNumSamples(void) const { return(m_nominalLength); }
    REAL32 GetEndRangeMetres(void) const { return(m_rangeOfLastSample); }

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */
    /* Protocol (only once connected). */
    unsigned int m_protocolMajor;	/* Major protocol version (e.g. 2) */
    unsigned int m_protocolMinor;	/* Minor version (e.g. 0, 2) */

    /* Resolution etc. */
    unsigned int m_numAzis;		/* Number of azimuths per scan */
    unsigned int m_sectorSize;		/* Number of azimuths per sector */
    unsigned int m_numSectors;		/* Expected number of sectors */

    /* Configuration options. */
    unsigned int m_videoType;		/* Video type to request */

    /* PRF estimation. */
    UINT32 m_prfRefTimeMsecs;			/* Reference time */
    unsigned int m_prfSpokesSinceLastRef;	/* Counter */
    double m_prfEstimateHz;			/* Estimated PRF */
    unsigned int m_prfUsecsPerSpoke;		/* Inter-spoke timing */

    /* Watchdog support. */
    UINT32 m_timeOfLastGoodData;	/* Ticker when last processed */

    /* Buffer for pending control msgs. */
    char *m_ctrlBuf;			/* Buffer */
    unsigned int m_ctrlBufSizeBytes;	/* Size of buffer */
    unsigned int m_ctrlBufPendingBytes;	/* Number of bytes pending */

    /* Data socket etc. */
    SOCKET_TYPE m_dataSocket;			/* Socket handle */
    struct sockaddr_in m_dataSocketAddress;	/* Address structure */

    /* Support for re-assembly of fragmented packets. */
    struct SPxNetworkReceiveP217Fragment_tag *m_freeFragments;	/* List */
    struct SPxNetworkReceiveP217Fragment_tag *m_firstFragment;	/* List */
    unsigned int m_numFragmentsAllocated;	/* Count */

    /* Buffer for re-assembling data. */
    char *m_dataBuf;				/* Buffer */
    unsigned int m_dataBufSizeBytes;		/* Size of buffer */

    /* ZLIB decompression. */
    SPxCompress *m_compress;

    /* Buffer for decompressing data. */
    char *m_decompressBuf;			/* Buffer */
    unsigned int m_decompressBufSizeBytes;	/* Size of buffer */

    /*
     * Private functions.
     */
    int handleCtrlMsg(UINT32 now, const char *msg);
    int handleDataMsg(UINT32 now,
			struct SPxNetworkReceiveP217Fragment_tag *frag);
    int movePendingDataToFreeList(void);
    int processPendingData(UINT32 now);
    int setupDataSocket(const char *addr, unsigned int port);
    int closeDataSocket(void);
    int checkProtocolAndVideoType(void);
}; /* SPxNetworkReceiveP217 class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECVP217_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
