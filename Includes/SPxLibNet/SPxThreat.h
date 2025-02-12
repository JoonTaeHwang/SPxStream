/*********************************************************************
*
* (c) Copyright 2013 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxThreat class.
*
* Revision Control:
*   10/02/21 v1.40   AGC	Add GetRadarTrack().
*
* Previous Changes:
*   26/11/18 1.39   SP 	Improve access to alarm info.
*   23/11/18 1.38   SP 	Add IsHostile().
*   14/08/18 1.37   SP 	Add GetInsideAreaName().
*   18/07/18 1.36   SP 	Rename function.
*   16/07/18 1.35   SP 	Support new area alarm rules.
*   16/07/18 1.34   SP 	Add IterateAlarmAreas().
*   22/06/18 1.33   AGC	Fix new cppcheck warnings.
*   26/01/18 1.32   SP 	Rename variable.
*   24/01/18 1.31   SP 	Support RF target fusion.
*   09/01/18 1.30   SP 	Support RF target.
*   28/09/17 1.29   AGC	Fix mutex lock error.
*   11/09/17 1.28   AGC	Remove altitude functions.
*   22/06/17 1.27   AGC	Support threat altitude.
*   12/04/17 1.26   SP 	Add Set/GetSecondaryName().
*   08/03/17 1.25   SP 	Add IsTypeIgnore().
*   21/02/17 1.24   SP 	Support provisional threat type.
*                       Add GetColour().
*   31/01/17 1.23   SP 	Support timestamps on threat type.
*   22/11/16 1.22   SP 	Reorder threat types.
*   16/11/16 1.21   SP 	Support configurable threat types.
*   16/11/16 1.20   SP 	Fix version history.
*   20/04/16 1.19   AGC	Remove undefined static BuildPacketTrackThreat() function.
*   27/11/15 1.18   REW	Make more Get functions const.
*   26/11/15 1.17   REW	Support timestamps on threat level.
*   17/07/15 1.16   SP 	Add GetGateLastCrossingTime().
*   17/07/15 1.15   REW	Support SetUserData() etc.
*   15/07/15 1.14   SP 	Support handling of gate crossings.
*   02/03/15 1.13   SP 	Remove unused deletion handler.
*   17/02/14 1.12   SP 	Support alarm segments.
*   09/12/13 1.11   SP 	Add mutex.
*   06/12/13 1.10   SP 	Changes to support CxEye C2 operation.
*   08/11/13 1.9    SP 	Handle alarm poly deletion correctly.
*   04/11/13 1.8    SP 	Support alarm polys.
*   21/10/13 1.7    SP 	Changes to support per-polygon 
*                       threat weighting.
*   07/10/13 1.6    SP 	Use track ID in threat name.
*   04/10/13 1.5    AGC	Simplify headers.
*   05/09/13 1.4    SP 	Add UNCLASSIFIED threat level.
*   28/08/13 1.3    SP 	Add snapshot support.
*   14/08/13 1.2    REW	Fix compiler errors.
*   14/08/13 1.1    SP 	Initial version.
**********************************************************************/

#ifndef _SPX_THREAT_H
#define _SPX_THREAT_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxGate.h"
#include "SPxLibData/SPxProcessRegion.h"
#include "SPxLibData/SPxPackets.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Change flags used in update event. */
#define SPX_THREAT_CHANGE_NAME             (1U << 0)
#define SPX_THREAT_CHANGE_NAME_PREFIX      (1U << 1)
#define SPX_THREAT_CHANGE_GLOBAL_ID        (1U << 2)
#define SPX_THREAT_CHANGE_TYPE             (1U << 3)
#define SPX_THREAT_CHANGE_LEVEL            (1U << 4)

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

/* Forward-declare other classes we use in case headers are in wrong order. */
class SPxThreatDatabase;
class SPxRadarTrack;
class SPxRFTarget;
struct SPxThreatPriv;
struct SPxTime_tag;

/*
 * Define our class.
 */
