/*********************************************************************
 *
 * (c) Copyright 2017 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxUniTrackDatabase class which unifies support for
 *   multiple radar, fused, AIS and ADS-B track sources.
 *
 * Revision Control: 
 *   09/09/22 v1.61  AGC	Support retrieving track sources by index.
 *
 * Previous Changes:
 *   27/05/22 1.60  REW	Remove DeleteAllTracks() (not thread safe).
 *   26/05/22 1.59  REW	Add DeleteAllTracks().
 *   06/04/22 1.58  AGC	Remove vector header.
 *   31/01/22 1.57  AGC	Add mutexAlreadyLocked option to GetNumSources().
 *   20/01/22 1.56  SP 	Ensure source indices are unique after source removal.
 *   20/01/22 1.55  SP 	Add optional sourceIndex to GetTrack().
 *			Add GetAssociatedTrack() and GetPrimaryTrack();
 *   11/01/22 1.54  SP 	Remove handleTrackCreate().
 *   14/09/21 1.53  BTB	Support remote control of track filters.
 *   09/09/21 1.52  SP 	Add SetFilterDefaultIsExclusion().
 *			Remove isExclusion arg from CreateFilter().
 *   10/05/21 1.51  AJH	Support track retrieval as GeoJSON.
 *   27/04/21 1.50  SP 	Support load & save of embedded track groups.
 *   20/04/21 1.49  SP 	Support remote control of track groups.
 *   04/03/21 1.48  JGB	Remove function GetTracks().
 *   02/03/21 1.47  JGB	Add function GetTracks()
 *   12/02/21 1.46  SP 	Add EVENT_TRACK_PRESELECT event.
 *   10/02/21 1.45  AGC	Support queries relating to track history.
 *   04/01/21 1.44  AJH	Support query for last track update.
 *   23/10/20 1.43  SP 	Add IterateTracks() for consistency with other track databases.
 *   19/10/20 1.42  AGC	Fix warnings by more conservative use of virtual.
 *   15/10/20 1.41  AGC	Add source removal functions to fix shutdown.
 *   09/10/20 1.40  SP 	Add GetTrackGroupShortPath() and SaveAllTrackGroups().
 *   08/10/20 1.39  SP 	Add CanDeleteFilters() and CanDeleteSharedFilters().
 *   28/09/20 1.38  AGC	Add IsTrackGroupUsedInFilter().
 *   25/09/20 1.37  AGC	Add IsTrackGroupUnsaved().
 *			Add GetTrackGroupPath().
 *   24/09/20 1.36  AGC	Support height units config.
 *   18/09/20 1.35  AGC	Support runtime track group config.
 *                     	Support retrieving filtered number of tracks.
 *   27/08/20 1.34  AGC	Support including label offset in display pos.
 *   25/10/19 1.33  AGC	Move parameter defines to source file.
 *   03/05/19 1.32  SP 	Support enhancements to track descriptor.
 *   14/11/18 1.31  BTB	Handle load, save and deletion of shared track filters.
 *   13/11/18 1.30  BTB	Handle reload and deletion of all shapes.
 *   27/09/18 1.29  REW	Add numPlotsRtn param to GetNumTracks().
 *   21/09/18 1.28  SP 	Support background timer.
 *   19/09/18 1.27  SP 	Add support for reference points.
 *   18/09/18 1.26  SP 	Support user-defined track groups.
 *   17/09/18 1.25  SP 	Add GetTrackFromDescriptor().
 *   04/06/18 1.24  SP 	Support track class name.
 *   31/05/18 1.23  REW	Fix mutex problem with my last change.
 *   30/05/18 1.22  SP 	Support track class colour.
 *   30/05/18 1.21  REW	Add UpdateFilters().
 *   30/05/18 1.20  REW	Support exclusion filters.
 *   17/05/18 1.19  SP 	Add SetFiltersState().
 *   09/05/18 1.18  SP 	Remove BOOL.
 *   09/05/18 1.17  SP 	Make filtering optional for track access functions.
 *   04/05/18 1.16  SP 	Support loading and saving filters.
 *   03/05/18 1.15  SP 	Add support for track filtering.
 *   01/05/18 1.14  SP 	Add support for an area database.
 *   01/05/18 1.13  SP 	Remove current track filtering options.
 *   29/03/18 1.12  SP 	Further changes to support source index.
 *   28/03/18 1.11  SP 	Support source index.
 *   27/03/18 1.10  SP 	Add GetRadarPosition().
 *   11/12/17 1.9   SP 	Support removal of GetBestID().
 *                      Support unique track ID.
 *   17/11/17 1.8   AGC	Support changes to track object inheritance.
 *   18/10/17 1.7   SP 	Move alarm handling to SPxUniTrackDatabase.
 *   18/10/17 1.6   AGC	Remove superfluous forward declaration.
 *   22/08/17 1.5   SP 	Remove unused functions.
 *                      Add GetTrack() and GetTracksInRange().
 *                      Add support for reference points.
 *   31/07/17 1.4   SP 	Further development.
 *   24/07/17 1.3   SP 	Support fused tracks.
 *   18/07/17 1.2   SP 	Further development.
 *   13/07/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_UNI_TRACK_DATABASE_H
#define _SPX_UNI_TRACK_DATABASE_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibNet/SPxUniTrack.h"
#include "SPxLibNet/SPxUniTrackFilter.h"
#include "SPxLibNet/SPxUniTrackCondition.h"

/* Forward declare any classes required. */
class SPxTrackDatabase;
class SPxAISDecoder;
class SPxADSBDecoder;
class SPxNavData;
class SPxRadarTrack;
class SPxAreaObj;
class SPxRefPoint;

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

