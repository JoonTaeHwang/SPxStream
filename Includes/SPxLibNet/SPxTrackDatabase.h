/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxTrackDatabase class which supports the receipt
*   and handling of SPx track reports.
*
* Revision Control:
*   09/09/22 v1.54   AGC	Support plot input from SPxPacketDecoder.
*				Support sensor altitude retrieval.
*
* Previous Changes:
*   06/04/22 1.53   AGC	Support track input from SPxPacketDecoder.
*   14/03/22 1.52   REW	Add GetMsecsSinceLastInput().
*   22/02/22 1.51   SP 	Make report functions virtual.
*   21/04/21 1.50   AGC	Support link to radar replay object.
*   05/02/21 1.49   SP 	Support user-defined sensor position.
*   29/10/20 1.48   AGC	Support project code specific behaviour.
*   18/09/20 1.47   AGC	Support retrieving filtered number of tracks/plots.
*   26/08/20 1.46   AGC	Support compare function for track iteration.
*   17/07/20 1.45   AGC	Add DeleteAll().
*   22/01/20 1.44   REW	Support SetMaxNumTrailPoints().
*   14/01/19 1.43   AGC	Add deletion callback for remote server.
*   11/01/19 1.42   AGC	Support SPx plot status messages.
*   18/09/18 1.41   AGC	Allow include/skip of preserved tracks.
*   17/07/18 1.40   AGC	Support selecting preserved tracks.
*   06/07/18 1.39   AGC	Support storing deleted track trails.
*   02/07/18 1.38   AGC	Support minimum trail dot interval.
*   26/06/18 1.37   AGC	Use tail pointers for plot lists.
*   24/05/18 1.36   REW	Support searching by unified track ID.
*   20/04/18 1.35   REW	Support Asterix strobes.
*   13/04/18 1.34   REW	Add GetSensorPeriodSecs().
*   22/03/18 1.33   REW	Support plots.
*   14/03/18 1.32   AGC	Support optional track stats.
*   06/03/18 1.31   AGC	Store ASTERIX status message.
*   11/12/17 1.30   AGC	Pass ASTERIX message to radar track.
*   08/03/17 1.29   SP 	Add SetRemoteServer().
*   08/03/17 1.28   AGC	Improve mutex locking.
*   21/02/17 1.27   SP 	Add GetNextTrack().
*   23/08/16 1.26   AGC	Make GetNearest... functions const.
*   16/06/16 1.25   SP 	Support channel index for SQL database.
*   08/04/16 1.24   SP 	Add GetMaxTrackRangeMetres().
*   30/03/16 1.23   SP 	Support link with SQL database.
*   26/01/15 1.22   REW	Support link with SPxFlightPlan modules.
*   02/10/14 1.21   REW	Support label offsets.
*   02/10/14 1.20   REW	Add GetSensorLatLong().
*   24/09/14 1.19   REW	Add SetCalculateLatLongs().
*   12/09/14 1.18   REW	Add mutexAlreadyLocked to IterateTracks().
*   13/05/14 1.17   REW	Support link with NavData object.
*   17/02/14 1.16   SP 	Add GetLastTrackTimeMsecs() and
*			GetLastStatusTimeMsecs().
*   07/01/14 1.15   REW	Add GetTrack_InternalLocked().
*   07/10/13 1.14   SP 	Pass track data to CreateTrack().
*   05/09/13 1.13   AGC	Allow specifying desired track ID in AddTrack().
*   19/04/13 1.12   REW	Support SPxAsterixDecoder input.
*   23/11/12 1.11   AGC	Add ReportTrack/ReportTrackerStatus() functions.
*   26/10/12 1.10   AGC	Add manual track management functions.
*			Add GetNearestTrackLatLong() function.
*   04/04/12 1.9    SP 	Add track creation callback.
*   29/03/12 1.8    SP 	Rename SPxTrackReport as SPxRadarTrack.
*   25/10/11 1.7    SP 	Change deleted list to free list.
*                       Add CreateTrack().
*                       Add track update callbacks.
*   14/10/11 1.6    SP 	Add m_deletedTrackListHead.
*                       Remove SPxTrackInfo_t.
*   27/07/11 1.5    SP 	Return distance from GetNearestTrack().
*                       Remove ReleaseTrack().
*                       Remove ClearTrails().
*   13/07/11 1.4    SP 	Increase default num trails points to 500.
*   07/07/11 1.3    SP 	Add optional arg to set max trail pts.
*                       Use SPxTrackReport class.
*   26/04/11 1.2    SP 	General enhancements.
*   12/04/11 1.1    SP 	Initial Version.
**********************************************************************/

