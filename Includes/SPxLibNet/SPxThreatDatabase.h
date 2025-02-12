/*********************************************************************
 *
 * (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxThreatDatabase class which supports the
 *   threat classification and management of tracks received
 *   from multiple sources.
 *
 * Revision Control:
 *   26/11/18 v1.54   SP  Remove area argument from IterateThreats().
 *
 * Previous Changes:
 *   23/11/18 1.53   SP  Add IterateThreats().
 *   20/11/18 1.52   AGC Add default target height support.
 *   25/10/18 1.51   SP  Use new SPxRFTargetDatabase class.
 *   08/10/18 1.51   AGC Support Spexer classification.
 *   18/09/18 1.50   AGC Allow include/skip of preserved threats.
 *   14/08/18 1.49   SP  Support L1 track selection.
 *   02/08/18 1.48   BTB Build fix.
 *   02/08/18 1.47   BTB Store whether selected camera is primary.
 *   16/07/18 1.46   SP  Support new alarm rules.
 *   02/07/18 1.45   AGC Support minimum trail dot interval.
 *   26/01/18 1.44   SP  Add parameter to control RF target fusion.
 *   25/01/18 1.43   SP  Support logging of RF targets to file.
 *   24/01/18 1.42   SP  Support fusion of radar tracks and RF targets.
 *   09/01/18 1.41   SP  Add support for RF target detector.
 *   13/11/17 1.40   SP  Optionally disable event in SetSelectedCamera().
 *   01/11/17 1.39   SP  Various enhancements to CSV file output.
 *   18/10/17 1.38   SP  Rename EventDragDrop_t to EventDragDropInfo_t.
 *   06/10/17 1.37   SP  Support drag and drop.
 *   11/09/17 1.36   AGC Add configurable heights for provisional air targets.
 *   31/08/17 1.35   AGC Support provisional threat types for air targets.
 *   20/04/17 1.34   SP  Support provisonal threat type for AIS tracks.
 *                       Rename and combine some functions.
 *   12/04/17 1.33   SP  Support update of threat from secondary data.
 *   09/03/17 1.32   SP  Include both area name an pointer in alarm events.
 *   08/03/17 1.31   SP  Support type specific severity.
 *                       Use area name (not pointer) in alarm events.
 *                       Add EVENT_THREAT_ALARM_INSIDE.
 *   21/02/17 1.30   SP  Add GetNextThreat().
 *                       Support provisional threat type.
 *                       Add EVENT_THREAT_DELETE event.
 *   31/01/17 1.29   SP	 Support set initial threat type from track class.
 *   06/01/17 1.28   REW Support storage of user colours.
 *   16/11/16 1.27   SP  Support configurable threat types.
 *   23/08/16 1.26   AGC Improve gate support.
 *   27/07/15 1.25   REW Add pixelsPerMetre to GetNearestCamera().
 *   17/07/15 1.24   SP  Support gate alarms.
 *   15/07/15 1.23   SP  Add support for gates.
 *   02/03/15 1.22   SP  Add mutexAlreadyLocked arg to 
 *                       SetSelectedThreat().
 *   16/02/15 1.21   AGC Add support for database level alarm area management.
 *   22/08/14 1.20   SP  Support logging threats to CSV file 
 *                       for testing.
 *   20/06/14 1.19   AGC Move C++ headers to source file.
 *   17/02/14 1.18   SP  Support alarm segments.
 *   15/01/14 1.17   SP  Add flag to disable threat level colours.
 *   09/12/13 1.16   SP  Rename function.
 *   06/12/13 1.15   SP  Changes to support C2 operation.
 *   15/11/13 1.14   SP  Support threat update event reporting.
 *   08/11/13 1.13   SP  Handle alarm poly deletion.
 *                       Add threat update event.
 *   04/11/13 1.12   SP  Support alarm zones.
 *   21/10/13 1.11   SP  Add own lat/long position.
 *   15/10/13 1.10   REW Avoid compiler warning.
 *   15/10/13 1.9    SP  Add support for region editing.
 * 			 Remove internal region linked list.
 *   04/10/13 1.8    AGC Simplify headers.
 *   27/09/13 1.7    SP  Derive class from SPxTrackDatabase.
 *                       Change CreateCamera() to AddCamera().
 *   09/09/13 1.6    REW Fix compiler warning.
 *   09/09/13 1.5    SP	 Add ruler support.
 * 			 Improvements to event reporting.
 *   28/08/13 1.4    SP	 Add snapshot support.
 *   23/08/13 1.3    SP	 Remove level weightings.
 *			 Split velocity weighting into speed and direction.
 *			 Add position selection.
 *			 Add alert region support.
 *   14/08/13 1.2    REW Fix compiler errors.
 *   14/08/13 1.1    SP	 Initial version.
 **********************************************************************/

