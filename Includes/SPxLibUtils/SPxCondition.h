/*********************************************************************
 *
 * (c) Copyright 2018 - 2022, Cambridge Pixel Ltd.SPEED_DEVIATION
 *
 * Purpose:
 *   Header for SPxCondition base class.
 *
 * Revision Control:
 *   26/04/22 v1.47  BTB	Add TYPE_TRACK_THREAT_LEVEL.
 *
 * Previous Changes:
 *   28/02/22 1.46  BTB	Support condition for starting in an area.
 *   24/01/22 1.45  SP 	Add IsArgAISAttribute() and IsArgADSBAttribute().
 *   24/01/22 1.44  BTB	Add getArgTrackDescriptor().
 *   20/01/22 1.43  SP 	Add TYPE_TRACK_SPEED_DEVIATION and TYPE_TRACK_COURSE_DEVIATION.
 *   18/01/22 1.42  SP 	Simplify getArgDisplayString().
 *   18/01/22 1.41  SP 	Add TYPE_TRACK_ACCELERATION and TYPE_TRACK_RATE_OF_TURN.
 *   17/01/22 1.40  SP 	Support range limit operator in some proximity alarms.
 *   13/01/22 1.39  SP 	Add TYPE_PROXIMITY_TO_SHAPE.
 *   13/01/22 1.38  SP 	Rename track draught condition.
 *   12/01/22 1.37  SP 	Support track draught condition.
 *   22/09/21 1.36  BTB	Add IsTimeLimitOperatorValid() and IsRangeLimitOperatorValid().
 *   13/05/21 1.35  SP 	Add IsTypeProximity().
 *   12/05/21 1.34  AGC	Fix unused argument warnings.
 *   12/05/21 1.33  SP 	Rework to move CPA and proximity tests here.
 *   12/05/21 1.32  SP 	Add TYPE_PROXIMITY_TO_TRACK.
 *   11/05/21 1.31  SP 	Make trigger fields more generic.
 *   06/05/21 1.30  SP 	Add IsArgTrackAttribute().
 *   06/05/21 1.29  SP 	Fix testCPAToTrack() after last change.
 *   05/05/21 1.28  SP 	Store track using descriptor, not handle.
 *   22/04/21 1.27  SP 	Sync enable state of buddy conditions.
 *   20/04/21 1.26  SP 	Support condition enable/disable.
 *   20/04/21 1.25  SP 	Support time and range limit operators.
 *   20/04/21 1.24  SP 	Make operator specific to argument.
 *   16/04/21 1.24  SP 	Make operator specific to argument.
 *   13/04/21 1.23  SP 	Rename operation to operator.
 *   08/04/21 1.22  SP 	Support unique ID.
 *   10/02/21 1.21  AGC	Support new proximity conditions.
 *                     	Support timestamp condition.
 *   10/02/21 1.20  BTB	Fix trigger text for secondary ID conditions.
 *   08/10/20 1.19  SP 	Support restrictions.
 *   24/09/20 1.18  AGC	Add comms and flight status conditions.
 *   10/12/19 1.17  BTB	Support AIS only condition for hazardous cargo.
 *   13/05/19 1.16  SP 	Support Boolean option value.
 *   08/05/19 1.15  BTB	Support condition for lingering in an area.
 *   02/05/19 1.14  REW	Support OnGround, IsTest and IsSimulated.
 *   29/04/19 1.13  BTB	Support AIS specific conditions.
 *   12/12/18 1.12  BTB	Support secondary track type conditions.
 *   12/11/18 1.11  BTB	Store a name to go with shapes, allow shapes to be reset.
 *   26/09/18 1.10  SP 	Fix N/C #607 - Course conditions not saved and restored.
 *   19/09/18 1.9   SP 	Support CPA to ref point condition.
 *   18/09/18 1.8   SP 	Support track group conditions.
 *   17/09/18 1.7   SP 	Make private function protected.
 *   25/06/18 1.6   AGC	Correctly suppress cppcheck warning.
 *   14/06/18 1.5   SP 	Add GetArgValueMultiplier().
 *   04/06/18 1.4   SP 	Support track class.
 *   25/05/18 1.3   REW	Support altitude.
 *   08/05/18 1.2   SP 	Add setArgShape() etc for use in a derived class.
 *   03/05/18 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CONDITION_H
#define _SPX_CONDITION_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"

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
class SPxUniTrack;
class SPxAISTrack;
class SPxADSBTrack;
class SPxRadarTrack;
class SPxAreaObj;
class SPxUniTrackDatabase;
class SPxConditionGroup;
class SPxRefPoint;

/*
 * Define our class.
 */