#ifndef _SPX_TRACK_DATABASE_H
#define _SPX_TRACK_DATABASE_H

/*
 * Other headers required.
 */

/* We need SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need SPxPacketTrackerStatus_t. */
#include "SPxLibData/SPxPackets.h"

/* We need the SPxObj base class. */
#include "SPxLibUtils/SPxObj.h"

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
class SPxNavData;
class SPxFlightPlanDb;
class SPxSQLDatabase;
class SPxAsterixMsg;
class SPxAsterixDecoder;
class SPxRadarReplay;
class SPxRemoteServer;
class SPxPacketDecoder;
class SPxRadarTrack;
class SPxThread;
class SPxCallbackList;
struct SPxTime_tag;

typedef int (*SPxTrackDatabaseCompareFn_t)(const SPxRadarTrack*,
					   const SPxRadarTrack*);

/*
 * Define our class.
 */
class SPxTrackDatabase : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxTrackDatabase(unsigned int maxNumTrailPts=500,
                              double minTrailIntervalSecs=0.0,
                              unsigned int projectCode=0);
    virtual ~SPxTrackDatabase(void);

    /* Create the database */
    SPxErrorCode Create(SPxRemoteServer *remoteServer);

    /* Set optional remote server track input. */
    SPxErrorCode SetRemoteServer(SPxRemoteServer *remoteServer);

    /* Set optional packet decoder track input. */
    SPxErrorCode AddPacketDecoder(SPxPacketDecoder *packetDecoder);

    /* Add an optional Asterix input. */
    SPxErrorCode AddAsterixDecoder(SPxAsterixDecoder *asterixDecoder);

    /* Add an optional NavData object to pass radar lat/long to. */
    SPxErrorCode AddNavData(SPxNavData *navData);
    SPxErrorCode SetUseLatLongForNavData(int enable)
    {
	m_useLatLongForNavData = (enable ? TRUE : FALSE);
	return(SPX_NO_ERROR);
    }
    int GetUseLatLongForNavData(void) const { return(m_useLatLongForNavData); }

    /* Set an optional SPxFlightPlanDb database. */
    SPxErrorCode SetFlightPlanDb(SPxFlightPlanDb *fpdb);
    SPxFlightPlanDb *GetFlightPlanDb(void) { return(m_flightPlanDb); }

    /* Set an optional SQL database. */
    SPxErrorCode SetSQLDatabase(SPxSQLDatabase *sqlDB, UINT8 channelIndex=0);
    SPxSQLDatabase *GetSQLDatabase(void) { return m_sqlDB; }

    /* Enable/disable track statistics. */
    SPxErrorCode SetTrackStatisticsEnabled(int enable);

    /* Set or clear user-defined sensor lat/long. */
    SPxErrorCode SetSensorUserLatLong(double latDegs, double longDegs);
    SPxErrorCode SetSensorUserLatLong(REAL32 latDegs, REAL32 longDegs);
    SPxErrorCode ClearSensorUserLatLong(void);

    /* Get the sensor lat/long, if known (returns error otherwise). */
    SPxErrorCode GetSensorLatLong(double *latDegsPtr, double *longDegsPtr) const;
    SPxErrorCode GetSensorLatLong(REAL32 *latDegsPtr, REAL32 *longDegsPtr) const;

    /* Get the sensor altitude, if known (returns error otherwise). */
    SPxErrorCode GetSensorAltitudeMetres(double *altitudeMetresPtr) const;
    SPxErrorCode GetSensorAltitudeMetres(REAL32 *altitudeMetresPtr) const;

    /* Get the sensor period, if known (default otherwise). */
    double GetSensorPeriodSecs(void) const;

    /* Control whether the database calculates target lat/longs from range/azi
     * if possible (i.e. if radar lat/long is known).
     */
    SPxErrorCode SetCalculateLatLongs(int enable)
    {
	m_calculateLatLongs = (enable ? TRUE : FALSE);
	return(SPX_NO_ERROR);
    }
    int GetCalculateLatLongs(void)	{ return(m_calculateLatLongs); }

    /* Read tracks from the database. If calling this from within a callback,
     * the mutexAlreadyLocked flag should be set to TRUE to avoid a deadlock,
     * but the caller is responsible for ensuring the invoked function behaves
     * appropriately and does not modify the database.
     */
    SPxErrorCode IterateTracks(SPxCallbackListFn_t trackFn,
			       void *userArg,
			       int mutexAlreadyLocked=FALSE,
			       int maxPlotAge=0,
			       SPxTrackDatabaseCompareFn_t compareFn=NULL);

    unsigned int GetNumTracks(int maxPlotAge=0, int applyFilters=FALSE) const;
    unsigned int GetNumPlots(int maxPlotAge, int applyFilters=FALSE) const;

    /* Read strobes from the database.  See IterateTracks() mutex note.
     * The given userArg will be passed to the invoked function as the
     * userObject.  The strobe itself will be passed as a pointer to an
     * SPxPacketStrobe in the 'arg' parameter for the invoked function.
     * All un-purged strobes will be given, so the invoked function should
     * check the age of them if relevant.
     */
    SPxErrorCode IterateStrobes(SPxCallbackListFn_t strobeFn,
				void *userArg,
				int mutexAlreadyLocked=FALSE);

    /* Configure the track database */
    SPxErrorCode SetSenderID(UINT8 senderID);
    UINT8 GetSenderID(void) const { return m_senderID; }
    SPxErrorCode SetPurgeInterval(int interval);
    int GetPurgeInterval(void) const { return m_purgeInterval; }
    SPxErrorCode SetPurgeIntervalPlots(int interval);
    int GetPurgeIntervalPlots(void) const { return m_purgeIntervalPlots; }
    SPxErrorCode SetPurgeIntervalStrobes(int interval);
    int GetPurgeIntervalStrobes(void) const { return m_purgeIntervalStrobes; }
    SPxErrorCode SetPreserveTrailInterval(int interval);
    int GetPreserveTrailInterval(void) const { return m_preserveTrailInterval; }
    SPxErrorCode SetMinPreserveAgeSecs(double secs);
    double GetMinPreserveAgeSecs(void) const { return m_minPreserveAgeSecs; }
    SPxErrorCode SetPreserveIntervalSecs(double secs);
    double GetPreserveIntervalSecs(void) const { return m_preserveIntervalSecs; }

    /* Max number of trail points (changes only affect new tracks). */
    void SetMaxNumTrailPoints(unsigned int maxNumTrailPoints)
    {
	m_maxNumTrailPts = maxNumTrailPoints;
    }
    unsigned int GetMaxNumTrailPoints(void) const { return(m_maxNumTrailPts);}

    /* Manual track management. */
    SPxRadarTrack *AddTrack(UINT32 id=0);
    SPxErrorCode RemoveTrack(UINT32 id);

    /* Track access functions */
    SPxRadarTrack *GetTrack(UINT32 id, int idIsUnified = FALSE);
    SPxRadarTrack *GetNextTrack(UINT32 id, 
                                int nextHighest,
                                int allowWrap,
                                int idIsUnified = FALSE) const;
    SPxRadarTrack *GetNearestTrack(double xMetres, 
                                   double yMetres, 
                                   int filteredPos=TRUE,
                                   double *distMetresRtn=NULL,
                                   int maxPlotAge=0,
                                   int includePreserved=FALSE) const;
    SPxRadarTrack *GetNearestTrackLatLong(double latDegs,
					  double longDegs,
					  int filteredPos=TRUE,
					  double *distMetresRtn=NULL,
					  unsigned int radarIndex=0,
					  double labelMetresPerPixel=0.0,
					  int maxPlotAge=0,
					  int includePreserved=FALSE) const;
    double GetMaxTrackRangeMetres(void);

    /* Do not use the following function in normal situations. */
    SPxRadarTrack *GetTrack_InternalLocked(UINT32 id, int idIsUnified);

    /* Creation callback handling functions. The callback will be invoked with
     * a pointer to an SPxRadarTrack object whenever an object is created, but
     * the handle is only valid for the duration of the callback function.
     */
    SPxErrorCode AddCreationCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveCreationCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Update callback handling functions. The callback will be invoked with
     * a pointer to an SPxRadarTrack object whenever a track is updated, but
     * the handle is only valid for the duration of the callback function.
     */
    SPxErrorCode AddCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Functions for reporting tracks directly from SPxMHTnetReport. 
     * These functions have been made virtual at a customer's request,
     * so they can be overridden in a derived class.
     */
    virtual SPxErrorCode ReportTrack(struct SPxPacketTrackMinimal_tag *pMinRpt,
                                     struct SPxPacketTrackNormal_tag *pNormRpt,
                                     struct SPxPacketTrackExtended_tag *pExtRpt,
                                     const SPxAsterixMsg *pAsterixMsg=NULL);
    virtual SPxErrorCode ReportTrackerStatus(struct SPxPacketTrackerStatus_tag 
                                             *pStatusRpt,
                                             const SPxAsterixMsg *pAsterixMsg=NULL);
    virtual SPxErrorCode ReportStrobe(struct SPxPacketStrobe_tag *strobeRpt,
                                      const SPxAsterixMsg *pAsterixMsg=NULL);

    SPxErrorCode DeleteAll(void);

    /* Get last message times. */
    UINT32 GetLastTrackTimeMsecs(void) const { return m_lastTrackTimeMsecs; }
    UINT32 GetLastStatusTimeMsecs(void) const { return m_lastStatusTimeMsecs; }
    UINT32 GetMsecsSinceLastInput(void) const;

    /* Get last ASTERIX status message(s). */
    const SPxAsterixMsg *GetLastAsterixNorthCrossing(void) const;
    const SPxAsterixMsg *GetLastAsterixSector(void) const;

    SPxErrorCode AddRadarReplay(SPxRadarReplay *radarReplay);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Sender ID used to filter track reports */
    UINT8 m_senderID;

    /* Remote server object used to supply track reports */
    SPxRemoteServer *m_remoteServer;

    /* Optional asterix decoder for track reports. */
    SPxAsterixDecoder *m_asterixDecoder;

    /* Optional packet decoder for track reports. */
    SPxPacketDecoder *m_packetDecoder;

    /* Optional nav data object to be given lat/long received from radar. */
    SPxNavData *m_navData;
    int m_useLatLongForNavData;	/* Flag to enable/disable its use. */

    /* Optional database of flight plans. */
    SPxFlightPlanDb *m_flightPlanDb;

    /* Optional SQL database to record track reports and tracker status. */
    SPxSQLDatabase *m_sqlDB;
    UINT8 m_sqlDBChannelIndex;

    /* Project code for project specific decoding. */
    unsigned int m_projectCode;

    /* Linked-list of active tracks maintained in ascending track ID order */
    SPxRadarTrack *m_trackListHead;
    SPxRadarTrack *m_trackListTail;

    /* Linked-list of deleted tracks that are free to be reused */
    SPxRadarTrack *m_freeTrackListHead;
    SPxRadarTrack *m_freeTrackListTail;

    /* Linked-list of active plots, maintained in increasing age order. */
    SPxRadarTrack *m_plotListHead;
    SPxRadarTrack *m_plotListTail;

    /* Linked-list of deleted plots that are free to be reused. */
    SPxRadarTrack *m_freePlotListHead;
    SPxRadarTrack *m_freePlotListTail;

    /* Linked-list of strobe and free strobe holders. */
    struct SPxTrackDbStrobeInfo_tag *m_strobeListHead;
    struct SPxTrackDbStrobeInfo_tag *m_freeStrobeListHead;
    unsigned int m_numAllocatedStrobes;

    /* Track linked-list mutex */
    mutable SPxCriticalSection m_mutex;

    /* Creation callback list, i.e. functions to invoke when new track
     * radar track object is created.
     */
    SPxCallbackList *m_creationCallbackList;

    /* Update callback list, i.e. functions to invoke with new reports. */
    SPxCallbackList *m_updateCallbackList;

    /* Intervals after which non-updating tracks/plots are purged */
    int m_purgeInterval;
    int m_purgeIntervalPlots;
    int m_purgeIntervalStrobes;

    /* Interval to maintain deleted track trails/tracks. */
    int m_preserveTrailInterval;
    double m_minPreserveAgeSecs;
    double m_preserveIntervalSecs;

    /* Max number of trail points to keep */
    unsigned int m_maxNumTrailPts;

    /* Minimum interval between trail points */
    double m_minTrailIntervalSecs;

    /* Do we use radar lat/long to calculate track lat/longs from rng/azi? */
    int m_calculateLatLongs;

    /* Last tracker status report */
    SPxPacketTrackerStatus_t m_lastTrackerStatus;
    SPxAutoPtr<SPxAsterixMsg> m_lastAsterixMsgNorthCrossing;
    int m_lastAsterixMsgNorthCrossingValid;
    SPxAutoPtr<SPxAsterixMsg> m_lastAsterixMsgSector;
    int m_lastAsterixMsgSectorValid;

    /* Database maintenance thread */
    SPxThread *m_thread;

    /* Time last report received. */
    UINT32 m_lastTrackTimeMsecs;
    UINT32 m_lastStatusTimeMsecs;

    /* Are track statistics enabled? */
    int m_trackStatsEnabled;

    /* Sensor lat/long defined by user. */
    int m_isSensorUserLLSet;
    double m_sensorUserLatDegs;
    double m_sensorUserLongDegs;

    /*
     * Private functions.
     */
    /* Convert interval (+ve secs, -ve sweeps) to seconds. */
    double getSecsFromInterval(int interval) const;

    /* Create and delete tracks */
    SPxRadarTrack *CreateTrack(const SPxTime_tag *timestamp,
                               SPxPacketTrackMinimal *pMinRpt,
                               SPxPacketTrackNormal *pNormRpt,
                               SPxPacketTrackExtended *pExtRpt,
                               const SPxAsterixMsg *pAsterixMsg);
    SPxRadarTrack *DeleteTrack(SPxRadarTrack *trk, int invokeCallbacks=TRUE);
    SPxRadarTrack *getTrackInternal(UINT32 id, int alreadyLocked,
					int idIsUnified);

    /*
     * Private static functions.
     */

    static void *databaseMaintenanceHandler(SPxThread *thread);

    static void plotHandler(SPxRemoteServer *svr,
			    void *userArg,
			    struct sockaddr_in *from,
			    struct SPxTime_tag *timestamp,
			    struct SPxPacketPlot_tag *plot);

    static void plotStatusHandler(SPxRemoteServer *svr,
				  void *userArg,
				  struct sockaddr_in *from,
				  struct SPxTime_tag *timestamp,
				  struct SPxPacketPlotStatus_tag *plotStatus);

    static void trackHandler(SPxRemoteServer *svr,
			     void *userArg,
			     struct sockaddr_in *from,
			     struct SPxTime_tag *timestamp,
			     struct SPxPacketTrackMinimal_tag *pMinRpt,
			     struct SPxPacketTrackNormal_tag *pNormRpt,
			     struct SPxPacketTrackExtended_tag *pExtRpt);

    static void trackerStatusHandler(SPxRemoteServer *svr,
				     void *userArg,
				     struct sockaddr_in *from,
				     struct SPxTime_tag *timestamp,
				     struct SPxPacketTrackerStatus_tag 
				     *pStatusRpt);

    static void plotPktHandler(SPxPacketDecoder *decoder,
			       void *userArg,
			       UINT32 packetType,
			       struct sockaddr_in *from,
			       struct SPxTime_tag *time,
			       const unsigned char *payload,
			       unsigned int numBytes);

    static void plotPktStatusHandler(SPxPacketDecoder *decoder,
				     void *userArg,
				     UINT32 packetType,
				     struct sockaddr_in *from,
				     struct SPxTime_tag *time,
				     const unsigned char *payload,
				     unsigned int numBytes);

    static void trackPktHandler(SPxPacketDecoder *decoder,
				void *userArg,
				UINT32 packetType,
				struct sockaddr_in *from,
				struct SPxTime_tag *time,
				const unsigned char *payload,
				unsigned int numBytes);

    static void trackPktStatusHandler(SPxPacketDecoder *decoder,
				      void *userArg,
				      UINT32 packetType,
				      struct sockaddr_in *from,
				      struct SPxTime_tag *time,
				      const unsigned char *payload,
				      unsigned int numBytes);

    static void asterixTrackHandler(const SPxAsterixDecoder *decoder,
				    void *userArg,
				    struct SPxPacketTrackExtended_tag *rpt,
				    const SPxAsterixMsg *asterixMsg);
    static void asterixStatusHandler(const SPxAsterixDecoder *decoder,
				    void *userArg,
				    struct SPxPacketTrackerStatus_tag *rpt,
				    const SPxAsterixMsg *asterixMsg);
    static void asterixStrobeHandler(const SPxAsterixDecoder *decoder,
				    void *userArg,
				    struct SPxPacketStrobe_tag *rpt,
				    const SPxAsterixMsg *asterixMsg);

    static void replayTrackHandler(SPxPacketDecoder *pktDecoder,
				   void *userArg,
				   UINT32 packetType,
				   struct sockaddr_in *from,
				   struct SPxTime_tag *time,
				   const unsigned char *payload,
				   unsigned int numBytes);

    static int remoteServerDeleted(void *invokingObject,
				   void *userObject,
				   void *arg);

    static int packetDecoderDeleted(void *invokingObject,
				    void *userObject,
				    void *arg);


}; /* SPxTrackDatabase */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TRACK_DATABASE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