class SPxThreat : public SPxObj
{

/* The database class can access our internals directly. */
friend class SPxThreatDatabase;

public:
    /*
     * Public types.
     */

     /* Threat type - do not change order or values. */ 
     typedef enum
     {
        /* Current threat types. */
        TYPE_UNKNOWN = 0,
        TYPE_1 = 1,
        TYPE_2 = 2,
        TYPE_3 = 3,
        TYPE_4 = 4,
        TYPE_5 = 5,
        TYPE_6 = 6,
        NUM_TYPES,    /* Must be last entry. */

        /* Fixed threat types. */
        TYPE_HELO = TYPE_1,
        TYPE_PLANE = TYPE_2,
        TYPE_CAR = TYPE_3,
        TYPE_TRUCK = TYPE_4,
        TYPE_ARMVECH = TYPE_5,
        TYPE_MAN = TYPE_6

     } Type_t;

     /* Threat level - do not change order or values. */ 
     typedef enum
     {
        LEVEL_UNCLASSIFIED = 0,
        LEVEL_FRIENDLY = 1,
        LEVEL_NEUTRAL = 2,
        LEVEL_HOSTILE = 3,
        NUM_LEVELS   /* Must be last entry. */

     } Level_t;

     /* Primary data type. */
     typedef enum
     {
         DATA_TYPE_RADAR_TRACK = 0,
         DATA_TYPE_RF_TARGET = 1

     } DataType_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by the SPxThreatDatabase.
     */

     DataType_t GetDataType(void) { return m_dataType; }
     SPxRadarTrack *GetRadarTrack(void) { return m_radarTrack; }
     const SPxRadarTrack *GetRadarTrack(void) const { return m_radarTrack; }
     SPxRFTarget *GetRFTarget(void) { return m_rfTarget; }
     SPxErrorCode SetThreatNamePrefix(const char *prefix);
     const char *GetThreatNamePrefix(void);
     SPxErrorCode SetSecondaryName(const char *name);
     const char *GetSecondaryName(void);
     SPxErrorCode SetGlobalID(UINT32 id);
     UINT32 GetGlobalID(void) const { return m_globalID; }
     const char *GetThreatName(void);
     SPxErrorCode  SetType(Type_t type, int isProvisional=FALSE);
     Type_t GetType(void) const { return m_type; } 
     const char *GetTypeName(void);
     const char *GetTypeStr(void) { return GetTypeName(); } /* Deprecated. */
     UINT32 GetTypeTimestampMsecs(void) const {return(m_typeMsecs);}
     UINT32 GetMsecsSinceType(void) const;
     int IsTypeProvisional(void) const { return m_isTypeProvisional; }
     int IsTypeIgnore(void);
     SPxErrorCode SetLevel(Level_t level);
     Level_t GetLevel(void) const { return m_level; } 
     const char *GetLevelName(void);
     const char *GetLevelStr(void) { return GetLevelName(); } /* Deprecated. */
     UINT32 GetLevelTimestampMsecs(void) const {return(m_levelMsecs);}
     int IsHostile(void) { return m_level == LEVEL_HOSTILE ? TRUE : FALSE; }
     UINT32 GetMsecsSinceLevel(void) const;
     UINT32 GetColour(void) const;
     SPxErrorCode SetIsSelected(int select);
     int GetIsSelected(void);
     void SetUserData(void *ptr) { m_userData = ptr; }
     void *GetUserData(void) { return(m_userData); }
     SPxErrorCode UpdateSeverity(void);
     double GetSeverity(void);
     unsigned int GetNumSnapshots(void) const { return m_numSnapshots;}
     int IsInsideAlarmArea(const char *areaName, int isAlarm);
     int HasExitedAlarmArea(const char *areaName, int isAlarm);
     SPxErrorCode GetPrimaryInsideAreaName(char *buf, unsigned int bufLen);
     int IsGateCrossed(SPxGate *gate);
     SPxErrorCode BuildPacketTrackThreat(SPxPacketTrackThreat *packet,
                                         UINT32 setterID=0);