#ifndef _SPX_THREAT_DATABASE_H
#define _SPX_THREAT_DATABASE_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCamera.h"
#include "SPxLibUtils/SPxGate.h"
#include "SPxLibUtils/SPxRefPoint.h"
#include "SPxLibUtils/SPxImage.h"
#include "SPxLibNet/SPxTrackDatabase.h"
#include "SPxLibNet/SPxThreat.h"
#include "SPxLibData/SPxProcessRegion.h"

/*********************************************************************
 *
 *   Constants
 *
 **********************************************************************/

/* User-defined colours. */
#define	SPX_THREAT_DB_MAX_USER_COLOURS		8
#define	SPX_THREAT_DB_USER_COLOUR_NOT_SET	0x00000000


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
class SPxActiveRegion;
class SPxRFTargetDatabase;

/*
 * Define our class, derived from the generic packet decoder.
 */
class SPxThreatDatabase : public SPxTrackDatabase
{
public:

    /*
     * Public types.
     */

    /* Selection state. */
    typedef enum
    {
        STATE_NONE = 0,     /* Not set - must be zero. */
        STATE_SELECT,       /* Select a track. */
        STATE_DESELECT,     /* Deselect a track. */
        STATE_TOGGLE,       /* Toggle track selection. */
        STATE_SELECT_L1     /* Level 1 user-defined selection. */
    
    } State_t;

    /* Notification event type. */
    typedef enum
    {
        EVENT_THREAT_UPDATE,        /* Threat data has been updated. */
        EVENT_THREAT_SELECT,        /* Threat selected or deselected. */
        EVENT_THREAT_EDIT,          /* Threat to edit or finish editing. */
        EVENT_THREAT_SNAPSHOT,      /* Take snapshot of threat. */
        EVENT_THREAT_ALARM_ENTER,   /* Threat has triggered an area enter alarm. */
        EVENT_THREAT_ALARM_EXIT,    /* Threat has triggered an area exit alarm. */
        EVENT_THREAT_ALARM_INSIDE,  /* Threat has triggered an inside area alarm. */
        EVENT_THREAT_ALARM_OUTSIDE, /* Threat has triggered an outside area alarm. */
        EVENT_THREAT_DELETE,        /* Threat has been deleted by tracker. */
        EVENT_CAMERA_SELECT,        /* Camera selected or deselected. */
        EVENT_POSITION_SELECT,      /* Position in PPI window selected. */
        EVENT_RULER,                /* Ruler start and/or end point changed. */
        EVENT_REGION_START_EDIT,    /* Start region editing. */
        EVENT_REGION_FINISH_EDIT,   /* Finish region editing. */
        EVENT_GATE_CROSS,           /* Gate boundary has been crossed. */
        EVENT_GATE_ALARM,           /* Gate alarm triggered. */
        EVENT_DRAG_DROP             /* Object dragged and dropped on to another object. */

    } EventType_t;

    /* Threat event info. Used by:
     *  EVENT_THREAT_SELECT
     *  EVENT_THREAT_EDIT
     *  EVENT_THREAT_SNAPSHOT
     *  EVENT_THREAT_DELETE
     */
    typedef struct
    {
        SPxThreat *threat;  /* Threat handle or NULL. */
        State_t state;      /* Selection state (EVENT_THREAT_SELECT only). */

    } EventThreatInfo_t;