class SPxCondition : public SPxObj
{
    /* These classes can access our internals directly. */
    friend class SPxConditionGroup;

public:
    /*
     * Public types.
     */

    /* Change flags. */
    typedef UINT32 ChangeFlags_t;
    static const UINT32 CHANGE_STATE                = 0x00000001;
    static const UINT32 CHANGE_ARG_OPERATOR         = 0x00000002;
    static const UINT32 CHANGE_ARG_VALUE            = 0x00000003;
    static const UINT32 CHANGE_TIME_LIMIT_OPERATOR  = 0x00000004;
    static const UINT32 CHANGE_TIME_LIMIT_VALUE     = 0x00000010;
    static const UINT32 CHANGE_RANGE_LIMIT_OPERATOR = 0x00000020;
    static const UINT32 CHANGE_RANGE_LIMIT_VALUE    = 0x00000040;
    static const UINT32 CHANGE_EXCLUSION            = 0x00000080;


    /* Condition operator - DO NOT change values as they are used in config files. */
    typedef UINT32 Operator_t;
    static const UINT32 OPERATOR_NONE           = 0;    /* No operator. */
    static const UINT32 OPERATOR_EQUAL          = 1;
    static const UINT32 OPERATOR_NOT_EQUAL      = 2;
    static const UINT32 OPERATOR_GREATER_THAN   = 3;
    static const UINT32 OPERATOR_LESS_THAN      = 4;
    static const UINT32 NUM_OPERATORS           = 5;    /* KEEP UPDATED. */