     /* Gate crossing information. */
     SPxErrorCode RemoveAllCrossedGates(unsigned int *numCrossed, SPxTime_tag *crossingTime=NULL,
                                        unsigned int *preNumCrossed=NULL, SPxTime_tag *preCrossingTime=NULL,
                                        unsigned int *postNumCrossed = NULL, SPxTime_tag *postCrossingTime=NULL);
     
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

    /* Constructor and destructor. */
    SPxThreat(SPxThreatDatabase *threatDB, SPxRadarTrack *radarTrack);
    SPxThreat(SPxThreatDatabase *threatDB, SPxRFTarget *rfTarget);
    virtual ~SPxThreat(void);
    void AddSnapshot(void) { m_numSnapshots++; }

private:

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private types.
     */
    typedef struct
    {
        SPxShapeArea *area;         /* Area object. */
        int areaAlarmType;          /* Area alarm type. */
        int isAlarmTriggered;       /* Is alarm triggered in this area? */
        int isExitAlarmPending;     /* Trigger alarm on exit from this area? */
        UINT32 numUpdates;          /* Number of times track updated in area. */
        double averageSpeedMps;     /* Average track speed in area. */

    } AreaInfo_t;

    typedef struct
    {
        SPxGate *gate;              /* Gate object. */
        UINT32 lastCrossingTime;    /* Time gate was last crossed. */
        SPxTime_t lastCrossingEpoch;/* Epoch time gate was last crossed. */

    } GateInfo_t;

    /*
     * Private variables.
     */
    SPxCriticalSection m_mutex;     /* Object mutex. */
    SPxThreatDatabase *m_threatDB;  /* Track database . */
    DataType_t m_dataType;          /* Data type. */
    SPxRadarTrack *m_radarTrack;    /* Associated radar track. */
    SPxRFTarget *m_rfTarget;        /* Associated RF target. */
    UINT32 m_globalID;              /* Global threat ID. */
    Type_t m_type;                  /* Assigned threat type. */
    UINT32 m_typeMsecs;             /* Time when threat type was set. */
    int m_isTypeProvisional;        /* Is type set to a provisional value? */
    Level_t m_level;                /* Assigned threat level. */
    UINT32 m_levelMsecs;            /* Time when threat level was set. */
    double m_severity;              /* Severity level 0.0 to 1.0. */
    unsigned int m_numSnapshots;    /* Number of snapshots taken. */
    SPxCriticalSection m_areaListMutex; /* Mutex used to protect area lists. */
    mutable SPxCriticalSection m_crossedGateListMutex; /* Mutex used to protect crossed gate list. */
    unsigned int m_rfTargetFusionMissedScans; /* Number of scans fused RF target did not match track. */

    /*
     * Private functions.
     */

    void InitCommon(SPxThreatDatabase *threatDB);
    SPxErrorCode Reset(void);
    int UpdateThreatName(const char **oldName);
    SPxErrorCode SetTypeInternal(Type_t type, int isProvisional=FALSE);

    /* Alarm area handling. */
    void CheckAlarmAreas(void);
    void CheckAlarmAreaTrigger(AreaInfo_t *info, int isEnter);
    SPxErrorCode HandleAlarmAreaDelete(SPxShapeArea *area);
    double GetAlarmAreaWeighting(void);

    /* Gate handling. */
    void CheckGateCrossings(void);
    SPxGate *GetCrossedGate(unsigned int index) const;
    SPxErrorCode ReportCrossedGate(SPxGate *gate);
    SPxErrorCode RemoveCrossedGate(SPxGate *gate);
    UINT32 GetGateLastCrossingTime(SPxGate *gate) const;
    double GetCrossedGatesWeighting(void) const;
    unsigned int GetNumCrossedGates(void) const;

    /* User data. */
    void *m_userData;

    /*
     * Private static functions.
     */

}; /* SPxThreat */

#endif /* SPX_THREAT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