    /* Threat event info. Used by EVENT_THREAT_UPDATE. */
    typedef struct
    {
        SPxThreat *threat;  /* Threat handle. */
        UINT32 changeFlags; /* What has changed. */
        char oldName[SPX_MAX_THREAT_NAME_LENGTH]; /* Old name when changed. */

    } EventThreatUpdateInfo_t;

    /* Threat alarm event info. Used by:
     *  EVENT_THREAT_ALARM_ENTER
     *  EVENT_THREAT_ALARM_EXIT
     *  EVENT_THREAT_ALARM_INSIDE
     *  EVENT_THREAT_ALARM_OUTSIDE
     *
     * Take care when using the area handle as this is only guaranteed to
     * be valid in the callback which the active region list is locked. 
     * Outside of the callback the area could be deleted. If in doubt then
     * use the area name and get the area as and when needed with the mutex
     * locked.
     */
    typedef struct
    {
        SPxThreat *threat;          /* Threat handle. */
        SPxShapeArea *area;         /* Alarm area handle (see note). */
        char areaName[256];         /* Alarm area name. */
        int isAlarm;                /* Is this an alarm (used for backward compatibility). */

    } EventThreatAlarmInfo_t;

    /* Camera event info. Used by EVENT_CAMERA_SELECT. */
    typedef struct
    {
        SPxCamera *camera;  /* Camera handle or NULL. */

    } EventCameraInfo_t;

    /* Position event info. Used by EVENT_POSITION_SELECT. */
    typedef struct
    {
        SPxLatLong_t latLong;   /* Lat/long if set. */
        int isLatLongSet;       /* TRUE if lat/long is set. */

    } EventPositionInfo_t;

    /* Ruler event info. Used by EVENT_RULER. */
    typedef struct
    {
        int isStartSet;     /* Is start point set? */
        int isEndSet;       /* Is end point set? */

    } EventRulerInfo_t;

    /* Region event info. Used by:
     *  EVENT_REGION_START_EDIT
     *  EVENT_REGION_FINISH_EDIT
     */
    typedef struct
    {
        SPxActiveRegion *region;  /* Region handle or NULL. */

    } EventRegionInfo_t;

    /* Gate event info. Used by:
     *  EVENT_GATE_CROSS
     *  EVENT_GATE_ALARM
     */
    typedef struct
    {
        SPxGate *gate;      /* Gate. */
        SPxThreat *threat;  /* Threat handle or NULL. */

    } EventGateInfo_t;

    /* Drag and drop event info. Used by:
     *  EVENT_DRAG_DROP
     */
    typedef struct
    {
        UINT32 flags;       /* Mouse control flags, SPX_RENDERER_FLAGS_LEFT, etc. */
        int x;              /* Mouse X position. */
        int y;              /* Mouse Y position. */
        SPxObj *dragObj;    /* Object that was dragged. */
        SPxObj *dropObj;    /* Object that dragged object was dropped on to. */

    } EventDragDropInfo_t;

    /* Union of all event info structures. */
    typedef union
    {
        EventThreatInfo_t threatEvt;
        EventThreatUpdateInfo_t updateEvt;
        EventThreatAlarmInfo_t alarmEvt;
        EventCameraInfo_t cameraEvt;
        EventPositionInfo_t positionEvt;
        EventRulerInfo_t rulerEvt;
        EventRegionInfo_t regionEvt;
        EventGateInfo_t gateEvt;
        EventDragDropInfo_t dragDropEvt;
    
    } EventInfo_t;

    /* Top-level event structure. Use type to determine the event type 
     * and then access one (and only one) of the event info structures 
     * that follow.
     */
    typedef struct
    {
        EventType_t type;   /* Event type. */
        EventInfo_t info;   /* Event info. */
    
    } Event_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxThreatDatabase(unsigned int maxNumTrailPts=500,
                               double minTrailIntervalSecs=0.0);
    virtual ~SPxThreatDatabase(void);
    virtual SPxErrorCode Create(SPxRemoteServer *remoteServer);
    virtual SPxErrorCode Create(SPxRemoteServer *remoteServer,
                                SPxRFTargetDatabase *rfDatabase);

