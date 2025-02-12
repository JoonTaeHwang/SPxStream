/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxPing class.
 *
 *   This class that is used to send ICMP echo requests (i.e. pings) 
 *   and receive ICMP echo replies.
 *
 * Revision Control:
 *   19/05/22 v1.3   SP 	Make reply handling more robust.
 *
 * Previous Changes:
 *   16/05/22 1.2   SP 	Make some functions const.
 *   12/05/22 1.1   SP 	Initial version.
 *
 **********************************************************************/

#ifndef _SPX_PING_SENDER_H
#define _SPX_PING_SENDER_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibNet/SPxRawSender.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxTimer.h"

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

/* Forward-declare classes we may use. */

/*
 * Define our class.
 */
class SPxPing :public SPxRawSender
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxPing(void);
    virtual ~SPxPing(void);

    /* Config. */
    SPxErrorCode SetTimeout(UINT32 msecs);
    UINT32 GetTimeout(void) const { return m_timeoutMsecs; }

    /* Send single ping. */
    SPxErrorCode SendPing(UINT32 *tripMsecsRtn);

    /* Send continuous pings. */
    SPxErrorCode StartPings(UINT32 intervalMsecs);
    SPxErrorCode StopPings(void);
    int IsPinging(void) const { return m_isTimerRunning; }

    /* Statistics. */
    INT32 GetLastTripMsecs(void) const;
    INT32 GetAverageTripMsecs(void) const;
    unsigned int GetNumPingsSent(unsigned *numRepliesRtn) const;
    double GetSuccessRate(void) const;
    SPxErrorCode ResetStats(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    SPxTimer m_timers;
    UINT16 m_identifier;
    UINT16 m_nextSeqNumber;
    int m_isTimerRunning;
    UINT32 m_timeoutMsecs;
    unsigned int m_numPingsSent;
    unsigned int m_numReplies;
    unsigned int m_numRepliesLast10;
    double m_successRateLast10;
    INT32 m_lastTripTime;
    INT32 m_averageTripTime;

    /*
     * Private functions.
     */

    SPxErrorCode sendPing(UINT32 *tripMsecsRtn);

    SPxErrorCode waitForReply(UINT16 seqNumber);

    SPxErrorCode handleReplies(UINT16 seqNumber,
                               const UINT8 *buf, 
                               unsigned int numBytes, 
                               const sockaddr_in *from);

    SPxErrorCode handleReply(UINT16 seqNumber,
                             const UINT8 *buf, 
                             unsigned int numBytes);

    /*
     * Private static functions.
     */

    static void sendPingTimer(void *userArg);

}; /* SPxPing */

/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_PING_SENDER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
