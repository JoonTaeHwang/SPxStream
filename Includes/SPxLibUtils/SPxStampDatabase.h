/*********************************************************************
 *
 * (c) Copyright 2014 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxStampDatabase class header.
 *
 * Revision Control:
 *   03/01/20 v1.15  SP 	Report status when logging.
 *
 * Previous Changes:
 *   02/01/20 1.14  SP 	Report stamp ID when logging.
 *   06/03/19 1.13  SP 	Allow errors to be hidden in GetStampAndLock().
 *   25/06/18 1.12  AGC	Correctly suppress cppcheck warning.
 *   29/09/16 1.11  SP 	Add isDatabaseLocked flag to ReportEvent().
 *   04/02/16 1.10  SP 	Add updateNextStampID().
 *   22/01/16 1.9   SP 	Use UINT8 for channel ID.
 *   16/11/15 1.8   SP 	Improvements to channel locking.
 *   26/02/15 1.7   SP 	Support config load/save
 *   05/02/15 1.6   SP 	Add text field to event structure.
 *                      Make some protected functions private.
 *   26/01/15 1.5   SP  Remove EVENT_STAMP_LOG_SYSTEM.
 *                      Rename EVENT_STAMP_LOG_USER to EVENT_STAMP_LOG.
 *   07/01/15 1.4   SP 	Add EventStampLogInfo_t.
 *   11/12/14 1.3   SP 	Add user and system log events.
 *   21/11/14 1.2   SP 	Further development.
 *   06/11/14 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_STAMP_DATABASE_H
#define _SPX_STAMP_DATABASE_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxStamp.h"

/*********************************************************************
 *
 *  Constants
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

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */

class SPxStampDatabase : public SPxObj
{
    /* Grant SPxStamp private access. */
    friend class SPxStamp;

public:

    /*
     * Public types.
     */

    /* Selection state. */
    typedef enum
    {
        STATE_SELECT,
        STATE_DESELECT,
        STATE_TOGGLE
    
    } State_t;

    /* Notification event type. */
    typedef enum
    {
        EVENT_STAMP_SELECT,    /* Stamp selected or deselected. */
        EVENT_STAMP_ADD,       /* Stamp added. */
        EVENT_STAMP_REMOVE,    /* Stamp removed. */
        EVENT_STAMP_LOG        /* Stamp logged. */

    } EventType_t;

    /* Stamp event info. Used by:
     *  EVENT_STAMP_SELECT
     *  EVENT_STAMP_ADD
     *  EVENT_STAMP_REMOVE
     */
    typedef struct
    {
        SPxStamp *stamp;  /* Stamp handle or NULL. */

    } EventStampInfo_t;

    /* Stamp event info. Used by:
     *  EVENT_STAMP_LOG
     */
    typedef struct
    {
        unsigned int stampID;             /* Stamp ID. */
        SPxStamp::Type_t stampType;       /* Stamp type. */
        UINT32 rgb;                       /* Colour. */
        UINT8 chanID;                     /* Associated channel ID. */
        int isLatLongSet;                 /* Does event have lat/long? */
        SPxLatLong_t latLong;             /* Event position. */
        SPxStatus_t status;               /* Event status (added V1.83.1). */

    } EventStampLogInfo_t;

    /* Union of all event info structures. */
    typedef union
    {
        EventStampInfo_t stampEvt;
        EventStampLogInfo_t stampLogEvt;
    
    } EventInfo_t;

    /* Top-level event structure. Use type to determine the event type 
     * and then access one (and only one) of the event info structures 
     * that follow.
     */
    typedef struct
    {
        SPxTime_t time;     /* Event time. */
        EventType_t type;   /* Event type. */
        EventInfo_t info;   /* Event info. */
        char text[128];     /* Optional text. */
    
    } Event_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxStampDatabase(void);
    virtual ~SPxStampDatabase(void);

    /* Stamp event callbacks. */
    virtual SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, 
                                          void *userObj);
    virtual SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, 
                                             void *userObj);

    /* Get name to assign to a stamp. */
    void GetStampName(char *buf, unsigned int bufSizeBytes,
                      unsigned int id=0);

    /* Create a new stamp. */
    SPxStamp *CreateStamp(SPxErrorCode *errRtn,
                          SPxStamp::Type_t type,
                          unsigned int id=0);

    /* Access a stamp. */
    virtual unsigned int GetNumStamps(void);
    virtual SPxStamp *GetStampAndLock(unsigned int id, int reportErr=TRUE);
    virtual SPxStamp *GetSelectedStampAndLock(void);
    virtual SPxErrorCode UnlockStamp(SPxStamp *stamp);

    /* Iterate through stamps. */
    virtual SPxErrorCode IterateForward(SPxCallbackListFn_t fn, 
                                        void *userArg);
    virtual SPxErrorCode IterateReverse(SPxCallbackListFn_t fn, 
                                        void *userArg);

    /* Select a stamp. */
    virtual SPxErrorCode SetSelectedStamp(SPxStamp *stamp, 
                                          State_t state);

    /* Clear the database. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode DeleteAllStamps(void);

    /* Event reporting. */
    SPxErrorCode ReportEvent(Event_t *evt, int isDatabaseLocked=FALSE);

    /* Config load/save. */
    virtual SPxErrorCode Load(const char *filename);
    virtual SPxErrorCode Save(const char *filename);
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Event callback list. */
    SPxCallbackList *m_eventCBList;
    SPxCriticalSection m_eventMutex;

    /* Selected stamp. */
    SPxStamp *m_selectedStamp;

    /* Next stamp ID to allocate (zero is invalid). */
    unsigned int m_nextStampID;

    /*
     * Private functions.
     */

    /* Add a stamp. */
    SPxErrorCode addStamp(SPxStamp *stamp);
    SPxErrorCode removeStamp(SPxStamp *stamp);

    /* Event reporting. */
    SPxErrorCode reportEventInternal(Event_t *evt);
    SPxErrorCode setSelectedStampInternal(SPxStamp *stamp,
                                          State_t state);

    /* Misc. */
    void updateNextStampID(void);

}; /* SPxStampDatabase */

#endif /* _SPX_STAMP_DATABASE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
