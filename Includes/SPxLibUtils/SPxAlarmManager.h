/*********************************************************************
 *
 * (c) Copyright 2019 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmManager class which supports creation and 
 *   management of various predefined track alarms.
 *
 * Revision Control:
 *   10/02/21 v1.4    AGC	Support various alarm actions.
 *				Support area alarms.
 *				Support gate alarms.
 *				Support CPA alarms.
 *
 * Previous Changes:
 *   19/02/20 1.3  BTB 	Move parameter definitions to source file.
 *   19/12/19 1.2  BTB 	Fix to parameter name.
 *   13/12/19 1.1  BTB 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_ALARM_MANAGER_H
#define _SPX_ALARM_MANAGER_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"

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

/* Forward declare other classes we use. */
class SPxAlarm;
class SPxAlarmDatabase;
class SPxAreaObj;
class SPxNavData;
class SPxRefPoint;
class SPxUniTrackDatabase;

/*
 * Define our class.
 */
class SPxAlarmManager : public SPxObj
{
public:
    /*
     * Public types.
     */
    enum AlarmType
    {
        ALARM_ANY_TRACK_CPA = 0,
        ALARM_PLATFORM_CPA = 1,
        ALARM_PLATFORM_PROXIMITY = 2,
        ALARM_REF_POINT_CPA = 3,
        ALARM_REF_POINT_PROXIMITY = 4,
        ALARM_LAND_APPROACH = 5,
        ALARM_STATIONARY_TRACKS = 6,
        ALARM_FAST_TRACKS = 7,
        ALARM_HAZARDOUS_CARGO = 8,
        ALARM_IN_AREA = 9,
        ALARM_GATE_CROSSING = 10,
        ALARM_MAX
    };

    enum InAreaAlarmMode
    {
        MODE_SEPARATE_ALARM = 0,
        MODE_ALARM_CONDITION = 1
    };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxAlarmManager(SPxUniTrackDatabase *trackDB,
                             int alarmTracksAreBold = TRUE,
                             int alarmTracksAreRed = TRUE);
    virtual ~SPxAlarmManager(void);

    /* Platform location. */
    SPxErrorCode SetPlatformNavData(SPxNavData *navData);
    SPxErrorCode SetPlatformLatLong(double latDegs, double longDegs);

    /* Ref point. */
    SPxErrorCode SetRefPoint(SPxRefPoint *refPoint);

    SPxAlarmDatabase *GetAlarmDatabase(void) { return m_alarmDB; }
    SPxAreaObj *GetAreaDatabase(void);

    /* Alarm actions. */
    SPxErrorCode EnableHighlightTracks(int enable);
    int AreTracksHighlighted(void) const;
    SPxErrorCode SetHighlightTrackDurationSecs(unsigned int secs);
    unsigned int GetHighlightTrackDurationSecs(void) const;
    SPxErrorCode EnableHighlightShapes(int enable);
    int AreShapesHighlighted(void) const;
    SPxErrorCode SetHighlightShapesDurationSecs(unsigned int secs);
    unsigned int GetHighlightShapesDurationSecs(void) const;
    SPxErrorCode EnableAudibleAlarms(int enable);
    int AreAudibleAlarmsEnabled(void) const;
    SPxErrorCode SetAudibleAlarmOnce(int once);
    int IsAudibleAlarmOnce(void) const;
    SPxErrorCode SetLatchAlarms(int latchAlarms);
    int AreAlarmsLatched(void) const;
    SPxErrorCode SetAutoDeleteAlarms(int autoDeleteAlarms);
    int AreAlarmsAutoDeleted(void) const;

    /* Triggered alarm access. */
    unsigned int GetNumAlarms(void) const;
    unsigned int FetchAlarmList(SPxAlarm **listRtn, unsigned int listSize);
    int ListChangedSinceLastFetch(void) const;
    SPxErrorCode SetAlarmsState(SPxAlarm::State_t state, SPxUniTrack *track=NULL);
    SPxErrorCode DeleteAlarm(SPxAlarm *alarm);

    /* Type(s) of track to alarm on. */
    void SetAlarmOnRadarTracks(int state);
    int GetAlarmOnRadarTracks(void) const;

    void SetAlarmOnAISTracks(int state);
    int GetAlarmOnAISTracks(void) const;

    void SetAlarmOnADSBTracks(int state);
    int GetAlarmOnADSBTracks(void) const;

    void SetAlarmOnFusedTracks(int state);
    int GetAlarmOnFusedTracks(void) const;

    void ApproveTrack(SPxUniTrack *track);
    void UnapproveTrack(SPxUniTrack *track,
                        int retainIfPersistent=FALSE);
    int IsTrackApproved(SPxUniTrack *track) const;

