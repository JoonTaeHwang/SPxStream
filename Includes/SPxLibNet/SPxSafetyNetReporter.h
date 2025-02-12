/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSafetyNetReporter.h,v $
* ID: $Id: SPxSafetyNetReporter.h,v 1.1 2014/09/15 14:45:45 rew Exp $
*
* Purpose:
*	Header for SPxSafetyNetReporter class, for reporting safety alerts
*	on the network.
*
*
* Revision Control:
*   15/09/14 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_SAFETY_NET_REPORTER_H
#define _SPX_SAFETY_NET_REPORTER_H

/*
 * Other headers required.
 */
#include "SPxLibNet/SPxPacketSend.h"	/* Our base class */

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

/*
 * Forward-declare other classes we use in case headers are in wrong order.
 */
class SPxAsterixEncoder;
class SPxSafetyNetEngine;


/*
 * Define our class, derived from generic SPx packet sending class.
 */
class SPxSafetyNetReporter :public SPxPacketSender
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxSafetyNetReporter(SPxSafetyNetEngine *engine);
    virtual ~SPxSafetyNetReporter();

    /* Sender ID. */
    void SetSAC(UINT8 sac);
    UINT8 GetSAC(void);
    void SetSIC(UINT8 sic);
    UINT8 GetSIC(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */
    /* Engine we are reporting. */
    SPxSafetyNetEngine * const m_engine;

    /* Asterix encoder helper object. */
    SPxAsterixEncoder *m_asterixEncoder;

    /*
     * Private functions.
     */
    /* Alert handlers. */
    static int staticAlertHandler(void *invokingObj, void *userArg, void *arg);
    SPxErrorCode alertHandler(struct SPxSafetyNetAlert_tag *alert);

}; /* SPxSafetyNetReporter class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SAFETY_NET_REPORTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
