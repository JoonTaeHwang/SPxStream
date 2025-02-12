/*********************************************************************
 *
 * (c) Copyright 2017 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxUniTrack class, which unifies support for
 *   radar, fused, AIS and ADS-B tracks.
 *
 * Revision Control:
 *   26/04/22 v1.47  BTB	Add functions to handle threat level names.
 *
 * Previous Changes:
 *   28/02/22 1.46  BTB	Support checking for starting in an area.
 *   18/01/22 1.45  SP 	Add GetEstAcceleration() and GetEstRateOfTurn().
 *   04/06/21 1.44  AGC	Rework coastline approach checks.
 *   30/04/21 1.43  SP 	Default to FIELD_TYPE_PERSISTENT in GetDescriptor()
 *   20/04/21 1.42  SP 	Support time limit in IsInArea().
 *			Extend support for time limit in WasInArea() and IsGateCrossed().
 *   10/02/21 1.41  AGC	Add GetADSBCategory().
 *			Support marking cleared alarms.
 *			Support testing land conditions.
 *			Support key for user data.
 *   30/10/20 1.40  AGC	Return 12-bit info from GetIFFMode1().
 *			Return flight level support from GetAltitude().
 *			Add GetAsterixMsg().
 *			Extend FormatLabel() support.
 *   29/09/20 1.39  AGC	Improve persistent track descriptor handling.
 *   29/09/20 1.38  AGC	Add IsPlot() function.
 *   18/09/20 1.37  AGC	Add generic identifier accessor functions.
 *			Simplify track descriptor usage.
 *   27/08/20 1.36  AGC	Support including label offset in display pos.
 *   13/08/20 1.35  SP 	Support threat score timeout.
 *   05/08/20 1.34  SP 	Add FormatLabel() and Set/GetThreatScore().
 *   27/02/20 1.33  BTB	Allow suppression of creation error in GetDescriptor().
 *   19/12/19 1.32  BTB	Add Get/SetIsBold().
 *   25/06/19 1.31  REW	Add GetIsEmergency() and GetIsSPI().
 *   07/06/19 1.30  BTB	Add CheckTrackMatches().
 *   08/05/19 1.29  BTB Support checking for lingering in an area.
 *   03/05/19 1.28  SP 	Enhancements to track descriptor.
 *   02/05/19 1.27  REW	Add GetOnGround(), GetIsTest(), GetIsSimulated()
 *			GetSecondaryTrackTypeNameFromType().
 *   13/11/18 1.25  BTB Handle reload and deletion of all shapes.
 *   17/09/18 1.24  SP 	Add GetDescriptor().
 *   17/07/18 1.23  AGC	Support GetIsPreserved().
 *   30/05/18 1.22  SP 	Add GetClassColour().
 *   24/05/18 1.21  REW	Support GetIsDeleted().
 *   03/05/18 1.20  SP 	Add support for track filtering.
 *   01/05/18 1.19  SP 	Move area and gate logic here from SPxAlarmTrack.
 *   28/03/18 1.18  SP 	Add source index.
 *   27/03/18 1.17  SP 	Link track object to database.
 *                      Calculate track lat/long in GetDistanceFromPoint() if needed.
 *   15/03/18 1.16  SP 	Rename GetBestTrackName() to GetTrackName().
 *                      Add GetUserTrackName().
 *   12/03/18 1.15  SP 	Add Set/GetNote().
 *   11/12/17 1.14  SP 	Replace GetBestID() with GetID().
 *                      Add GetUnifiedID() and GetSecondaryID().
 *   23/11/17 1.13  SP 	Add initial support for colour control.
 *   17/11/17 1.12  AGC	Use as base class for track objects.
 *   13/11/17 1.11  SP 	Support alarm state.
 *   08/11/17 1.10  SP 	Remove unused global ID.
 *   18/10/17 1.9   SP 	Move alarm handling to SPxAlarmTrack.
 *                      Add GetPrevLatLong().
 *   07/09/17 1.8   AGC Add ageSecs arg to GetLatLong().
 *                      Add ageSecs arg to GetAltitude().
 *                      Add GetVerticalRate().
 *   22/08/17 1.7   SP 	Add GetSecondaryTrack().
 *                      Add GetDistanceFromPoint().
 *   03/08/17 1.6   SP 	Add GetUpdateTime().
 *   01/08/17 1.5   AGC	Add GetAltitude().
 *   31/07/17 1.4   SP 	Further development.
 *   24/07/17 1.3   SP 	Support fused tracks.
 *   18/07/17 1.2   SP 	Further development.
 *   13/07/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_UNI_TRACK_H
#define _SPX_UNI_TRACK_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxViewControl.h"

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

/* Forward declarations. */
struct SPxTime_tag;
class SPxAreaObj;
class SPxAsterixMsg;
class SPxUniTrackDatabase;