    /* Condition types - DO NOT change values as they are used in config files. */
    typedef UINT32 Type_t;
    static const UINT32 TYPE_UNKNOWN                = 0;    /* Not set. */
    static const UINT32 TYPE_AREA_INSIDE            = 100;  /* Currently inside a area. */
    static const UINT32 TYPE_AREA_VISITED           = 101;  /* Has visited a area. */
    static const UINT32 TYPE_AREA_NOT_LEFT          = 102;  /* Has not left an area. */
    static const UINT32 TYPE_AREA_STARTED           = 103;  /* Started in an area. */
    static const UINT32 TYPE_GATE_CROSSED_FWD       = 200;  /* Has crossed a gate forward. */
    static const UINT32 TYPE_GATE_CROSSED_REV       = 201;  /* Has crossed a gate reverse. */
    static const UINT32 TYPE_GATE_CROSSED           = 202;  /* Has crossed a gate in any direction. */
    static const UINT32 TYPE_TRACK                  = 300;  /* Track identified by unified ID. */
    static const UINT32 TYPE_TRACK_SECONDARY_ID     = 301;  /* Track identified by secondary ID. */
    static const UINT32 TYPE_TRACK_SPEED            = 302;  /* Track speed. */
    static const UINT32 TYPE_TRACK_COURSE           = 303;  /* Track course. */
    static const UINT32 TYPE_TRACK_ALTITUDE         = 304;  /* Track altitude. */
    static const UINT32 TYPE_TRACK_CLASS            = 305;  /* Track class. */
    static const UINT32 TYPE_TRACK_GROUP            = 306;  /* Track in a track group. */
    static const UINT32 TYPE_TRACK_SECONDARY_TYPE   = 307;  /* Track identified by secondary track type. */
    static const UINT32 TYPE_TRACK_ON_GROUND        = 308;  /* Track on ground flag. */
    static const UINT32 TYPE_TRACK_IS_TEST          = 309;  /* Track is test target */
    static const UINT32 TYPE_TRACK_IS_SIMULATED     = 310;  /* Track is simulated. */
    static const UINT32 TYPE_TRACK_COMMS_CAP        = 311;  /* Track communications capability. */
    static const UINT32 TYPE_TRACK_FLIGHT_STATUS    = 312;  /* Track flight status. */
    static const UINT32 TYPE_TRACK_ACCELERATION     = 313;  /* Track acceleration. */
    static const UINT32 TYPE_TRACK_RATE_OF_TURN     = 314;  /* Track rate of turn. */
    static const UINT32 TYPE_TRACK_SPEED_DEVIATION  = 315;  /* Track speed deviation (between primary and secondary). */
    static const UINT32 TYPE_TRACK_COURSE_DEVIATION = 316;  /* Track course deviation (between primary and secondary). */
    static const UINT32 TYPE_TRACK_THREAT_LEVEL     = 317;  /* Track threat level. */
    static const UINT32 TYPE_AIS_SHIP_TYPE          = 400;  /* AIS ship type. */
    static const UINT32 TYPE_AIS_NAV_STATUS         = 401;  /* AIS navigation status. */
    static const UINT32 TYPE_AIS_HAZARDOUS_CARGO    = 402;  /* AIS hazardous cargo. */
    static const UINT32 TYPE_AIS_DRAUGHT            = 403;  /* AIS vessel draught (extent below waterline). */
    static const UINT32 TYPE_PROXIMITY_TO_POINT     = 500;  /* Proximity to point (superseded by TYPE_PROXIMITY_TO_SHAPE). */
    static const UINT32 TYPE_PROXIMITY_TO_REF_POINT = 503;  /* Proximity to ref point. */
    static const UINT32 TYPE_PROXIMITY_TO_ANY_AIS   = 504;  /* Proximity to any AIS track (specific to VSD). */
    static const UINT32 TYPE_PROXIMITY_TO_ANY_ADSB  = 505;  /* Proximity to any ADS-B track (specific to VSD). */
    static const UINT32 TYPE_PROXIMITY_TO_TRACK     = 506;  /* Proximity to a track. */
    static const UINT32 TYPE_PROXIMITY_TO_ANY_TRACK = 507;  /* Proximity to any track. */
    static const UINT32 TYPE_PROXIMITY_TO_LAND      = 510;  /* Proximity to land. */
    static const UINT32 TYPE_PROXIMITY_TO_SHAPE     = 511;  /* Proximity to shape. */
    static const UINT32 TYPE_APPROACH_LAND          = 520;  /* Approaching land. */
    static const UINT32 TYPE_CPA_TO_POINT           = 600;  /* CPA to a point.*/
    static const UINT32 TYPE_CPA_TO_TRACK           = 601;  /* CPA to a track. */
    static const UINT32 TYPE_CPA_TO_ANY_TRACK       = 602;  /* CPA to any track. */
    static const UINT32 TYPE_CPA_TO_REF_POINT       = 603;  /* CPA to a reference point. */
    static const UINT32 TYPE_TIMESTAMP              = 700;  /* Unix timestamp. */
    static const UINT32 NUM_TYPES                   = 44;   /* KEEP UPDATED!!. */

    /* Restriction flags. */
    typedef UINT32 RestrictionFlags_t;
    static const UINT32 RESTRICT_ARG_OPERATOR  = 0x00000001;
    static const UINT32 RESTRICT_ARG_VALUE     = 0x00000002;
    static const UINT32 RESTRICT_ALL           = 0xFFFFFFFF;

    /* Trigger condition. */
    typedef struct
    {
        Type_t type;                /* Conditon type. */
        Operator_t argOp;           /* Argument operator (equal or not equal only). */
        char argStr[64];            /* Argument that was tested by the condition. */
        SPxUniTrack *argTrack;      /* Track argument that was tested by the condition. */
        SPxAreaObj *argShape;       /* Shape argument that was tested by the condition. */
        SPxRefPoint *argRefPoint;   /* Reference point argument that was tested by the condition. */
        double timeLimitSecs;       /* Time limit in seconds. */
        Operator_t timeLimitOp;     /* Time limit operator. */
        double rangeLimitMetres;    /* Range limit in metres. */
        Operator_t rangeLimitOp;    /* Range limit operator. */

    } Trigger_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by a friend class.
     */

