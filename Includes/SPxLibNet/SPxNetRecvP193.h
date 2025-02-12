/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetRecvP193.h,v $
* ID: $Id: SPxNetRecvP193.h,v 1.5 2017/03/08 14:43:29 rew Exp $
*
* Purpose:
*	Header for P193 network receiving objects.
*
*
* Revision Control:
*   08/03/17 v1.5    AGC	Make source ID and num azis atomic.
*
* Previous Changes:
*   29/11/16 1.4    AGC	Allow retrieval of source ID and num azis.
*   17/03/15 1.3    AGC	Remove nominal length (accessible from base class).
*   04/10/13 1.2    AGC	Simplify headers.
*   07/02/13 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECV_P193_H
#define _SPX_NETRECV_P193_H

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
class SPxRIB;

/*
 * Define our project-specific network receiver class, derived from the
 * standard one.
 */
class SPxNetworkReceiveP193 :public SPxNetworkReceive
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxNetworkReceiveP193(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveP193();

    UINT16 GetSourceID(void) const { return static_cast<UINT16>(m_sid); }
    UINT16 GetNumAzis(void) const { return static_cast<UINT16>(m_numAzis); }

    /* Override the base class functions we want to handle ourselves. */
    virtual int ThreadHandleDataFn(unsigned char *data,
					unsigned int dataSizeBytes);

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */
    /* Config fields. */
    int m_configIsOkay;		/* Flag */
    UINT32 m_ssn;		/* Scan Sequence Number */
    REAL32 m_periodSecs;	/* Radar Period */
    REAL32 m_metresPerSample;	/* Sample size */
    REAL32 m_endRangeMetres;	/* Max range */
    SPxAtomic<UINT32> m_numAzis;/* Number of azimuths per scan */
    UINT8 m_compression;	/* Compressiong algorithm */
    UINT8 m_bytesPerSample;	/* Data size */
    UINT8 m_packing;		/* Channel packing */
    UINT8 m_channelsInUse;	/* Bitmask */
    int m_northReferenced;	/* Flag */
    SPxAtomic<UINT32> m_sid;	/* Source ID (received from sender) */

    /* Decoding fields. */
    int m_partialSen;			/* Sector number in progress */
    unsigned char *m_partialBuf;	/* In-progress buffer */
    unsigned int m_partialBufSizeBytes;	/* Size of buffer */
    unsigned int m_partialBufMaxSample;	/* Highest sample count so far */

    /* PRF estimation. */
    UINT32 m_prfRefTimeMsecs;			/* Reference time */
    unsigned int m_prfSpokesSinceLastRef;	/* Counter */
    double m_prfEstimateHz;			/* Estimated PRF */
    unsigned int m_prfUsecsPerSpoke;		/* Inter-spoke timing */

    /*
     * Private functions.
     */
    SPxErrorCode handleMsgParams(unsigned char *msg,unsigned int msgSizeBytes);
    SPxErrorCode handleMsgData(unsigned char *msg, unsigned int msgSizeBytes);
    SPxErrorCode outputSector(int sector, unsigned char *data,
					unsigned int numSamples);
}; /* SPxNetworkReceiveP193 class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECV_P193_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
