/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for P427 network receiving object.
*
* Revision Control:
*   08/05/19 v1.7    SP 	Use mutex in base class.
*
* Previous Changes:
*   07/05/19 1.6    SP 	Improve PRF estimation.
*   25/04/19 1.5    SP 	Fix return count and time interval.
*   16/04/19 1.4    SP 	Don't make SPxNetP427 virtual.
*   15/04/19 1.3    SP 	Derive from SPxNetP427.
*   12/04/19 1.2    SP 	Support auto scan mode.
*   10/04/19 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_NET_RECV_P427_H
#define _SPX_NET_RECV_P427_H

/*
 * Other headers required.
 */

/* For SPxReturnHeader. */
#include "SPxLibData/SPxRib.h"

/* We need the base class headers. */
#include "SPxLibNet/SPxNetP427.h"
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

/* Forward declare any classes we need. */

/*
 * Define our project-specific network receiver class, derived from the
 * standard one.
 */
class SPxNetworkReceiveP427 : public SPxNetworkReceive, public SPxNetP427
{
public:

    /*
     * Public types.
     */

    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxNetworkReceiveP427(SPxRIB *air1Rib, 
                   SPxRIB *air2Rib, 
                   SPxRIB *sur1Rib, 
                   SPxRIB *sur2Rib);
    virtual ~SPxNetworkReceiveP427(void);

    virtual SPxErrorCode SetScanMode(SPxScanMode scanMode);
    virtual SPxScanMode GetScanMode(void) { return m_scanMode; }
    virtual SPxScanMode GetAutoScanMode(void) { return m_autoScanMode; }

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected functions.
     */

    /* Overwrite the base class functions we want to handle ourselves. */
    int ThreadHandleDataFn(unsigned char *data,
                           unsigned int dataSizeBytes);

    /*
     * Protected fields.
     */

private:

    /*
     * Private types.
     */

    /* Stream specific data. */
    typedef struct
    {
        /* General. */
        StreamID_t streamID;                    /* ID of this stream. */
        SPxNetworkReceive::Stream_t context;    /* Stream context used in base class. */
        UINT32 intervalStart;                   /* Interval start time. */
        UINT32 nominalNumSamplesOverInterval;   /* Used to calculate nominal length. */
        UINT16 nominalNumSamples;               /* Nominal length in samples. */
        UINT32 countOverInterval;               /* Number of returns during interval. */
        UINT32 avReturnIntervalUsecs;           /* Average return interval in microseconds. */
        UINT16 count;                           /* Total return count. */

        /* Information from the most recent return. */
        UINT16 startAziNum;                 /* Latest start azimuth number received. */
        UINT16 endAziNum;                   /* Latest end azimuth number received. */
        UINT32 numSamples;                  /* Latest number samples received. */
        UINT32 sampleMetres;                /* Latest sample size in metres. */
        double beamElevationDegs;           /* Latest beam elevation in degrees. */

        /* Information from previous return. */
        UINT16 prevStartAziNum;             /* Previous start azimuth number received. */

    } Stream_t;

    /*
     * Private fields.
     */

    Stream_t m_streams[MAX_NUM_STREAMS];    /* Streams. */
    SPxReturnHeader m_rtnHdr;               /* Return header. */
    SPxScanMode m_scanMode;                 /* Required scan mode. */
    SPxScanMode m_autoScanMode;             /* Auto detected scan mode. */
    unsigned int m_numRotatingRtns;         /* Number of consecutive rotating scan returns. */
    unsigned int m_numRandomRtns;           /* Number of consecutive random scan returns. */

    /*
     * Private functions.
     */

    SPxErrorCode handleMessage(unsigned char *data,
                               unsigned int dataSizeBytes);

    Stream_t *getStreamFromHeader(UINT32 *paramAndValuePtr, UINT16 numParams);

    SPxErrorCode handleStreamParam(Stream_t *stream, UINT32 paramAndValue);

    void updateStream(Stream_t *stream);

    void updateAutoScanMode(Stream_t *stream);

}; /* SPxNetworkReceiveP427 class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NET_RECV_P427_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