/*
 * Define our class.
 */
class SPxUniTrackDatabase : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Selection state. */
    typedef enum
    {
        STATE_SELECT,       /* Select a track. */
        STATE_DESELECT,     /* Deselect a track. */
        STATE_TOGGLE,       /* Toggle track selection. */
        STATE_SELECT_L1,    /* Level 1 user-defined selection. */
        STATE_SELECT_L2,    /* Level 2 user-defined selection. */
        STATE_SELECT_L3     /* Level 3 user-defined selection. */
    
    } State_t;

    /* Notification event type. */
    typedef enum
    {
        EVENT_TRACK_CREATE,             /* Track has been created. */
        EVENT_TRACK_UPDATE,             /* Track has been updated. */
        EVENT_TRACK_PRESELECT,          /* Track about to be selected or deselected. */
        EVENT_TRACK_SELECT,             /* Track has been selected or deselected. */
        EVENT_TRACK_DELETE,             /* Track has been deleted. */
        EVENT_FILTER_CREATE,            /* A track filter has been created. */
        EVENT_FILTER_CHANGE,            /* A track filter has changed. */
        EVENT_FILTER_DELETE,            /* A track filter  is about to be been deleted. */
        EVENT_FILTER_CONDITION_CREATE,  /* A track filter condition has been created. */
        EVENT_FILTER_CONDITION_CHANGE,  /* A track filter condition has changed. */
        EVENT_FILTER_CONDITION_DELETE,  /* A track filter condition is about to be been deleted. */
        EVENT_TRACK_GROUP_CREATE,       /* A track group has been created. */
        EVENT_TRACK_GROUP_CHANGE,       /* A track group has been changed. */
        EVENT_TRACK_GROUP_DELETE        /* A track group has been deleted. */

    } EventType_t;

    /* Track event info. Used by:
     *  EVENT_TRACK_CREATE
     *  EVENT_TRACK_UPDATE
     *  EVENT_TRACK_SELECT
     *  EVENT_TRACK_DELETE
     */
    typedef struct
    {
        SPxUniTrack *track;  /* Track handle or NULL. */
        State_t state;       /* Selection status. */

    } EventTrackInfo_t;

    /* Track filter event info. Used by:
     *  EVENT_FILTER_CREATE
     *  EVENT_FILTER_CHANGE
     *  EVENT_FILTER_DELETE
     */
    typedef struct
    {
        SPxUniTrackFilter *filter;              /* Filter handle. */
        SPxUniTrackFilter::ChangeFlags_t flags; /* Change flags. */

    } EventFilterInfo_t;

    /* Condition event info. Used by:
     *  EVENT_FILTER_CONDITION_CREATE
     *  EVENT_FILTER_CONDITION_CHANGE
     *  EVENT_FILTER_CONDITION_DELETE
     */
    typedef struct
    {
        SPxUniTrackCondition *condition;               /* Condition handle. */
        SPxUniTrackCondition::ChangeFlags_t flags;     /* Change flags. */

    } EventFilterConditionInfo_t;

    /* Track group event info. Used by:
     *  EVENT_TRACK_GROUP_CREATE
     *  EVENT_TRACK_GROUP_CHANGE
     *  EVENT_TRACK_GROUP_DELETE
     */
    typedef struct
    {
        UINT32 trackGroupID;               /* Track group ID. */

    } EventTrackGroupInfo_t;

    /* Union of all event info structures. */
    typedef union
    {
        EventTrackInfo_t trackEvt;
        EventFilterInfo_t filterEvt;
        EventFilterConditionInfo_t filterConditionEvt;
        EventTrackGroupInfo_t trackGroupEvt;
    
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

    /* Used by GetTracksInRange(). */
    typedef struct
    {
        SPxUniTrack *track;     /* Track. */
        double distMetres;      /* Distance from point. */

    } TrackDist_t;

    /* Track source definition. */
    typedef struct
    {
        SPxUniTrack::TrackType_t type;  /* Track type. */
        int index;                      /* Source index. */

    } Source_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxUniTrackDatabase(SPxAreaObj *areaDB=NULL,
                                 SPxWorldMap *worldMap=NULL);
    virtual ~SPxUniTrackDatabase(void);

    /* Access. */
    SPxAreaObj *GetAreaDB(void) { return m_areaDB; }
    SPxWorldMap *GetWorldMap(void) { return m_worldMap; }

    /* Add track sources. */
    SPxErrorCode AddRadarTrackSource(SPxTrackDatabase *database,
                                     SPxNavData *navData=NULL,
                                     int *sourceIndexRtn=NULL);
    SPxErrorCode RemoveRadarTrackSource(SPxTrackDatabase *database);
    SPxTrackDatabase *GetRadarTrackSource(unsigned int index);
    int GetRadarTrackSourceIndex(SPxTrackDatabase *database) const;

    SPxErrorCode AddFusedTrackSource(SPxTrackDatabase *database,
                                     int *sourceIndexRtn=NULL);
    SPxErrorCode RemoveFusedTrackSource(SPxTrackDatabase *database);
    SPxTrackDatabase *GetFusedTrackSource(unsigned int index);
    int GetFusedTrackSourceIndex(SPxTrackDatabase *database) const;

    SPxErrorCode AddAISTrackSource(SPxAISDecoder *decoder,
                                   int *sourceIndexRtn=NULL);
    SPxErrorCode RemoveAISTrackSource(SPxAISDecoder *decoder);
    SPxAISDecoder *GetAISTrackSource(unsigned int index);
    int GetAISTrackSourceIndex(SPxAISDecoder *decoder) const;

    SPxErrorCode AddADSBTrackSource(SPxADSBDecoder *decoder,
                                    int *sourceIndexRtn=NULL);
    SPxErrorCode RemoveADSBTrackSource(SPxADSBDecoder *decoder);
    SPxADSBDecoder *GetADSBTrackSource(unsigned int index);
    int GetADSBTrackSourceIndex(SPxADSBDecoder *decoder) const;

    unsigned int GetNumSources(unsigned int *numRadarRtn=NULL, 
                               unsigned int *numAISRtn=NULL, 
                               unsigned int *numADSBRtn=NULL, 
                               unsigned int *numFusedRtn=NULL,
                               int mutexAlreadyLocked=FALSE) const;

    /* Radar position. */
    virtual SPxErrorCode GetRadarLatLong(SPxTrackDatabase *database,
                                         double *latDegsRtn,
                                         double *longDegsRtn,
                                         int mutexAlreadyLocked=FALSE);

    /* Track access. */
    virtual unsigned int GetNumTracks(unsigned int *numRadarRtn=NULL, 
                                      unsigned int *numAISRtn=NULL, 
                                      unsigned int *numADSBRtn=NULL, 
                                      unsigned int *numFusedRtn=NULL,
                                      unsigned int *numPlotsRtn=NULL,
                                      int applyFilters=FALSE) const;

    virtual void SetSelectedTrack(SPxUniTrack *track, State_t state);
    virtual SPxUniTrack *GetSelectedTrack(void) const { return m_selectedTrack; }

    virtual SPxUniTrack *GetTrack(UINT32 uniqueID,
                                  int mutexAlreadyLocked=FALSE) const;

    virtual SPxUniTrack *GetTrack(UINT32 id, 
                                  SPxUniTrack::TrackType_t type,
                                  int mutexAlreadyLocked=FALSE,
                                  int sourceIndex=-1) const;

    virtual SPxUniTrack *GetTrackFromDescriptor(const char *desc) const;

    virtual SPxUniTrack *GetNearestTrack(double latDegs,
                                         double longDegs,
                                         int applyFilters,
                                         double *distMetresRtn=NULL) const;

     virtual SPxUniTrack *GetAssociatedTrack(SPxUniTrack *track,
                                             const Source_t sourceMap[],
                                             unsigned int sourceMapLen,
                                             int mutexAlreadyLocked=FALSE) const;

    virtual SPxUniTrack *GetPrimaryTrack(SPxUniTrack *track,
                                         int mutexAlreadyLocked=FALSE) const;

    virtual SPxUniTrack *GetSecondaryTrack(SPxUniTrack *track,
                                           int mutexAlreadyLocked=FALSE) const;

    virtual unsigned int GetTracksInRange(double latDegs,
                                          double longDegs,
                                          double maxDistMetres,
                                          TrackDist_t *tracksRtn[],
                                          double labelMetresPerPixel=0.0) const;

    virtual SPxErrorCode IterateTracksForward(SPxCallbackListFn_t fn, 
                                              void *userArg,
                                              int mutexAlreadyLocked=FALSE);
    virtual SPxErrorCode IterateTracksReverse(SPxCallbackListFn_t fn, 
                                              void *userArg,
                                              int mutexAlreadyLocked=FALSE);
    virtual SPxErrorCode IterateTracks(SPxCallbackListFn_t fn, 
                                       void *userArg)
    {
        return IterateTracksForward(fn, userArg);
    }

    /* JSON retrieval. */
    virtual int GetJSON(char *buf, int buflen, SPxLatLong_t llSW, SPxLatLong_t llNE);

    /* Units. */
    virtual SPxErrorCode SetDistUnits(SPxUnitsDist_t units, unsigned int decimalPlaces);
    virtual SPxUnitsDist_t GetDistUnits(void) const { return m_distUnits; }
    virtual unsigned int GetDistDecimalPlaces(void) const { return m_distDecimalPlaces; }
    virtual SPxErrorCode SetHeightUnits(SPxUnitsHeight_t units, unsigned int decimalPlaces);
    virtual SPxUnitsHeight_t GetHeightUnits(void) const { return m_heightUnits; }
    virtual unsigned int GetHeightDecimalPlaces(void) const;
    virtual SPxErrorCode SetSpeedUnits(SPxUnitsSpeed_t units, unsigned int decimalPlaces);
    virtual SPxUnitsSpeed_t GetSpeedUnits(void) const { return m_speedUnits; }
    virtual unsigned int GetSpeedDecimalPlaces(void) const { return m_speedDecimalPlaces; }

    /* Track filters. */
    virtual SPxErrorCode SetFilterDefaultIsExclusion(int isExclude);
    virtual int GetFilterDefaultIsExclusion(void) { return m_filterDefaultIsExclusion; }
    virtual SPxUniTrackFilter *CreateFilter(SPxErrorCode *errRtn=NULL);
    virtual unsigned int GetNumFilters(SPxUniTrackFilter::State_t states=SPxUniTrackFilter::STATE_ANY);
    virtual SPxErrorCode IterateFilters(SPxCallbackListFn_t fn, 
                                        void *userArg,
                                        int mutexAlreadyLocked=FALSE);
    virtual SPxErrorCode DeleteFilter(SPxUniTrackFilter *filter);
    virtual int CanDeleteFilters(SPxUniTrackFilter::State_t states=SPxUniTrackFilter::STATE_ANY);
    virtual SPxErrorCode DeleteFilters(SPxUniTrackFilter::State_t states=SPxUniTrackFilter::STATE_ANY);
    virtual int CanDeleteSharedFilters(SPxUniTrackFilter::State_t states=SPxUniTrackFilter::STATE_ANY);
    virtual SPxErrorCode DeleteSharedFilters(SPxUniTrackFilter::State_t states=SPxUniTrackFilter::STATE_ANY);
    virtual SPxErrorCode SetFiltersState(SPxUniTrackFilter::State_t state);
    virtual SPxErrorCode LoadFilters(const char *filename, int sharedFilters=FALSE);
    virtual SPxErrorCode SaveFilters(const char *filename, int sharedFilters=FALSE);
    virtual SPxErrorCode UpdateFilters(void);

    /* Track groups. */
    unsigned int GetNumTrackGroups(void) const;
    unsigned int GetMaxNumTrackGroups(void) const;
    unsigned int GetTrackGroupID(UINT32 index) const;
    virtual int IsTrackGroupInUse(UINT32 id) const;
    int IsTrackGroupUnsaved(UINT32 id) const;
    SPxErrorCode AddTrackGroup(UINT32 id, UINT32 *usedID=NULL);
    SPxErrorCode DeleteTrackGroup(UINT32 id);
    SPxErrorCode SetTrackGroupName(UINT32 id, const char *name);
    virtual SPxErrorCode GetTrackGroupName(UINT32 id,
                                           char *buf,
                                           unsigned int bufLen) const;
    SPxErrorCode LoadTrackGroup(UINT32 id, const char *filename);
    SPxErrorCode LoadEmbeddedTrackGroup(UINT32 id, FILE *fPtr);
    SPxErrorCode SaveTrackGroup(UINT32 id, const char *filename);
    SPxErrorCode SaveEmbeddedTrackGroup(UINT32 id, FILE *fPtr);
    SPxErrorCode SaveAllTrackGroups(void);
    virtual SPxErrorCode GetTrackGroupFilename(UINT32 id,
                                               char *buf,
                                               unsigned int bufLen) const;
    virtual SPxErrorCode GetTrackGroupPath(UINT32 id,
                                           char *buf,
                                           unsigned int bufLen) const;
    virtual SPxErrorCode GetTrackGroupShortPath(UINT32 id,
                                                char *buf,
                                                unsigned int bufLen) const;
    unsigned int GetNumTrackGroupDescriptors(UINT32 id, SPxErrorCode *errRtn=NULL) const;
    SPxErrorCode AddTrackGroupDescriptor(UINT32 id);
    SPxErrorCode DeleteTrackGroupDescriptor(UINT32 id, UINT32 descIndex);
    SPxErrorCode SetTrackGroupDescriptor(UINT32 id,
                                         UINT32 descIndex,
                                         const char *descriptor);
    SPxErrorCode GetTrackGroupDescriptor(UINT32 id,
                                         UINT32 descIndex,
                                         char *buf,
                                         unsigned int bufLen) const;
    int IsTrackGroupUsedInFilter(UINT32 id) const;
    virtual int IsTrackInGroup(UINT32 id, SPxUniTrack *track) const;

    /* Track history. */
    void SetMaxTrackHistorySecs(unsigned int secs);
    unsigned int GetMaxTrackHistorySecs(void) const;
    int IsTrackHistoryClose(double latDegs,
                            double longDegs,
                            unsigned int historySecs,
                            double maxDistMetres,
                            double *distMetresRtn=NULL,
                            int includeRadarTracks=TRUE,
                            int includeAISTracks=TRUE,
                            int includeADSBTracks=TRUE,
                            int includeFusedTracks=TRUE);

    /* Reference points. */
    virtual SPxErrorCode AddRefPoint(SPxRefPoint *refPoint);
    virtual SPxRefPoint *GetRefPoint(UINT32 id) const;
    virtual SPxErrorCode IterateRefPoints(SPxCallbackListFn_t fn, 
                                          void *userArg);
    
    /* Track classification. */
    virtual SPxErrorCode SetTrackClassName(unsigned int code, const char *name);
    virtual const char *GetTrackClassName(unsigned int code) const;
    virtual SPxErrorCode SetTrackClassColour(unsigned int code, UINT32 argb);
    virtual UINT32 GetTrackClassColour(unsigned int code) const;

    /* Status. */
    virtual UINT32 GetTimeSinceLastUpdate(void) const;

    /* Event callbacks. */
    virtual SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, 
                                          void *userObj);

    virtual SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, 
                                             void *userObj);

    virtual SPxErrorCode ReportEvent(Event_t *evt);

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

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxTimer m_timer;                   /* Background timer. */
    SPxAreaObj *m_areaDB;               /* Area database or NULL. */
    SPxWorldMap *m_worldMap;            /* World map or NULL. */
    SPxCallbackList *m_eventCBList;     /* Event callback list. */
    SPxUniTrack *m_selectedTrack;       /* Selected track. */
    SPxUnitsDist_t m_distUnits;         /* Distance units. */
    unsigned int m_distDecimalPlaces;   /* Distance decimal places. */
    SPxUnitsHeight_t m_heightUnits;     /* Height units. */
    unsigned int m_heightDecimalPlaces; /* Height decimal places. */
    SPxUnitsSpeed_t m_speedUnits;       /* Speed units. */
    unsigned int m_speedDecimalPlaces;  /* Speed decimal places. */
    int m_isTrackFilterChangePending;   /* Change made to track filters? */
    int m_filterDefaultIsExclusion;     /* New filters default to exclusion? */
    UINT32 m_lastUpdateTime;            /* Ticker time for last track report. */
    int m_nextRadarSourceIndex;         /* Next radar source index to allocate. */
    int m_nextFusedSourceIndex;         /* Next fused source index to allocate. */
    int m_nextAISSourceIndex;           /* Next AIS source index to allocate. */
    int m_nextADSBSourceIndex;          /* Next ADS-B source index to allocate. */

    /*
     * Private functions.
     */

    /* General. */
    void setTrackActive(SPxUniTrack *track, int isActive);
    void handleTrackUpdate(int isCreate, SPxUniTrack *track, int sourceIndex);
    void handleTrackDelete(SPxUniTrack *track);
    State_t getTrackSelectionState(SPxUniTrack *track);
    void handleShapeDelete(SPxAreaObj *shape);
    void handleShapeDeleteAll(void);
    void handleShapeReloadAll(void);
    void saveTrackHistory(const SPxUniTrack *track);
    void cullTrackHistory(void);

    /* Filtering. */
    SPxUniTrackFilter *getNewFilter(void);
    SPxErrorCode setFilterInUse(SPxUniTrackFilter *filter, int isInUse);
    SPxUniTrackFilter *getFilterFromID(UINT32 id);
    SPxUniTrackFilter *getFilterFromIndex(unsigned int index);
    SPxErrorCode deleteFilter(SPxUniTrackFilter *filter);
    int isTrackAllowedByFilters(SPxUniTrack *track,
                                int mutexAlreadyLocked=FALSE) const;
    int isTrackFilteringEnabled(void) const;

    /* Filter conditions. */
    SPxUniTrackCondition *getFilterConditionFromID(UINT32 id, SPxUniTrackFilter **filterRtn);

    /*
     * Private static functions.
     */

    static void areaDatabaseEventHandler(SPxAreaObj *area, 
                                         UINT32 flags, 
                                         void *userArg);

    static int radarFusedTrackCreationCallback(void *invokingObject,
                                               void *userObject,
                                               void *arg);

    static int radarFusedTrackUpdateCallback(void *invokingObject,
                                             void *userObject,
                                             void *arg);

    static int aisTrackCallback(void *invokingObject,
                                void *userObject,
                                void *arg);

    static int adsbTrackCallback(void *invokingObject,
                                 void *userObject,
                                 void *arg);

    static int radarDBDeletionCallback(void *invokingObject,
                                       void *userObject,
                                       void *arg);
    static int fusedDBDeletionCallback(void *invokingObject,
                                       void *userObject,
                                       void *arg);
    static int aisDBDeletionCallback(void *invokingObject,
                                     void *userObject,
                                     void *arg);
    static int adsbDBDeletionCallback(void *invokingObject,
                                      void *userObject,
                                      void *arg);

    static int isTrackGroupUsedInCondition(void *invokingObject,
                                           void *userObject,
                                           void *arg);

    static bool compareTrackDists(const TrackDist_t t1, 
                                  const TrackDist_t t2);

    static void doBackgroundProcessing(void * userArg);

}; /* SPxUniTrackDatabase */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_UNI_TRACK_DATABASE_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