/*
 * Define our class.
 */
class SPxUniTrack : public SPxObj
{
    /* The database class can access our internals directly. */
    friend class SPxUniTrackDatabase;

public:
    /*
     * Public types.
     */

    /* Track type bitfields. */
    typedef UINT32 TrackType_t;
    static const UINT32 TRACK_TYPE_NONE  = 0x00000000;
    static const UINT32 TRACK_TYPE_RADAR = 0x00000001;
    static const UINT32 TRACK_TYPE_AIS   = 0x00000002;
    static const UINT32 TRACK_TYPE_ADSB  = 0x00000004;
    static const UINT32 TRACK_TYPE_FUSED = 0x00000008;
    static const UINT32 TRACK_TYPE_ANY =   0xFFFFFFFF;

    enum FieldType_t
    {
        /* Order by preferred ID when persistent ID requested. */
        FIELD_TYPE_PERSISTENT  = 0, /* Must be first entry. */
        FIELD_TYPE_ID          = 1,
        FIELD_TYPE_MMSI        = 2,
        FIELD_TYPE_AA          = 3,
        FIELD_TYPE_MODE3A      = 4,
        FIELD_TYPE_MODE2       = 5,
        FIELD_TYPE_MODE1       = 6,
        FIELD_TYPE_MAX /* Must be last entry. */
    };

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* General. */
    virtual SPxUniTrackDatabase *GetUniTrackDatabase(void) const { return m_database; }
    virtual int GetSourceIndex(void) const { return m_sourceIndex; }
    virtual TrackType_t GetTrackType(void) const { return m_trackType; }
    virtual const char *GetTrackTypeName(void) const;
    virtual UINT32 GetUnifiedID(void) const { return m_unifiedID; }
    virtual SPxErrorCode SetIsAlarm(int isEnabled);
    virtual int GetIsAlarm(void) const { return m_isAlarm; }
    virtual SPxErrorCode SetIsClearedAlarm(int isEnabled);
    virtual int GetIsClearedAlarm(void) const { return m_isClearedAlarm; }
    virtual SPxErrorCode SetClassCode(unsigned int code);
    virtual unsigned int GetClassCode(void) const { return m_classCode; }
    virtual UINT32 GetClassColour(void) const;
    virtual SPxErrorCode GetUpdateTime(SPxTime_t *time) const;
    virtual SPxErrorCode GetDistanceFromPoint(double latDegs, 
                                              double longDegs, 
                                              double *distMetresRtn,
                                              int mutexAlreadyLocked=FALSE,
                                              double labelMetresPerPixel=0.0) const;
    virtual SPxErrorCode SetNote(const char *text);
    virtual SPxErrorCode GetNote(char *buf, unsigned int len) const;
    virtual int IsAllowedByFilters(void) const { return m_isAllowedByFilters; }
    virtual int CheckTrackMatches(const char *matchString);
    virtual SPxErrorCode SetThreatScore(double score);
    virtual SPxErrorCode GetThreatScore(double *score, unsigned int expirySecs=0) const;