    /* Misc. */
    virtual UINT32 GetID(void) { return m_id; }
    virtual SPxUniTrackDatabase *GetTrackDB(void) { return m_trackDB; }
    virtual SPxCondition *GetBuddy(void) const { return m_buddy; }
    virtual SPxTime_t GetResetTime(void) const { return m_resetTime; }
    virtual Type_t GetType(void) { return m_type; }
    virtual const char *GetTypeString(void);
    virtual SPxCondition *GetBuddy(void) { return m_buddy; }
    virtual int IsPrimary(void) { return m_isPrimary; }
    virtual SPxErrorCode SetEnabled(int isEnabled, int fromBuddy=FALSE);
    virtual int IsEnabled(void) const{ return m_isEnabled; }

    /* Argument. */
    virtual SPxErrorCode SetArgValue(double value);
    virtual double GetArgValue(void) const { return getArgDouble(); }
    virtual double GetArgValueMultiplier(void);
    virtual SPxErrorCode SetArgString(const char *str);
    virtual const char *GetArgString(void);
    virtual const char *GetArgDisplayString(void);
    virtual int IsArgOperatorValid(Operator_t op);
    virtual SPxErrorCode SetArgOperator(Operator_t op);
    virtual Operator_t GetArgOperator(void) { return m_argOp; }
    virtual const char *GetArgOperatorString(void);
    virtual int IsConditionValid(void);

    /* Time limit. */
    virtual int IsTimeLimitValid(void);
    virtual SPxErrorCode SetTimeLimit(double secs);
    virtual double GetTimeLimit(void) const { return m_timeLimitSecs; }
    virtual int IsTimeLimitOperatorValid(Operator_t op);
    virtual SPxErrorCode SetTimeLimitOperator(Operator_t op);
    virtual Operator_t GetTimeLimitOperator(void) { return m_timeLimitOp; }
    virtual const char *GetTimeLimitOperatorString(void);

    /* Range limit. */
    virtual int IsRangeLimitValid(void);
    virtual SPxErrorCode SetRangeLimit(double metres);
    virtual double GetRangeLimit(void) const { return m_rangeLimitMetres; }
    virtual int IsRangeLimitOperatorValid(Operator_t op);
    virtual SPxErrorCode SetRangeLimitOperator(Operator_t op);
    virtual Operator_t GetRangeLimitOperator(void) { return m_rangeLimitOp; }
    virtual const char *GetRangeLimitOperatorString(void);

    /* Exclusions. */
    virtual SPxErrorCode SetExclusion(int exclusion);
    virtual int IsExclusion(void) const;

    /* Restrictions. */
    virtual SPxErrorCode SetRestrictions(RestrictionFlags_t flags);
    virtual RestrictionFlags_t GetRestrictions(void) { return m_restrictions; }
    virtual int IsRestricted(RestrictionFlags_t flags);

    /*
     * Public static functions.
     */