    /* Threat levels. */
    const char *GetLevelName(SPxThreat::Level_t level);

    /* Threat types. */
    unsigned int GetNumTypesInUse(void);
    SPxErrorCode SetTypeName(SPxThreat::Type_t type, const char *name);
    const char *GetTypeName(SPxThreat::Type_t type);
    SPxErrorCode AddThreatImage(SPxImage *image, const char *tag);
    SPxErrorCode SetTypeImage(SPxThreat::Type_t type, const char *tagOrFilename);
    SPxErrorCode SetTypeImage(SPxThreat::Type_t type, SPxImage *image);
    SPxImage *GetTypeImage(SPxThreat::Type_t type);
    const char *GetTypeImageTagOrFilename(SPxThreat::Type_t type);
    SPxErrorCode SetTypeSeverity(SPxThreat::Type_t type, double severity);
    double GetTypeSeverity(SPxThreat::Type_t type);
    SPxErrorCode SetTypeDefaultHeight(SPxThreat::Type_t type, double defaultHeight);
    double GetTypeDefaultHeight(SPxThreat::Type_t type);
    SPxErrorCode SetTypeSpexerClasses(SPxThreat::Type_t type, const char *spexerClasses);
    SPxErrorCode GetTypeSpexerClasses(SPxThreat::Type_t type,
				      char *buf, unsigned int bufLen);

    /* Our own lat/long position. */
    virtual SPxErrorCode SetOwnLatLong(const SPxLatLong_t *ll);
    virtual SPxErrorCode GetOwnLatLong(SPxLatLong_t *ll);

    /* Add/get camera. */
    virtual SPxErrorCode AddCamera(SPxCamera *camera);
    virtual unsigned int GetNumCameras(void);
    virtual SPxCamera *GetCamera(unsigned int index);

    /* Add/get gate. */
    virtual SPxErrorCode AddGate(SPxGate *gate);
    virtual SPxErrorCode RemoveGate(SPxGate *gate);
    virtual unsigned int GetNumGates(void);
    virtual SPxGate *GetGate(unsigned int index);

    /* Add/get reference point. */
    virtual SPxErrorCode AddRefPoint(SPxRefPoint *refPoint);
    virtual unsigned int GetNumRefPoints(void);
    virtual SPxRefPoint *GetRefPoint(unsigned int index);
    virtual SPxRefPoint *GetRefPointFromID(UINT32 id);

    /* Gate alarm. */
    void SetGateAlarm(SPxGate *gate, int isAlarm);
    void ClearGateAlarms(double minAgeSecs=0.0);

    /* Region editing. */
    void StartRegionEdit(SPxActiveRegion *region=NULL);
    void FinishRegionEdit(void);

    /* Get threats. */
    virtual unsigned int GetTopThreats(SPxThreat *threats[], 
                                       unsigned int maxNumThreats,
                                       int incIgnoreThreats=TRUE,
                                       int incFusedRFTargets=FALSE);

    /* Threat selection. */
    void SetSelectedThreat(SPxThreat *threat, State_t state,
                           int mutexAlreadyLocked=FALSE);
    SPxThreat *GetSelectedThreat(void) { return m_selectedThreat; }

    /* Threat editing. */
    void SetEditThreat(SPxThreat *threat);
    SPxThreat *GetEditThreat(void) { return m_editThreat; }

    /* Threat update reporting. */
    SPxErrorCode ReportThreatUpdate(SPxThreat *threat, 
                                    UINT32 changeFlags,
                                    const char *oldName=NULL);

    /* Camera selection. */
    void SetSelectedCamera(SPxCamera *camera, State_t state, int noEvent=FALSE);
    SPxCamera *GetSelectedCamera(void) { return m_selectedCamera; }
    void SetIsCameraPrimary(int isPrimary) { m_isSelectedPrimary = isPrimary; }
    int GetIsCameraPrimary(void) { return m_isSelectedPrimary; }

    /* Position selection. */
    void SetSelectedPosition(const SPxLatLong_t *latLong, State_t state);
    int GetSelectedPosition(SPxLatLong_t *latLong);
    
