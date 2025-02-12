/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Network interface monitoring utility.
 *
 * Revision Control:
 *   04/03/20 v1.1   SP 	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_NET_INTERFACES_H
#define _SPX_NET_INTERFACES_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibSc/SPxScNet.h"

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

/* Forward declare classes we use. */

/*
 *  SPxNetInterfaces class.
 */
class SPxNetInterfaces : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /* Loopback address 127.0.0.1. */
    static const UINT32 LOOPBACK_ADDR = 0x7F000001;

    /*
     * Public types.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxNetInterfaces(unsigned int updateRateSecs=10);
    virtual ~SPxNetInterfaces(void);

    /* General. */
    int IsInterfaceLocal(UINT32 addr);
    int IsInterfaceEnabled(UINT32 addr);
    int IsInterfaceConnected(UINT32 addr);
    UINT32 GetDefaultInterfaceAddr(void);
    unsigned int GetInterfacesAndLock(SPxScNetIfStruct **netIfsRtn);
    SPxErrorCode UnlockInterfaces(SPxScNetIfStruct **netIfs);

    /*
     * Public static functions.
     */

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* General. */

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;               /* Private data. */
    SPxTimer m_timers;                  /* Thread timer. */

    /* Network interfaces. */
    SPxCriticalSection m_mutex;         /* Mutex. */
    SPxScNetIfStruct m_netIfs[32];      /* This PC's network interfaces. */
    unsigned int m_numNetIfs;           /* Number of items in m_netIfs[]. */

    /*
     * Private functions.
     */

    /* General. */
    void update(void);
    UINT32 getDefault(void);

    /*
     * Private static functions.
     */

    /* Timer callbacks. */
    static void updateTimer(void *userArg);

}; /* SPxNetInterfaces */

#endif /* _SPX_NET_INTERFACES_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
