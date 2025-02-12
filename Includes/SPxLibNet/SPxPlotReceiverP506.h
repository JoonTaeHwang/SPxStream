/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxPlotReceiverP506 class, for receiving external
 *   plots on the network in P506 format.
 *
 * Revision Control:
 *   13/10/22 v1.6   SP 	Changes based on new information.
 *
 * Previous Changes:
 *   24/05/21 1.5   SP 	Improve sorting compare function.
 *   21/05/21 1.4   SP 	Fix flushing of sectors.
 *   20/05/21 1.3   SP 	Add internal test source.
 *   19/05/21 1.2   SP 	Further development.
 *   19/05/21 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_PLOT_RECEIVER_P506_H
#define _SPX_PLOT_RECEIVER_P506_H

/*
 * Other headers required.
 */
/* We need some library headers for common types, functions, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxPlotReceiver.h"
#include "SPxLibNet/SPxNetAddr.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/


/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/


/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Forward declare classes we need. */
class SPxPacketDecoder;
class SPxPacketDecoderNet;
class SPxAsterixDecoder;
struct sockaddr_in;
struct SPxTime_tag;

/*
 * Define our class, derived from the generic base class.
 */
class SPxPlotReceiverP506 : public SPxPlotReceiver, public SPxNetAddr
{
public:
    /*
     * Public members.
     */
    /* Constructor and destructor. */
    explicit SPxPlotReceiverP506(void);
    virtual ~SPxPlotReceiverP506(void);

    /* SPxNetAddr interface. */
    virtual SPxErrorCode SetAddress(const char *address, int port=0,
                                    const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0,
                                    const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;

    /* Generic parameter assignment. */
    int SetParameter(char *name, char *value);
    int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /*
     * Protected members.
     */

private:

    /*
     * Private types.
     */

    /* Data header. Note that this structure will be padded by
     * the complier. Therefore we cannot memcpy() to it or
     * use sizeof() - use HEADER_SIZE_BYTES instead.
     */
    static const UINT32 HEADER_SIZE_BYTES = 7; /* KEEP IN SYNC with Header_t. */
    struct Header_t
    {
        UINT16 magic;       /* Set to HEADER_MAGIC. */
        UINT32 numBytes;    /* Size of message after this field. */
        UINT8 dataType;     /* DATA_TYPE_xxx */
    };

    /* Header magic value. */
    static const UINT16 HEADER_MAGIC = 0x5E5E;

    /* Data types. Note that these values are incorrectly 
     * swapped in the spec!
     */
    static const UINT8  DATA_TYPE_TRACK  = 0x01;
    static const UINT8  DATA_TYPE_PLOT   = 0x02;

    /* Plot data. Note that this structure will be padded by
     * the complier. Therefore we cannot memcpy() to it or
     * use sizeof() - use PLOT_SIZE_BYTES instead.
     */
    static const UINT32 PLOT_SIZE_BYTES = 18; /* KEEP IN SYNC with Plot_t. */
    struct Plot_t
    {
        UINT32 time;            /* Timestamp. */
        REAL32 rangeKm;         /* Range in km. */
        REAL32 azimuthDegs;     /* Azimuth in degrees. */
        UINT16 reserved1;       /* Not used? */
        REAL32 amplitude;       /* Amplitude of plot. */
    };

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxCriticalSection m_mutex;     /* Mutex for object. */
    SPxPacketDecoderNet *m_pdn;     /* Generic decoder object */
    int m_isDataBigEndian;          /* Is received data big endian? */
    UINT32 m_plotWeight;            /* Plot weight (number of samples). */
    SPxThread *m_thread;            /* Background thread. */
    UINT32 m_lastTestMesgTime;      /* Time of last test message. */
    UINT8 *m_testBuf;               /* Test message buffer. */
    int m_isTestModeEnabled;        /* Test mode enabled? */
    double m_testRangeKm;           /* Range of test targets. */

    /*
     * Private functions.
     */

    SPxErrorCode handlePacket(struct sockaddr_in *fromAddr,
                              const SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    SPxErrorCode handlePlots(const SPxTime_t *timestamp,
                             const unsigned char *payload,
                             unsigned int numBytes);

    SPxErrorCode handlePlot(const SPxTime_t *timestamp,
                            unsigned int index, 
                            const Plot_t *plot);

    void byteSwapToHost(UINT16 *val);
    void byteSwapToHost(UINT32 *val);
    void byteSwapToHost(REAL32 *val);
    void byteSwapToData(UINT16 *val);
    void byteSwapToData(UINT32 *val);
    void byteSwapToData(REAL32 *val);

    const UINT8 *getNextTestMesg(SPxTime_t *timestampRtn, 
                                 unsigned int *numBytesRtn);

    /*
     * Private static functions.
     */

    static void packetHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *fromAddr,
                              struct SPxTime_tag *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

    static void *threadHandler(SPxThread *thread);

    static bool comparePlots(const Plot_t &p1, const Plot_t &p2);

}; /* SPxPlotReceiverP506 */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_PLOT_RECEIVER_P506_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
