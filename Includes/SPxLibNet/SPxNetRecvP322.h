/*********************************************************************
*
* (c) Copyright 2016 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for project-322 network receiving module.
*
*
* Revision Control:
*   20/07/18 v1.4    AGC	Move discovery to SPxNetClientP322.
*
* Previous Changes:
*   25/10/17 1.3    AGC	Make handle...() functions protected for testing.
*   23/10/17 1.2    AGC	Move common defines to private header.
*   29/01/16 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECVP322_H
#define _SPX_NETRECVP322_H

/*
 * Other headers required.
 */
/* We need the base class header (which includes several others as well). */
#include "SPxLibNet/SPxNetRecv.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * Define our project-specific network receiver class, derived from the
 * standard one.
 */
class SPxNetworkReceiveP322 :public SPxNetworkReceive
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxNetworkReceiveP322(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveP322();

    /* Overwrite the base class functions we want to handle ourselves. */
    virtual int ThreadHandleDataFn(unsigned char *data,
				unsigned int dataSizeBytes);

    /* Configuration functions. */
    SPxErrorCode SetForce8BitOutput(int force);
    int GetForce8BitOutput(void) const { return(m_force8BitOutput); }

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /* Functions that may be called for testing. */
    SPxErrorCode handleVideoPacket(UINT32 now, const unsigned char *data,
				   unsigned int dataSizeBytes);

private:
    /*
     * Private fields.
     */
    /* Configuration options. */
    int m_force8BitOutput;		/* Use 8-bit output even if 12 in? */

    /* Buffer for unpacking data. */
    unsigned char *m_decodeBuf;			/* Buffer */
    unsigned int m_decodeBufSizeBytes;		/* Allocated size of buffer */

    /* PRF estimation. */
    UINT32 m_prfRefTimeMsecs;			/* Reference time */
    unsigned int m_prfSpokesSinceLastRef;	/* Counter */
    double m_prfEstimateHz;			/* Estimated PRF */
    unsigned int m_prfUsecsPerSpoke;		/* Inter-spoke timing */

    /*
     * Private functions.
     */

    /* Data handling. */
    SPxErrorCode extractSamples(const unsigned char *data,
					unsigned int numPackedBytes,
					unsigned int numSamples,
					unsigned int bitsPerSample,
					unsigned char *decodeBuf,
					unsigned int decodeBufSizeBytes,
					unsigned char packing);

}; /* SPxNetworkReceiveP322 class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECVP322_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
