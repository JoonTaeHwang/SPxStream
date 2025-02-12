/*********************************************************************
*
* (c) Copyright 2020, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for project-489 network receiving module.
*
*
* Revision Control:
*   05/01/21 v1.2    AGC	Support min/max beam altitude angles.
*
* Previous Changes:
*   18/12/20 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECVP489_H
#define _SPX_NETRECVP489_H

/*
 * Other headers required.
 */
/* We need the base class header. */
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

/* Forward declare any classes we use. */
class SPxRIB;
class SPxNetTCP;
class SPxCallbackList;

/*
 * Define our specific network receiver class, derived from the
 * standard one.
 */
class SPxNetworkReceiveP489 :public SPxNetworkReceive
{
public:
    /* Constructor, destructor etc. */
    explicit SPxNetworkReceiveP489(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveP489(void);

    /* Configuration. */
    SPxErrorCode SetNumSamples(unsigned int numSamples);
    unsigned int GetNumSamples(void) const;
    SPxErrorCode SetNumAzimuths(unsigned int numAzimuths);
    unsigned int GetNumAzimuths(void) const;
    SPxErrorCode SetEndRangeMetres(double endRangeMetres);
    double GetEndRangeMetres(void) const;
    SPxErrorCode SetGain(double gain);
    double GetGain(void) const;
    SPxErrorCode SetMinBeamAltAngle(double minBeamAltAngle);
    double GetMinBeamAltAngle(void) const;
    SPxErrorCode SetMaxBeamAltAngle(double maxBeamAltAngle);
    double GetMaxBeamAltAngle(void) const;

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */

    /* Overwrite the base class functions we want to handle ourselves. */
    virtual int ThreadHandleDataFn(unsigned char *data,
				   unsigned int dataSizeBytes);

    /* Radar video functions. */
    SPxErrorCode parsePacket(const unsigned char *data);

}; /* SPxNetworkReceiveP489 class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECVP489_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
