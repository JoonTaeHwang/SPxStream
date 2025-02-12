/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRFTargetDatabase class which supports storing
 *   RF targets and display in a PPI window using a renderer.
 *
 * Revision Control:
 *   25/10/18 v1.1   SP 	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_RF_TARGET_DATABASE_H
#define _SPX_RF_TARGET_DATABASE_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxThreads.h"
#include "SPxLibNet/SPxRFTargetDetector.h"
#include "SPxLibNet/SPxRFTarget.h"

/* Forward declare any classes required. */

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Parameters. */

/* Maximum expected interval between target updates. */
#define MAX_UPDATE_INTERVAL_SECS     10

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

/*
 * Define our class.
 */
class SPxRFTargetDatabase : public SPxObj
{
    /* The objects these can access our internals directly. */
    friend class SPxRFTarget;
    friend class SPxRFTargetDetector;
    friend class SPxRFTargetDtrRedrone;
    friend class SPxRFTargetDtrArgos;
    friend class SPxRFJammer;

public:

    /*
     * Public types.
     */

    /* Types of detector. */
    typedef enum
    {
        DTR_TYPE_NONE,
        DTR_TYPE_REDRONE,   /* ReDrone. */
        DTR_TYPE_ARGOS      /* MyDefence Watchdog, Wolfpack & Wingman. */

    } DtrType_t;

    /* Notification event type. */
    typedef enum
    {
        EVENT_TARGET_CREATE,        /* A target has been created. */
        EVENT_TARGET_CHANGE,        /* A target has changed. */
        EVENT_TARGET_DELETE         /* A target is about to be deleted. */

    } EventType_t;

    /* RFTarget event info. Used by:
     *  EVENT_TARGET_CREATE
     *  EVENT_TARGET_CHANGE
     *  EVENT_TARGET_DELETE
     */
    typedef struct
    {
        SPxRFTarget *target;                 /* Target handle. */
        SPxRFTarget::ChangeFlags_t flags;    /* Change flags. */

    } EventRFTargetInfo_t;

    /* Union of all event info structures. */
    typedef union
    {
        EventRFTargetInfo_t targetEvt;
    
    } EventInfo_t;

    /* Top-level event structure. Use type to determine the event type 
     * and then access one (and only one) of the event info structures 
     * that follow.
     */
    typedef struct
    {
        EventType_t type;       /* Event type. */
        EventInfo_t info;       /* Event info. */
    
    } Event_t;

    /* A detector source. */
    typedef struct
    {
        UINT32 sourceID;        /* ID of this source. */
        char name[64];          /* Name of this source. */
        double latDegs;         /* Latitude in degrees. */
        double longDegs;        /* Longitude in degrees. */
        double bearingDegs;     /* Bearing offset in degrees. */
    
    } Source_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxRFTargetDatabase(void);
    virtual ~SPxRFTargetDatabase(void);
    virtual SPxErrorCode Create(void);

    /* Detector sources. */
    virtual SPxErrorCode IterateSources(SPxCallbackListFn_t fn, 
                                        void *userArg);

    /* Targets. */
    virtual SPxErrorCode SetTargetPurgeTimeoutSecs(unsigned int secs);
    virtual unsigned int GetTargetPurgeTimeoutSecs(void) { return m_targetPurgeTimeoutSecs; }
    virtual unsigned int GetNumTargets(void);
    virtual SPxErrorCode IterateTargets(SPxCallbackListFn_t fn, 
                                        void *userArg);
    virtual SPxErrorCode DeleteTarget(SPxRFTarget *target);
    virtual SPxRFTarget *GetTarget(double latDegs, 
                                   double longDegs,
                                   SPxRFTarget::Type_t type=SPxRFTarget::TYPE_UNKNOWN);

    /* Event callbacks. */
    virtual SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, 
                                          void *userObj);

    virtual SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, 
                                             void *userObj);

    /* Debug. */
    virtual void SetLogFile(FILE *logFile);
    virtual void SetVerbosity(unsigned int verbosity);
    virtual unsigned int GetVerbosity(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Add a detector to the database. */
    virtual UINT32 addDetector(SPxRFTargetDetector *detector);

    /* Add or update a target. */
    virtual SPxErrorCode updateTarget(const SPxTime_t *timestamp,
                                      UINT32 sourceID,
                                      double sourceLatDegs,
                                      double sourceLongDegs,
                                      const char *targetID,
                                      SPxRFTarget::Type_t targetType,
                                      const char *targetClass,
                                      int isTargetBearingSet,
                                      double targetBearingDegs,
                                      double targetWidthDegs,
                                      double targetLengthMetres,
                                      double targetCommsFreqMHz,
                                      double targetCommsAmpdBm);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    FILE *m_logFile;                        /* Debug log file. */
    unsigned int m_verbosity;               /* Debug versbosity. */
    SPxCallbackList *m_eventCBList;         /* Event callback list. */
    unsigned int m_targetPurgeTimeoutSecs;  /* Timeout before a non-updating target is purged. */
    SPxThread *m_thread;                    /* Background thread. */
    UINT32 m_nextDetectorID;                /* Next detector ID to assign. */

    /*
     * Private functions.
     */

    /* Targets. */
    SPxRFTarget *createTarget(SPxErrorCode *errRtn);
    SPxErrorCode deleteTarget(SPxRFTarget *target);
    SPxRFTarget *getNewTarget(void);
    SPxErrorCode setTargetInUse(SPxRFTarget *target, int isInUse);
    SPxErrorCode checkTargets(void);

    /* Events. */
    SPxErrorCode reportEvent(Event_t *evt);

    /*
     * Private static functions.
     */

    static void *threadHandler(SPxThread *thread);

}; /* SPxRFTargetDatabase */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_RF_TARGET_DATABASE_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
