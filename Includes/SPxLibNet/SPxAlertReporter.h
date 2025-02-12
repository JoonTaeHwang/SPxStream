/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAlertReporter.h,v $
* ID: $Id: SPxAlertReporter.h,v 1.1 2012/12/21 16:18:27 rew Exp $
*
* Purpose:
*	Header for SPxAlertReporter object which support sending alert
*	messages to clients.
*
*
* Revision Control:
*   21/12/12 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_ALERT_REPORTER_H
#define _SPX_ALERT_REPORTER_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxError.h"

/* We need our base class. */
#include "SPxLibNet/SPxPacketSend.h"

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
struct SPxPacketAlert_tag;

/*
 * Define our class, derived from the standard SPx base class.
 */
class SPxAlertReporter :public SPxPacketSender
{
public:
    /* Public functions. */
    explicit SPxAlertReporter(void);
    virtual ~SPxAlertReporter(void);

    /* Configure settings. */
    SPxErrorCode SetSenderID(UINT32 senderID) { m_senderID = senderID; return SPX_NO_ERROR; }
    UINT32 GetSenderID(void) const { return m_senderID; }

    /* Report alerts. */
    SPxErrorCode Report(UINT32 packetType, 
	void *data, unsigned int numBytes);

    /* Generic parameter control. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    UINT32 m_senderID;
    UINT32 m_alertID;

}; /* SPxAlertReporter class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_ALERT_REPORTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