    /* Enable state of managed alarms. */
    void EnableAlarm(AlarmType alarm, int enable);
    int IsAlarmEnabled(AlarmType alarm) const;
    int IsAlarmInState(AlarmType alarm, SPxUniTrack *track,
                       SPxAlarm::State_t state) const;
    void DisarmAlarm(AlarmType alarm, double timeSecs, const char *shape=NULL);
    void RearmAlarm(AlarmType alarm, const char *shape=NULL);
    int IsAlarmDisarmed(AlarmType alarm, const char *shape = NULL);

    void EnableInAreaAlarms(int enable);
    int AreInAreaAlarmsEnabled(void) const;

    void EnableGateCrossingAlarms(int enable);
    int AreGateCrossingAlarmsEnabled(void) const;

    /* Enable state of exclusion zones. */
    void EnableAISExclusion(int enable);
    int IsAISExclusionEnabled(void) const;

    void EnableADSBExclusion(int enable);
    int IsADSBExclusionEnabled(void) const;

    /* Configuration of managed alarms. */
    void SetAnyTrackCPARange(double rangeMetres);
    double GetAnyTrackCPARange(void) const;

    void SetAnyTrackCPATime(double timeSecs);
    double GetAnyTrackCPATime(void) const;

    void SetPlatformCPARange(double rangeMetres);
    double GetPlatformCPARange(void) const;

    void SetPlatformCPATime(double timeSecs);
    double GetPlatformCPATime(void) const;

    void SetPlatformProximity(double proximityMetres);
    double GetPlatformProximity(void) const;

    void SetRefPointCPARange(double rangeMetres);
    double GetRefPointCPARange(void) const;

    void SetRefPointCPATime(double timeSecs);
    double GetRefPointCPATime(void) const;

    void SetRefPointProximity(double proximityMetres);
    double GetRefPointProximity(void) const;

    void SetLandApproachRange(double rangeMetres);
    double GetLandApproachRange(void) const;
    void SetLandApproachTime(double timeSecs);
    double GetLandApproachTime(void) const;

    void SetMaxStationarySpeed(double speedMps);
    double GetMaxStationarySpeed(void) const;

    void SetMinFastSpeed(double speedMps);
    double GetMinFastSpeed(void) const;

    /* Alarm area configuration. */
    SPxErrorCode SetInAreaAlarmMode(InAreaAlarmMode mode);
    InAreaAlarmMode GetInAreaAlarmMode(void) const;
    SPxErrorCode AddInAreaAlarm(const char *area);
    SPxErrorCode RemoveInAreaAlarm(const char *area);

    /* Gate configuration. */
    SPxErrorCode AddGateCrossedAlarm(const char *gate, int isBidirectional);
    SPxErrorCode RemoveGateCrossedAlarm(const char *gate);
    unsigned int GetNumGateCrossedAlarms(void) const;
    int IsGateCrossedAlarm(const char *gate) const;
    int IsGateCrossedAlarmBidirectional(const char *gate) const;

    /* Configuration of exclusion zones. */
    void SetAISExclusionRange(double rangeMetres);
    double GetAISExclusionRange(void) const;

    void SetAISExclusionTime(double timeSecs);
    double GetAISExclusionTime(void) const;

    void SetADSBExclusionRange(double rangeMetres);
    double GetADSBExclusionRange(void) const;

    void SetADSBExclusionTime(double timeSecs);
    double GetADSBExclusionTime(void) const;

protected:

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private types.
     */
    struct AlarmInfo;

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxAlarmDatabase *m_alarmDB;

    /*
     * Private functions.
     */

    void createAlarmConfigs(void);
    SPxAlarmConfig *createAlarm(AlarmType alarmType,
                                AlarmInfo *existingAlarmInfo=NULL,
                                SPxErrorCode *err=NULL);

    void setAlarmOnTrackType(UINT32 trackType, int state);
    int getAlarmOnTrackType(UINT32 trackType) const;

    void addAlarmToList(SPxAlarm *alarm);

    void checkForAlarmChange(const SPxAlarmDatabase::Event_t *alarmEvent);

    /*
     * Private static functions.
     */
    static int addAlarmToListIterator(void *alarmDBVoid, 
                                      void *objVoid, 
                                      void *alarmVoid);

    static int alarmListChangeEventHandler(void *alarmDBVoid,
                                           void *objVoid,
                                           void *eventVoid);

    static int isAlarmInStateIterator(void *alarmDBVoid,
                                      void *objVoid,
                                      void *alarmVoid);

}; /* SPxAlarmManager */

#endif /* _SPX_ALARM_MANAGER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