    static int IsTypeValid(Type_t type);
    static Type_t GetType(unsigned int index);
    static const char *GetTypeString(Type_t type);
    static int IsTypeProximity(Type_t type);
    static int IsTypeCPA(Type_t type);
    static Operator_t GetOperator(unsigned int index);
    static const char *GetOperatorString(Operator_t op);
    static int IsArgOperatorValid(Type_t type, Operator_t op);
    static int IsArgValid(Type_t type);
    static int IsArgValue(Type_t type);
    static int IsArgValueBool(Type_t type);
    static int IsArgValueTrackSecondaryID(Type_t type);
    static int IsArgValueTrackClassCode(Type_t type);
    static int IsArgValueTrackGroupID(Type_t type);
    static int IsArgValueTrackSecondaryType(Type_t type);
    static int IsArgValueSpeed(Type_t type);
    static int IsArgValueCourse(Type_t type);
    static int IsArgValueDistance(Type_t type);
    static int IsArgValueAltitude(Type_t type);
    static int IsArgValueRefPointID(Type_t type);
    static int IsArgValueTrackCommsCap(Type_t type);
    static int IsArgValueTrackFlightStatus(Type_t type);
    static int IsArgValueAISShipType(Type_t type);
    static int IsArgValueAISNavStatus(Type_t type);
    static int IsArgValueTimestamp(Type_t type);
    static int IsArgValueDimension(Type_t type);
    static int IsArgValueAcceleration(Type_t type);
    static int IsArgValueRateOfTurn(Type_t type);
    static int IsArgValuePercentage(Type_t type);
    static int IsArgValueTrackThreatLevel(Type_t type);
    static int IsArgString(Type_t type);
    static int IsArgStringTrackDesc(Type_t type);
    static int IsArgStringShape(Type_t type);
    static int IsArgStringShapeArea(Type_t type);
    static int IsArgStringShapeGate(Type_t type);
    static int IsArgStringShapePoint(Type_t type);
    static int IsArgStringShapeProx(Type_t type);
    static int IsArgShapeValid(Type_t type, SPxAreaObj *shape);
    static int IsArgTrackAttribute(Type_t type);
    static int IsArgAISAttribute(Type_t type);
    static int IsArgADSBAttribute(Type_t type);
    static int IsTimeLimitValid(Type_t type);
    static int IsTimeLimitOperatorValid(Type_t type, Operator_t op);
    static int IsRangeLimitValid(Type_t type);
    static int IsRangeLimitOperatorValid(Type_t type, Operator_t op);
    static int HasBuddy(Type_t type);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Constructor and destructor. */
    SPxCondition(SPxConditionGroup *group, int isFilter);
    virtual ~SPxCondition(void);

    /* General. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode reset(void);
    virtual SPxErrorCode initialise(Type_t type, const char *argString);
    virtual SPxErrorCode setBuddy(SPxCondition *buddy, int isPrimary);
    virtual int isConditionValid(void);
    virtual void doBackgroundProc(void);
    virtual int test(SPxUniTrack *track, Trigger_t *triggerRtn);
    virtual int haveShapeName(void);

    /* Used to build argument string. */
    virtual SPxErrorCode getArgDisplayString(double value, 
                                             char *buf, 
                                             unsigned int bufLen);

    virtual SPxErrorCode getArgDisplayString(char *buf, 
                                             unsigned int bufLen);

    /* CPA/TCPA utility functions. */
    virtual int testCPATracks(SPxUniTrack *track1, 
                              SPxUniTrack *track2, 
                              Trigger_t *triggerRtn);
    virtual int testCPA(double lat1Degs,
                        double long1Degs,
                        double speed1Mps,
                        double course1Degs,
                        double lat2Degs,
                        double long2Degs,
                        double speed2Mps,
                        double course2Degs,
                        SPxUniTrack *track2,
                        Trigger_t *triggerRtn);

    /* Proximity utility functions. */
    virtual int testProxTracks(SPxUniTrack *track1, 
                               SPxUniTrack *track2, 
                               Trigger_t *triggerRtn);

    /* Config load/save. */
    virtual SPxErrorCode load(FILE *fPtr);
    virtual SPxErrorCode save(FILE *fPtr);

    /* Implement these functions in a derived class. */
    virtual SPxErrorCode reportChangeEvent(ChangeFlags_t flags)=0;

    /* Optionally use/override these functions in a derived class. */
    virtual SPxErrorCode setArgString(const char *value, int callHandlers);
    virtual SPxErrorCode setArgValue(double value, int callHandlers);
    virtual void setArgDouble(double value);
    virtual double getArgDouble(void) const { return m_argValue; }
    virtual void setArgShape(SPxAreaObj *shape);
    virtual SPxAreaObj *getArgShape(void) { return m_argShape; }
    virtual void resetArgShape(void);
    virtual void setArgRefPoint(SPxRefPoint *refPoint);
    virtual SPxRefPoint *getArgRefPoint(void) { return m_argRefPoint; }
    virtual SPxUniTrack *getArgTrack(void);
    virtual const char *getArgTrackDescriptor(void);

    /* Alarm specific functions to override in a derived class. */
    virtual int canTrackTriggerAlarm(SPxUniTrack * /*track*/) { return TRUE; }
    virtual int hasTrackTriggeredAlarm(SPxUniTrack * /*track*/) { return FALSE; }