    /* Common interface to track data. */
    virtual SPxErrorCode SetUserTrackName(const char *name);
    virtual const char *GetUserTrackName(void) const;
    virtual const char *GetTrackName(void) const;
    virtual TrackType_t GetSecondaryTrackType(void) const;
    virtual const char *GetSecondaryTrackTypeName(void) const;
    virtual SPxErrorCode GetDescriptor(char *buf, unsigned int bufLen,
                                       FieldType_t fieldType = FIELD_TYPE_PERSISTENT) const;
    virtual int TestDescriptor(const char *desc) const;
    virtual int TestDescriptor(int isFused, TrackType_t srcType, int srcIndex,
                               FieldType_t fieldType, UINT32 id) const;
    virtual int GetIsDeleted(void) const { return m_isDeleted; }
    virtual int GetIsPreserved(void) const { return m_isPreserved; }
    virtual SPxErrorCode SetIsSelected(int select);
    virtual int GetIsSelected(void) const { return m_isSelected; }
    virtual SPxErrorCode SetIsActivated(int activate);
    virtual int GetIsActivated(void) const { return m_isActivated; }
    virtual SPxErrorCode SetIsHighlighted(int state, unsigned int durationSecs = 0);
    virtual int GetIsHighlighted(void);
    virtual const SPxAsterixMsg *GetAsterixMsg(void) const { return(NULL); }
    virtual int GetOnGround(void) const		{ return(FALSE); }
    virtual int GetIsTest(void) const		{ return(FALSE); }
    virtual int GetIsSimulated(void) const	{ return(FALSE); }
    virtual int GetIsEmergency(void) const	{ return(FALSE); }
    virtual int GetIsSPI(void) const		{ return(FALSE); }

