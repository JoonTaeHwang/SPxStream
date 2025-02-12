/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxDFReceiverCRFS, a specific CRFS Direction Finder class.
 *
 * Revision Control:
 *  27/02/18 v1.6    AGC	Add ShutdownProtobuf().
 *
 * Previous Changes:
 *  22/02/18 1.5    AGC	Support Linux.
 *  22/02/18 1.4    REW	Support access to detections.
 *  22/02/18 1.3    REW	Move decoding to separate functions.
 *  20/02/18 1.2    REW	Convert from SDK to raw TCP connection.
 *  20/02/18 1.1    REW	Initial Version.
 *********************************************************************/

#ifndef _SPX_DF_RECEIVER_CRFS_H
#define _SPX_DF_RECEIVER_CRFS_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Other classes that we need. */
#include "SPxLibNet/SPxDFReceiver.h"	/* Our base class */

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
class SPxNetTCP;

/*
 * Define our class.
 */
class SPxDFReceiverCRFS : public SPxDFReceiver
{
public:
    /*
     * Public types.
     */
    typedef struct TBearing_tag
    {
	UINT16 angle;		/* Radians * 1000 */
	UINT8 confidence;	/* 0 (least) to 255 (most confident) */
	UINT8 amplitude;	/* Amplitude */
    } TBearing;

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxDFReceiverCRFS(void);
    virtual ~SPxDFReceiverCRFS(void);

    /* Override SPxObj enable state. */
    virtual void SetActive(int active);

    /* Note various configuration functions in base class.  */

    /* Search actions. */
    virtual SPxErrorCode Scan(void);

    /* Debug. */
    SPxErrorCode SetDebugLevel(unsigned int debugLevel)
    {
	m_debug = debugLevel;
	return(SPX_NO_ERROR);
    }
    unsigned int GetDebugLevel(void) const { return(m_debug); }

    static void ShutdownProtobuf(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected variables.
     */

private:
    /*
     * Private variables.
     */
    /* TCP stream. */
    SPxNetTCP *m_stream;

    /* Buffers. */
    unsigned char *m_recvBuffer;		/* Buffer, if allocated */
    unsigned int m_recvBufferSizeBytes;		/* Allocated size */

    /* Timeout for various commands. */
    UINT32 m_timeoutMsecs;

    /* Decoding. */
    int m_msgStreamGUIDSet;
    char m_msgStreamGUID[128];
    int m_msgBearingSet;
    double m_msgBearing;
    int m_msgLatDegsSet;
    double m_msgLatDegs;
    int m_msgLongDegsSet;
    double m_msgLongDegs;

    /* Debug. */
    unsigned int m_debug;			/* Debug level, 0, 1 etc. */

    /*
     * Private functions.
     */
    SPxErrorCode connect(void);
    SPxErrorCode disconnect(void);

    /* Message handling. */
    SPxErrorCode decodeMessage(unsigned char *buf, unsigned int numBytes);
    SPxErrorCode processElementCollection(void *coll);
    SPxErrorCode getValFromElement(void *ele, double *valPtr);

}; /* SPxDFReceiverCRFS */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_DF_RECEIVER_CRFS_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
