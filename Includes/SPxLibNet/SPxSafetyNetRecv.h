/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSafetyNetRecv.h,v $
* ID: $Id: SPxSafetyNetRecv.h,v 1.2 2014/11/19 14:13:46 rew Exp $
*
* Purpose:
*	Header for SPxSafetyNetReceiver class, for receiving safety alerts
*	on the network.
*
*
* Revision Control:
*   19/11/14 v1.2    REW	Add BuildAlertContentString().
*
* Previous Changes:
*   24/09/14 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SAFETY_NET_RECEIVER_H
#define _SPX_SAFETY_NET_RECEIVER_H

/*
 * Other headers required.
 */
#include "SPxLibNet/SPxAsterixDecoder.h"	/* Our base class */
#include "SPxLibUtils/SPxCallbackList.h"

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


/*
 * Define our class, derived from Asterix receiving class.
 */
class SPxSafetyNetReceiver :public SPxAsterixDecoder
{
public:
    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxSafetyNetReceiver(void);
    virtual ~SPxSafetyNetReceiver();

    /* Alert reporting. */
    SPxErrorCode AddAlertCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveAlertCallback(SPxCallbackListFn_t fn, void *userObj);

    /* See base class for other configuration functions (SAC/SIC etc.). */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Conversion functions. */
    static SPxErrorCode BuildSPxAlert(const SPxAsterixMsg *msg,
				struct SPxSafetyNetAlert_tag *alert);
    static SPxErrorCode BuildAlertString(char *buffer, int bufLen,
				const struct SPxSafetyNetAlert_tag *alert,
				unsigned int verbosity=0);
    static SPxErrorCode BuildAlertContentString(char *buffer, int bufLen,
				const struct SPxSafetyNetAlert_tag *alert,
				unsigned int verbosity=0);
    static const char *GetAlertTypeString(UINT8 alertType);


private:
    /*
     * Private fields.
     */
    /* Alert reporting. */
    SPxCallbackList *m_alertCallbacks;	/* List of callbacks */

    /*
     * Private functions.
     */
    /* Alert handlers. */
    static void staticHandleReport(const SPxAsterixDecoder *decoder,
                                        void *arg,
                                        UINT8 category,
                                        SPxAsterixMsg *msg);
    SPxErrorCode handleReport(UINT8 category, SPxAsterixMsg *msg);

}; /* SPxSafetyNetReceiver class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SAFETY_NET_RECEIVER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