    /* Snapshots. */
    void SnapshotThreat(SPxThreat *threat);

    /* User-defined colours (for users of the class, not the class itself). */
    SPxErrorCode SetUserColourARGB(unsigned int idx, UINT32 argb);
    UINT32 GetUserColourARGB(unsigned int idx);
    SPxErrorCode SetUserColourName(unsigned int idx, const char *name);
    const char *GetUserColourName(unsigned int idx);

    /* Threat level colours. */
    void SetUseLevelColours(int state) { m_useLevelColours = state; }
    int GetUseLevelColours(void) { return m_useLevelColours; }
    SPxErrorCode SetLevelColour(SPxThreat::Level_t level, UINT32 argb);
    UINT32 GetLevelColour(SPxThreat::Level_t level);
    SPxErrorCode SetProvisionalColour(UINT32 argb);
    UINT32 GetProvisionalColour(void) { return m_provisionalARGB; }

    /* Threat severity weightings. */
    SPxErrorCode SetRangeWeighting(double weighting);
    double GetRangeWeighting(void) { return m_rangeWeighting; }

    SPxErrorCode SetSpeedWeighting(double weighting);
    double GetSpeedWeighting(void) { return m_speedWeighting; }

    SPxErrorCode SetDirectionWeighting(double weighting);
    double GetDirectionWeighting(void) { return m_directionWeighting; }

    /* Ruler. */
    SPxErrorCode SetRulerStart(SPxThreat *threat, const SPxLatLong_t *ll=NULL);
    SPxThreat *GetRulerStartThreat(void) { return m_rulerStartThreat; }
    int GetRulerStartLatLong(SPxLatLong_t *llRtn);
    int IsRulerStartSet(void) { return GetRulerStartLatLong(NULL); } 
    SPxErrorCode SetRulerEnd(SPxThreat *threat, const SPxLatLong_t *ll=NULL);
    SPxThreat *GetRulerEndThreat(void) { return m_rulerEndThreat; }
    int GetRulerEndLatLong(SPxLatLong_t *llRtn);
    int IsRulerEndSet(void) { return GetRulerEndLatLong(NULL); } 
    void ResetRuler(void);

    /* Maximum threat range. */
    void SetMaxThreatRangeMetres(double metres) { m_maxThreatRangeMetres = metres; }
    double GetMaxThreatRangeMetres(void) { return m_maxThreatRangeMetres; }

    /* Maximum threat speed. */
    void SetMaxThreatSpeedMps(double mps) { m_maxThreatSpeedMps = mps; }
    double GetMaxThreatSpeedMps(void) { return m_maxThreatSpeedMps; }