    virtual UINT32 GetID(void) const = 0;
    virtual UINT32 GetSecondaryID(void) const = 0;
    virtual UINT32 GetSecondarySensorType(void) const = 0;
    virtual void GetDefaultTrackName(char *buf, unsigned int bufLen) const = 0;
    virtual SPxErrorCode GetField(UINT32 *fieldRtn, FieldType_t fieldType,
                                  TrackType_t trackType=TRACK_TYPE_ANY) const;
    virtual SPxErrorCode GetMMSI(UINT32 * /*mmsi*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetAA(UINT32 * /*aa*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetIFFMode1(UINT16 * /*iffMode1Rtn*/, int * /*is12BitRtn*/=NULL) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetIFFMode2(UINT16 * /*iffMode2Rtn*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetIFFMode3A(UINT16 * /*iffMode3ARtn*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetLatLong(double *latDegsRtn, 
                                    double *longDegsRtn,
                                    int useFiltered=TRUE,
                                    double *ageSecsRtn=NULL) const = 0;
    virtual SPxErrorCode GetAltitude(double *altMetresRtn,
                                     double *ageSecsRtn=NULL,
                                     int *isAltFromFLRtn=NULL) const = 0;
    virtual SPxErrorCode GetVelocity(double * /*speedMpsRtn*/,
                                     double * /*courseDegsRtn*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetHeading(double * /*headingDegsRtn*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetVerticalRate(double * /*verticalRateMpsRtn*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetADSBCategory(unsigned int * /*categoryRtn*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual const SPxTime_t *GetTimestamp(void) const { return NULL; }
    virtual const SPxTime_t *GetFirstTimestamp(void) const { return NULL;}
    virtual SPxErrorCode SetUseColour(int /*useColour*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int GetUseColour(void) { return FALSE; }
    virtual SPxErrorCode SetColour(UINT32 /*colour*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual UINT32 GetColour(void) { return 0; }
    virtual int IsPlot(void) const { return FALSE; }

    virtual SPxErrorCode SetIsBold(int /*isBold*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int GetIsBold(void) { return FALSE; }

    /* Estimated acceleration and rate of turn. */
    virtual double GetEstAcceleration(void) const;
    virtual double GetEstRateOfTurn(void) const;

    /* Areas. */
    virtual int IsInArea(SPxAreaObj *area, double secs=0.0) const;
    virtual int WasInArea(SPxAreaObj *area, double secs=0.0) const;
    virtual int LingeredInArea(SPxAreaObj *area, double secs) const;
    virtual int StartedInArea(SPxAreaObj *area) const;

    /* Gates. */
    virtual int IsGateCrossed(SPxAreaObj *gate, int isFwd, double secs) const;
    virtual int IsGateCrossed(SPxAreaObj *gate, double secs) const;

    /* Land proximity (ONLY for use by SPxCondition). */
    virtual void CheckCoastApproach(void);
    virtual int IsNearLand(double rangeLimitMetres) const;
    virtual int IsApproachingLand(double rangeLimitMetres,
                                  double timeLimitSecs) const;
    virtual double GetLandDistance(void) const;
    virtual double GetLandApproachSpeed(void) const;

    /* User data. */
    virtual void SetUserData(void *ptr, const void *key=NULL);
    virtual void *GetUserData(const void *key=NULL) const;

    /* Misc. */
    virtual SPxErrorCode FormatLabel(char *buf, int bufLen,
                                     const char *formatString,
                                     SPxFormatLatLong_t latLongFormat = SPX_FORMAT_LL_DEGS,
                                     SPxUnitsDist_t unitsDist = SPX_UNITS_DIST_METRES,
                                     SPxUnitsHeight_t unitsHeight = SPX_UNITS_HGT_AUTO,
                                     SPxUnitsSpeed_t unitsSpeed = SPX_UNITS_SPEED_MPS,
                                     SPxHeightMode_t heightMode = SPX_HEIGHT_MODE_DEFAULT,
                                     double refLatDegs = 0.0,
                                     double refLongDegs = 0.0,
                                     unsigned int maxRows = 10,
                                     int enableAutoUnits = FALSE,
                                     int isInterceptor = FALSE,
                                     int useLegacyFormat = FALSE) const;

    /*
     * Public static functions.
     */

    static SPxErrorCode DecodeDescriptor(const char *desc,
                                         int *isFusedRtn,
                                         TrackType_t *srcTypeRtn,
                                         int *srcIndexRtn,
                                         FieldType_t *fieldTypeRtn,
                                         UINT32 *fieldRtn,
                                         int silent=FALSE);

    static SPxErrorCode ValidateDescriptor(const char *desc);

    static SPxErrorCode BuildDescriptor(char *buf,
                                        unsigned int bufLen,
                                        int isFused,
                                        TrackType_t srcType,
                                        int srcIndex,
                                        FieldType_t fieldType,
                                        UINT32 field,
                                        int persistent);

    static int IsPersistentDescriptor(const char *desc);

    static int IsEmptyDescriptor(const char *desc);

    static const char *GetSecondaryTrackTypeNameFromType(TrackType_t type);

    static const char *GetNameForThreatLevel(UINT8 threatLevel);

    static SPxErrorCode SetNameForThreatLevel(UINT8 threatLevel, const char *name);

    static int IsThreatLevelNameSet(UINT8 threatLevel);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Constructor and destructor. */
    explicit SPxUniTrack(TrackType_t trackType);
    virtual ~SPxUniTrack(void);

    /* Set link to unified database. */
    virtual void SetUniTrackDatabase(SPxUniTrackDatabase *database) {m_database = database; }
    virtual void SetSourceIndex(int id) {m_sourceIndex = id; }

    /* Reset the object. */
    virtual SPxErrorCode Reset(void);

    /* Handle area delete. */
    virtual void handleShapeDelete(SPxAreaObj *area);
    virtual void handleShapeDeleteAll(void);
    virtual void handleShapeReloadAll(void);

    /* Set filter state. */
    void setIsAllowedByFilters(int isAllowed) { m_isAllowedByFilters = isAllowed; }

    /* Used by track databases. */
    virtual void setIsDeleted(int isDeleted) { m_isDeleted = isDeleted; }
    virtual void setIsPreserved(int isPreserved) { m_isPreserved = isPreserved; }

private:

    /*
     * Private types.
     */

    /* General area info. */
    struct AreaInfo;

    /* Visited area info. */
    struct VisitedAreaInfo;

    /* Crossed gate info. */
    struct GateInfo;

    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    mutable SPxCriticalSection m_mutex;     /* Object mutex. */
    SPxUniTrackDatabase *m_database;        /* Unified track database. */
    int m_sourceIndex;                      /* Source index in unified track database. */
    TrackType_t m_trackType;                /* Track type. */
    UINT32 m_unifiedID;                     /* Unique ID assigned to each track in SPxUniTrackDatabase. */
    unsigned int m_classCode;               /* Class code. */
    SPxTime_t m_updateTime;                 /* Time of last update. */
    double m_secsSinceUpdate;               /* Seconds between updates. */
    SPxLatLong_t m_filteredLL;              /* Previous filtered lat/long position. */
    int m_isFilteredLLSet;                  /* Is m_prevFilteredLL set? */
    SPxLatLong_t m_rawLL;                   /* Previous raw lat/long position. */
    int m_isRawLLSet;                       /* Is m_prevRawLL set? */
    SPxLatLong_t m_prevFilteredLL;          /* Previous filtered lat/long position. */
    int m_isPrevFilteredLLSet;              /* Is m_prevFilteredLL set? */
    SPxLatLong_t m_prevRawLL;               /* Previous raw lat/long position. */
    int m_isPrevRawLLSet;                   /* Is m_prevRawLL set? */
    int m_isAlarm;                          /* Is track in alarm state? */
    int m_isClearedAlarm;                   /* Is track triggering any cleared alarms? */
    int m_isDeleted;                        /* Is track being deleted? */
    int m_isPreserved;                      /* Is track preserved? */
    int m_isSelected;                       /* Is track selected? */
    int m_isActivated;                      /* Is track activated? */
    int m_isAllowedByFilters;               /* Is allowed by track filters? */
    double m_threatScore;                   /* Calculated threat score. */
    SPxTime_t m_threatScoreUpdateTime;      /* Time of last threat score update. */
    SPxTime_t m_lastLandUpdateTime;         /* Time of last land update. */
    double m_lastLandDistMetres;            /* Last distance from land. */
    double m_lastLandApproachSpeedMps;      /* Last approach speed to land. */
    double m_lastSpeedMps;                  /* Speed from last update. */
    double m_lastCourseDegs;                /* Course from last update. */
    double m_estAccelMps2;                  /* Estimated acceleration in m/s/s. */
    double m_estRateOfTurnDps;              /* Estimated rate of turn in degs/s. */

    /* Is track supposed to be highlighted. */
    int m_isHighlighted;                    /* Current state, TRUE/FALSE */
    unsigned int m_highlightDurationSecs;   /* How long for, 0 forever */
    UINT32 m_highlightStartMsecs;           /* Timestamp */

    /*
     * Private functions.
     */

    /* General. */
    void reportUpdate(void);

    /* Areas. */
    int isInArea(SPxAreaObj *area, double secs) const;
    int wasInArea(SPxAreaObj *area, double secs) const;
    int startedInArea(SPxAreaObj *area) const;
    SPxErrorCode addStartArea(SPxAreaObj *area);
    SPxErrorCode removeStartArea(SPxAreaObj *area);
    SPxErrorCode addCurrentArea(SPxAreaObj *area);
    SPxErrorCode removeCurrentArea(SPxAreaObj *area);
    SPxErrorCode addVisitedArea(SPxAreaObj *area);
    SPxErrorCode removeVisitedArea(SPxAreaObj *area);
    void checkAreas(void);
    void checkArea(SPxAreaObj *area);

    /* Gates. */
    int isGateCrossed(SPxAreaObj *gate, int isFwd, double secs) const;
    SPxErrorCode addCrossedGate(SPxAreaObj *gate, int isCrossedFwd);
    SPxErrorCode removeCrossedGate(SPxAreaObj *gate);
    void checkGates(void);
    void checkGate(SPxAreaObj *gate);
    SPxErrorCode getPrevLatLong(double *latDegsRtn,
                                double *longDegsRtn,
                                int useFiltered = TRUE) const;

    /*
     * Private static functions.
     */

    static SPxErrorCode areaIterator(SPxAreaObj *area, 
                                     void *userArg);

    static SPxErrorCode gateIterator(SPxAreaObj *area, 
                                     void *userArg);

}; /* SPxUniTrack */

#endif /* SPX_UNI_TRACK_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