    virtual void updateDisarmed(int isDisarmed);

private:

    /*
     * Private types.
     */
    struct TypeInfo;

    /* Passed to a track iterator functions. */
    typedef struct
    {
        SPxCondition *obj;          /* Condition object. */
        SPxUniTrack *track1;        /* Trigger track. */
        SPxUniTrack *track2;        /* Second track. */
        int isTrue;                 /* Result of test. */
        Trigger_t *triggerRtn;      /* Returned trigger info. */

    } TrackTestInfo_t;

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    mutable SPxCriticalSection m_mutex; /* Mutex. */
    UINT32 m_id;                        /* Condition ID. */
    SPxUniTrackDatabase *m_trackDB;     /* Track database. */
    SPxConditionGroup *m_group;         /* Parent condition group. */
    int m_isFilter;                     /* Is filter condition? */
    SPxTime_t m_resetTime;              /* Last reset time. */
    Type_t m_type;                      /* Type. */
    double m_argValue;                  /* Argument value. */
    SPxAreaObj *m_argShape;             /* Argument shape object. */
    SPxRefPoint *m_argRefPoint;         /* Argument ref point object. */
    Operator_t m_argOp;                 /* Argument operator. */
    double m_timeLimitSecs;             /* Time limit in seconds. */
    Operator_t  m_timeLimitOp;          /* Time limit operator. */
    double m_rangeLimitMetres;          /* Range limit in metres. */
    Operator_t m_rangeLimitOp;          /* Range limit operator. */
    int m_isExclusion;                  /* Is exclusion condition? */
    SPxCondition *m_buddy;              /* Buddy condition. */
    int m_isPrimary;                    /* Is this the primary when paired? */
    RestrictionFlags_t m_restrictions;  /* Restriction flags. */
    int m_isDisarmed;                   /* Is disarmed? */
    int m_isEnabled;                    /* Is action enabled? */

    /*
     * Private static variables.
     */
    static TypeInfo Types[SPxCondition::NUM_TYPES];

    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode handleChange(ChangeFlags_t flags);
    int isArgOperatorValid(Type_t type, Operator_t op);
    int testRange(double metres);
    int testValue(double value, Trigger_t *triggerRtn);
    int testAreaInside(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testAreaVisited(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testAreaNotLeft(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testAreaStarted(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testGateCrossed(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrack(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackSecondaryID(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackSpeed(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackCourse(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackAltitude(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackClass(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackGroup(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackSecondaryType(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackOnGround(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackIsTest(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackIsSimulated(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackCommsCap(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackFlightStatus(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackAcceleration(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackRateOfTurn(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackSpeedDeviation(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackCourseDeviation(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackThreatLevel(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackAISShipType(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackAISNavStatus(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackAISHazardousCargo(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTrackAISDraught(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testProxToRefPoint(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testProxToAnyAIS(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testProxToAnyADSB(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testProxToTrack(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testProxToAnyTrack(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testProxToLand(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testProxToShape(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testApproachLand(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testCPAToPoint(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testCPAToRefPoint(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testCPAToTrack(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testCPAToAnyTrack(SPxUniTrack *track, Trigger_t *triggerRtn);
    int testTimestamp(Trigger_t *triggerRtn);
    SPxRadarTrack *getRadarTrack(SPxUniTrack *track);
    SPxAISTrack *getAISTrack(SPxUniTrack *track);
    SPxADSBTrack *getADSBTrack(SPxUniTrack *track);

    /*
     * Private static functions.
     */
    static int testTrackProxToAISItr(void *invokingObject, void *userObject, void *arg);
    static int testTrackProxToADSBItr(void *invokingObject, void *userObject, void *arg);
    static int testTrackCPAItr(void *trackDBVoid, void *infoVoid, void *trackVoid);
    static int testTrackProxItr(void *trackDBVoid, void *infoVoid, void *trackVoid);

}; /* SPxCondition */

#endif /* SPX_CONDITION_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