    /* Threat event callbacks. */
    SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, 
                                  void *userObj);

    SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, 
                                     void *userObj);

    SPxErrorCode ReportEvent(Event_t *evt);

    SPxErrorCode ReportDragDrop(UINT32 flags,
                                int x, int y,
                                SPxObj *dragObj, 
                                SPxObj *dropObj);

    void LogAlarmThreatToFile(SPxThreat *threat, 
                              int hasEnteredAlarmArea,
                              int isInAlarmArea,
                              int hasExitedAlarmArea,
                              int isDeleted);

    /* Nearest objects to a specified point. */
    SPxCamera *GetNearestCamera(double latDegs,
				double longDegs,
				double *distMetresRtn,
				double pixelsPerMetre = 0.0) const;

    SPxThreat *GetNearestThreat(double latDegs,
				double longDegs,
				double *distMetresRtn,
				int includePreserved = FALSE) const;

    SPxGate *GetNearestGate(double latDegs,
			    double longDegs,
			    double *distMetresRtn) const;

    /* How threat information is updated from track reports. */
    void SetThreatFromTrack(int state) {m_setThreatFromTrack = state;}
    void SetThreatNameFromAIS(int state) {m_setNameFromAIS = state;}
    void SetThreatProvisionalTypeFromTrackClass(int state) { m_setProvisionalTypeFromTrackClass = state;}
    SPxErrorCode SetProvisionalTypeForRFDrone(SPxThreat::Type_t type);
    SPxThreat::Type_t GetProvisionalTypeForRFDrone(void) const { return m_provisionalTypeForRFDrone; }
    SPxErrorCode SetProvisionalTypeForRFRemote(SPxThreat::Type_t type);
    SPxThreat::Type_t GetProvisionalTypeForRFRemote(void) const { return m_provisionalTypeForRFRemote; }
    SPxErrorCode SetProvisionalTypeForAIS(SPxThreat::Type_t type);
    SPxThreat::Type_t GetProvisionalTypeForAIS(void) const { return m_provisionalTypeForAIS; }
    SPxErrorCode SetProvisionalTypeForLowAirTarget(SPxThreat::Type_t type);
    SPxThreat::Type_t GetProvisionalTypeForLowAirTarget(void) const { return m_provisionalTypeForLowAirTarget; }
    SPxErrorCode SetProvisionalTypeForHighAirTarget(SPxThreat::Type_t type);
    SPxThreat::Type_t GetProvisionalTypeForHighAirTarget(void) const { return m_provisionalTypeForHighAirTarget; }
    SPxErrorCode SetLowAirTargetMinHeight(double heightMetres);
    double GetLowAirTargetMinHeight(void) const { return m_lowAirTargetMinHeightMetres; }
    SPxErrorCode SetHighAirTargetMinHeight(double heightMetres);
    double GetHighAirTargetMinHeight(void) const { return m_highAirTargetMinHeightMetres; }

    /* Hold-off remote configuration. */
    SPxErrorCode HoldOffRemoteUpdate(unsigned int secs);

    /* Get threat from track ID. */
    SPxThreat *GetThreat(UINT32 id);

    /* Get next threat based on severity. */
    SPxThreat *GetNextThreat(SPxThreat *threat,
                             int nextHighest,
                             int allowWrap);

    /* Iterate through all threats. */
    SPxErrorCode IterateThreats(SPxCallbackListFn_t threatFn, 
                                void *userArg);

    /* SPxThreat calls these to manage area deletion callbacks. */
    SPxErrorCode AddAlarmArea(SPxShapeArea *area);
    SPxErrorCode RemoveAlarmArea(SPxShapeArea *area);

    /*
     * Public static functions.
     */

    static int AlarmAreaDeletionCallback(void *invokingObject, 
                                         void *userObject, 
                                         void *arg);

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

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Optional RF target database. */
    SPxRFTargetDatabase *m_rfDatabase;

    /* Mutex used to lock the threat lists. */
    mutable SPxCriticalSection m_threatListMutex;

    /* Mutex used to lock the camera list. */
    mutable SPxCriticalSection m_cameraListMutex;

    /* Mutex used to lock the region list. */
    SPxCriticalSection m_regionListMutex;

    /* Mutex used to lock the alarm area list. */
    SPxCriticalSection m_alarmAreaListMutex;

    /* Mutex used to lock the gate list. */
    mutable SPxCriticalSection m_gateListMutex;

    /* Mutex used to lock the reference point list. */
    mutable SPxCriticalSection m_refPointListMutex;

    /* Event callback list. */
    SPxCallbackList *m_eventCBList;
    SPxCriticalSection m_eventMutex;

    /* Selected threats. */
    SPxThreat *m_selectedThreat;
    SPxThreat *m_editThreat;

    /* Selected camera. */
    SPxCamera *m_selectedCamera;
    int m_isSelectedPrimary;

    /* Selected position. */
    SPxLatLong_t m_selectedLatLong;
    int m_isLatLongSelected;

    /* User-defined colours. */
    UINT32 m_userColourARGBs[SPX_THREAT_DB_MAX_USER_COLOURS];
    char *m_userColourNames[SPX_THREAT_DB_MAX_USER_COLOURS];

    /* Threat level colours. */
    UINT32 m_levelARGBs[SPxThreat::NUM_LEVELS];
    int m_useLevelColours;          /* Are level colours in use? */
    UINT32 m_provisionalARGB;       /* Provisional threat colour. */

    /* Threat severity weightings (0.0 to 1.0). */
    double m_rangeWeighting;        /* Distance from radar. */
    double m_speedWeighting;        /* Target speed. */
    double m_directionWeighting;    /* Target direction. */

    /* Provisional threat types. */
    SPxThreat::Type_t m_provisionalTypeForRFDrone;         /* Type to use for RF drone targets. */
    SPxThreat::Type_t m_provisionalTypeForRFRemote;        /* Type to use for RF remote control targets. */
    SPxThreat::Type_t m_provisionalTypeForAIS;             /* Type to use for AIS. */
    SPxThreat::Type_t m_provisionalTypeForLowAirTarget;    /* Type to use for low air targets. */
    SPxThreat::Type_t m_provisionalTypeForHighAirTarget;   /* Type to use for high air targets. */
    double m_lowAirTargetMinHeightMetres;                  /* Min height for low air targets. */
    double m_highAirTargetMinHeightMetres;                 /* Min height for high air targets. */

    /* Own position used in severity calculation. */
    SPxLatLong_t m_ownLL;

    /* Max range and speed used in severity calculation. */
    double m_maxThreatRangeMetres;
    double m_maxThreatSpeedMps;

    /* Ruler. */
    SPxLatLong_t m_rulerStartLL;    /* Ruler start lat/long. */
    SPxLatLong_t m_rulerEndLL;      /* Ruler end lat/long. */
    int m_isRulerStartLLSet;        /* Ruler start lat/long set? */
    int m_isRulerEndLLSet;          /* Ruler end lat/long set? */
    SPxThreat *m_rulerStartThreat;  /* Ruler start position threat. */
    SPxThreat *m_rulerEndThreat;    /* Ruler end position threat. */

    /* Update threat information from track reports? */
    int m_setThreatFromTrack;                 /* Update threat info from track? */
    int m_setProvisionalTypeFromTrackClass;   /* Update provisional type from track class? */
    int m_setNameFromAIS;                     /* Update name from secondary AIS data? */
    SPxTime_t m_holdOffRemoteUpdateEpoch;     /* Interval to hold-off remote update. */

    /* Logging of threats in alarm zones to CSV file for testing. */
    FILE *m_alarmLogFile;

    /* Logging of RF targets to CSV file for testing. */
    FILE *m_rfTargetLogFile;

    /* Next global ID. */
    UINT32 m_nextGlobalID;

    /* RF target fusion parameters. */
    unsigned int m_rfTargetFusionMaxMissedScans;

    /*
     * Private functions.
     */

    SPxErrorCode DeselectAllThreats(int mutexAlreadyLocked=FALSE);
    int IsThreatActive(SPxThreat *threat);
    void SetThreatActive(SPxThreat *threat, int isActive);
    void SortThreatsDescendingSeverity(void);
    void UpdateThreatFromTrack(SPxThreat *threat, SPxRadarTrack *track);
    int IsRemoteUpdateAllowed(void);
    void CheckRFTargetFusion(SPxThreat *threat);
    void LogRFTargetThreatToFile(SPxThreat *threat, 
                                 int isCreated,
                                 int isDeleted);
    void GetPositionStrings(SPxRadarTrack *track,
                            int useFiltered,
                            int posFmt,
                            char *posA,
                            unsigned int posALen,
                            char *posB,
                            unsigned int posBLen);
    void GetDegsMinsSecs(double degs, int isNorthSouth, 
                         char *buf, unsigned int bufLen);
    SPxThreat::Type_t GetRFTargetThreatType(SPxRFTarget *rfTarget);

    /*
     * Private static functions.
     */

    static bool CompareThreats(SPxThreat *threat1, SPxThreat *threat2);

    static int TrackCreationCallback(void *invokingObject,
                                     void *userObject,
                                     void *arg);

    static int TrackUpdateCallback(void *invokingObject,
                                   void *userObject,
                                   void *arg);

    static int RFTargetEventHandler(void *invokingObjPtr,
                                    void *userObjPtr,
                                    void *eventPtr);
        
}; /* SPxThreatDatabase */

/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* SPX_THREAT_DATABASE_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
