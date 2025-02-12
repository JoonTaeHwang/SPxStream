/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRFTarget class, which is used to represent a 
 *   target, typically a drone or a drone remote control.
 *
 * Revision Control:
 *   25/10/18 v1.7   SP 	Support RF target database.
 *
 * Previous Changes:
 *   17/10/18 1.6   SP 	Further changes to support new detector.
 *   15/10/18 1.5   SP 	Apply detector bearing.
 *   25/06/18 1.4   AGC	Correctly suppress cppcheck warning.
 *   24/01/18 1.3   SP 	Add IsPointInSector().
 *                      Add GetCentreLatLong().
 *   16/01/18 1.2   SP 	Add GetClass().
 *   09/01/18 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_RF_TARGET_H
#define _SPX_RF_TARGET_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxViewControl.h" /* For SPxLatLong_t */

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

/* Forward-declare other classes we use. */
class SPxRFTargetDatabase;
class SPxUniTrack;
class SPxAreaObj;

/*
 * Define our class.
 */
class SPxRFTarget : public SPxObj
{
    /* These classes can access our internals directly. */
    friend class SPxRFTargetDatabase;

public:
    /*
     * Public types.
     */

    /* Change flags. */
    typedef UINT32 ChangeFlags_t;
    static const UINT32 CHANGE_TYPE         = 0x00000001;
    static const UINT32 CHANGE_STATE        = 0x00000002;
    static const UINT32 CHANGE_SELECTION    = 0x00000004;

    /* Target types. */
    typedef enum
    {
        TYPE_DRONE = 1,     /* RF from drone. */
        TYPE_REMOTE = 2,    /* RF from remote control. */
        TYPE_UNKNOWN = 3    /* RF from unknown. */
    
    } Type_t;

    /* Target states. */
    typedef enum
    {
        STATE_PROVISIONAL = 0,  /* RF received. */
        STATE_CONFIRMED = 1,    /* RF received and bearing known or confirmed by user. */
        STATE_LOST = 2          /* RF stopped. */
    
    } State_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by the SPxRFTargetDetector.
     */

    /* Configuration. */
    virtual SPxRFTargetDatabase *GetDatabase(void) { return m_database; }
    virtual SPxTime_t GetResetTime(void) const { return m_resetTime; }

    /* Access. */
    virtual UINT32 GetID(void) { return m_id; }
    virtual SPxErrorCode GetLastMesgTime(SPxTime_t *timeRtn);
    virtual SPxErrorCode GetLastUpdateTime(SPxTime_t *timeRtn);
    virtual UINT32 GetSourceID(void);
    virtual SPxErrorCode GetSourcePosition(double *latDegsRtn, 
                                           double *longDegsRtn);
    virtual Type_t GetType(void);
    virtual SPxErrorCode GetClass(char *buf, unsigned int bufLen);
    virtual SPxErrorCode GetBearingDegs(double *bearingRtn, SPxLatLong_t *refLL=NULL);
    virtual double GetWidthDegs(void) { return m_widthDegs; }
    virtual double GetLengthMetres(void) { return m_lengthMetres; }
    virtual double GetCommsFreq(void);
    virtual double GetCommsAmp(void);
    virtual SPxErrorCode SetState(State_t state);
    virtual State_t GetState(void);
    virtual SPxErrorCode SetIsSelected(int select);
    virtual int GetIsSelected(void) const { return m_isSelected; }
    virtual SPxErrorCode GetCentreLatLong(SPxLatLong_t *llRtn);

    /* User data. */
    virtual void SetUserData(void *ptr) { m_userData = ptr; }
    virtual void *GetUserData(void) { return(m_userData); }

    /* Utility functions. */
    virtual int IsPointInSector(double latDegs, double longDegs);

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

    /* Protected functions may be accessed by the SPxRFTargetDetector. */

    /* Constructor and destructor. */
    SPxRFTarget(SPxRFTargetDatabase *database);
    virtual ~SPxRFTarget(void);

    /* General. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode reset(void);
    virtual SPxErrorCode initialise(void);

    virtual int isMatch(UINT32 sourceID,
                        const char *targetID,
                        Type_t type,
                        const char *classStr);

    virtual SPxErrorCode update(const SPxTime_t *timestamp,
                                UINT32 sourceID,
                                double sourceLatDegs,
                                double sourceLongDegs,
                                const char *targetID,
                                Type_t type,
                                const char *classStr,
                                int isBearingSet,
                                double bearingDegs,
                                double widthDegs,
                                double lengthMetres,
                                double commsFreqMHz,
                                double commsAmpdBm);

    virtual void checkState(void);

    /*
     * Protected static functions.
     */

    static void resetNextID(void);

private:

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    mutable SPxCriticalSection m_mutex; /* Mutex. */
    SPxRFTargetDatabase *m_database;    /* Target database . */
    SPxTime_t m_resetTime;              /* Last reset time. */
    UINT32 m_id;                        /* Unique ID. */
    State_t m_state;                    /* Target state. */
    SPxTime_t m_lastMesgTime;           /* Last timestamp from message. */
    SPxTime_t m_lastUpdateTime;         /* Time of last update. */
    UINT32 m_sourceID;                  /* Detector source ID. */
    double m_sourceLatDegs;             /* Detector source latitude. */
    double m_sourceLongDegs;            /* Detector source longitude. */
    Type_t m_type;                      /* Target type. */
    int m_isBearingSet;                 /* Is bearing known? */
    double m_bearingDegs;               /* Bearing from north. */
    double m_widthDegs;                 /* Width in degress. */
    double m_lengthMetres;              /* Length in metres. */
    double m_commsFreqMHz;              /* Comms frequency in MHz. */
    double m_commsAmpdBm;               /* Comms amplitude in dBm. */
    void *m_userData;                   /* User data. */
    int m_isSelected;                   /* Is selected? */

    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode setStateInternal(State_t state);
    SPxErrorCode getBearingInternal(double *bearingRtn, SPxLatLong_t *refLL);
    SPxErrorCode handleChange(ChangeFlags_t flags);
    SPxErrorCode reportChangeEvent(ChangeFlags_t flags);

    /*
     * Private static functions.
     */

}; /* SPxRFTarget */

#endif /* SPX_RF_TARGET_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
