/*********************************************************************
*
* (c) Copyright 2017, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for P372 network receiving objects.
*
*
* Revision Control:
*   11/11/19 v1.3    AGC	Fix video type data race.
*
* Previous Changes:
*   28/11/17 1.2    AJH	Add GetVideoType().
*   22/11/17 1.1    AJH	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECV_P372_H
#define _SPX_NETRECV_P372_H

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
class SPxNetworkReceiveP372 :public SPxNetworkReceive
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxNetworkReceiveP372(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveP372();

    /* Override the base class functions we want to handle ourselves. */
    virtual int ThreadHandleDataFn(unsigned char *data,
					unsigned int dataSizeBytes);

    /* Configuration functions. */
    UINT8 GetVideoType(void) const { return static_cast<UINT8>(m_videoType); }

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */
    /* Config fields. */
    UINT32 m_packetNum;		/* Packet number */
    UINT32 m_packetNumPrev;	/* Last packet number */
    REAL32 m_periodSecs;	/* Radar period */
    REAL32 m_metresPerSample;	/* Sample size */
    REAL32 m_startRangeMetres;	/* Minimum range */
    REAL32 m_endRangeMetres;	/* Maximum range */
    UINT16 m_currAzimuth;	/* Azimuth of current sweep */
    UINT16 m_shipHead;		/* Ship's heading */
    int m_northReferenced;	/* Flag */
    SPxAtomic<UINT32> m_videoType; /* SPX_NET_P372_VIDEO_... */

    /* PRF estimation. */
    UINT32 m_prfRefTimeMsecs;			/* Reference time */
    unsigned int m_prfSpokesSinceLastRef;	/* Counter */
    double m_prfEstimateHz;			/* Estimated PRF */
    unsigned int m_prfUsecsPerSpoke;		/* Inter-spoke timing */
    
    /* CRC lookup table. */
    UINT8 m_crcTable[256];

    /*
     * Private functions.
     */
    SPxErrorCode handleMsgData(unsigned char *msg, unsigned int msgSizeBytes);
    void setupCRC8();
    UINT8 computeCRC8(unsigned char *data, unsigned int len);

}; /* SPxNetworkReceiveP372 class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECV_P372_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
