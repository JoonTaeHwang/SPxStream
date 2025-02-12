/*********************************************************************
*
* (c) Copyright 2014, 2015, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxFlightPlanDb class which supports the storage
*	and handling of multiple SPxFlightPlan objects.
*
* Revision Control:
*   22/10/20 v1.6    REW	Support link with InfoDB object.
*
* Previous Changes:
*   03/11/15 1.5    AGC	Packet decoder callback data now const.
*   17/06/14 1.4    REW	Complete support for network sharing.
*   06/06/14 1.3    REW	Support network sharing.
*   21/01/14 1.2    REW	Use thread to support auto-deletion of plans.
*   15/01/14 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_FLIGHT_PLAN_DB_H
#define _SPX_FLIGHT_PLAN_DB_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxSysUtils.h"
#include "SPxLibData/SPxFlightPlan.h"


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
class SPxFlightPlan;
class SPxThread;
class SPxPacketDecoder;
class SPxPacketDecoderNet;
class SPxPacketSender;
class SPxInfoDBNode;
class SPxInfoDBItem;

/*
 * Define our class.
 */
class SPxFlightPlanDb : public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxFlightPlanDb(void);
    virtual ~SPxFlightPlanDb(void);

    /* File storage. */
    SPxErrorCode LoadFromFile(const char *filename);
    SPxErrorCode SaveToFile(const char *filename);
    const char *GetLongFilePath(void) { return(m_filename.GetFullPath()); }
    const char *GetShortFilePath(void) { return(m_filename.GetShortPath()); }

    /* Plan creation etc. */
    SPxFlightPlan *CreatePlan(void);
    SPxErrorCode DeletePlan(SPxFlightPlan *plan);
    SPxErrorCode DeleteAllPlans(void);

    /* Auto deletion control (zero means don't auto-delete). */
    SPxErrorCode SetAutoDeleteMinutes(unsigned int mins);
    unsigned int GetAutoDeleteMinutes(void) const {return(m_autoDeleteMinutes);}

    /* Network sharing. */
    SPxErrorCode SetAddress(const char *address);
    const char *GetAddress(void) const { return(m_netAddr); }
    SPxErrorCode SetPort(unsigned int port);
    unsigned int GetPort(void) const { return(m_netPort); }
    SPxErrorCode SetNetHz(unsigned int hz);
    unsigned int GetNetHz(void) const { return(m_netHz); }

    /* Link with info database. */
    SPxErrorCode SetInfoDb(SPxInfoDBNode *infoDb);
    SPxInfoDBNode *GetInfoDb(void) const { return(m_infoDb); }

    /* Info retrieval. */
    SPxErrorCode GetPlanCounts(unsigned int *totalPtr,
				unsigned int *activePtr,
				unsigned int *inactivePtr);
    SPxFlightPlan *FindPlanForTrack(
				const struct SPxPacketTrackExtended_tag *track);
    UINT32 GetMsecsLastNetPacketUsed(void) {return(m_lastMsecsNetPacketUsed);}

    /* Read plans from the database. */
    SPxErrorCode IteratePlans(SPxCallbackListFn_t planFn, void *userArg);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    /* Mutex for linked list etc. */
    SPxCriticalSection m_mutex;

    /* Linked-list of plans. */
    SPxFlightPlan *m_planListHead;
    SPxFlightPlan *m_planListTail;

    /* Linked-list of deleted plans that are free to be reused */
    SPxFlightPlan *m_freePlanListHead;
    SPxFlightPlan *m_freePlanListTail;

    /* Worker thread for database maintenance. */
    SPxThread *m_thread;

    /* Name of file storing plans. */
    SPxPath m_filename;

    /* Network sharing of plans. */
    SPxPacketDecoderNet *m_packetDecoder;	/* For receiving plans */
    SPxPacketSender *m_packetSender;		/* For sending plans */
    char *m_netAddr;				/* Address string */
    unsigned int m_netPort;			/* Port number */
    unsigned int m_netHz;			/* Sending frequency */
    UINT32 m_lastMsecsNetPacketUsed;		/* Time of last update */

    /* Config options.  */
    unsigned int m_autoDeleteMinutes;	/* 0, or time to delete after use */

    /* Link the info database. */
    SPxInfoDBNode *m_infoDb;		/* NULL if not linked */

    /*
     * Private functions.
     */
    SPxErrorCode deletePlanLocked(SPxFlightPlan *plan);
    void *threadFunc(SPxThread *thread);

    /* Info database iterator. */
    static int infoDbIterStatic(void *infoDb, void *obj, void *item);
    int infoDbIter(SPxInfoDBNode *infoDb, SPxInfoDBItem *item);

    /* Network handlers. */
    static void flightPlanNetHandlerStatic(SPxPacketDecoder *decoder,
						void *userArg,
						UINT32 packetType,
						struct sockaddr_in *from,
						SPxTime_t *timestamp,
						const unsigned char *payload,
						unsigned int numBytes);
    void flightPlanNetHandler(struct sockaddr_in *from, SPxTime_t *timestamp,
				const unsigned char *payload, unsigned int numBytes);

    /* Thread function (static). */
    static void *threadWrapper(SPxThread *thread);
}; /* SPxFlightPlanDb */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_FLIGHT_PLAN_DB_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
