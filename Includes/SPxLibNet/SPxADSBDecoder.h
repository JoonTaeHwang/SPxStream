/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxADSBDecoder class which supports the receipt
*	of ADSB messages in SPx network format.
*
*
* Revision Control:
*   05/05/22 v1.33   AGC	Add public Cull() function.
*
* Previous Changes:
*   10/01/22 1.32   AGC	Decode parts of Aircraft Operation Status Messages.
*   04/11/21 1.31   REW	Add optional trackNumber to getTrackForAA().
*   19/08/21 1.30   AJH	Support ignoring NOGO bit.
*   16/02/21 1.29   REW	Decode 112-bit message type 28 for Mode 3/A code.
*   03/02/21 1.28   JGB	Added Track selection.
*   28/01/21 1.27   JGB	Brought over functionality from SPxAISDecoder.
*   18/09/20 1.26   AGC Support retrieving filtered number of tracks.
*   05/08/20 1.25   BTB	Add reportTrackUpdate().
*   04/08/20 1.24   BTB	Make decodePacket() protected.
*   17/07/20 1.23   AGC	Add DeleteAll().
*   22/01/20 1.22   REW	Support MaxNumTrailPoints control.
*   21/01/20 1.21   REW	Support SrcVersion parameter for CAT21 version.
*   23/12/19 1.20   REW	Support NavData for receiver position.
*   26/04/19 1.19   REW	Add alreadyLocked param to GetTrack().
*   16/07/18 1.18   AJH	Add InjectTrack().
*   09/03/18 1.17   AGC	Add GetNumTracks().
*			Add GetTrack().
*   30/09/16 1.16   REW	Support initiallyDisabled constructor arg.
*   23/08/16 1.15   SP 	Add IsPayloadAsterixCat21().
*   28/07/16 1.14   AGC	Support checking distance to history.
*		    SP 	Store packet time.
*   16/06/16 1.13   SP 	Support logging to SQL database.
*                       Add FindFurthestTrack().
*   03/11/15 1.12   AGC	Packet decoder callback data now const.
*   25/06/15 1.11   REW	Add atTimestamp to FindNearestTrack().
*   23/06/15 1.10   AGC	Add serial manufacturer configuration.
*   15/10/14 1.9    REW	Move ModeCToFeet() to SPxADSBUtils.
*   03/09/14 1.8    REW	Add ModeCToFeet().
*   29/07/14 1.7    REW	Add GillhamToFeet().
*   04/10/13 1.6    AGC	Simplify headers.
*   26/09/13 1.5    REW	Add GetMsecsSinceLastMsg().
*   03/05/13 1.4    AGC	Support Asterix CAT-21 input.
*   28/09/12 1.3    REW	Support creation time.
*   05/04/12 1.2    REW	Start supporting surface messages.
*   23/12/11 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_ADSB_DECODE_H
#define _SPX_ADSB_DECODE_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxCallbackList.h"

/* We need the SPxDecoderRepeater base class. */
#include "SPxLibNet/SPxDecoderRepeater.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Debug flags. */
#define	SPX_ADSB_DEBUG_NEW		0x00000001	/* New target */
#define	SPX_ADSB_DEBUG_LOST		0x00000002	/* Lost target */
#define	SPX_ADSB_DEBUG_DELETED		0x00000004	/* Deleted target */
#define	SPX_ADSB_DEBUG_MSG_ALL		0x00000010	/* All messages */
#define	SPX_ADSB_DEBUG_MSG_ID		0x00000020	/* ID data */
#define	SPX_ADSB_DEBUG_MSG_POSITION	0x00000100	/* Airborne Position */
#define	SPX_ADSB_DEBUG_MSG_ALTITUDE	0x00000200	/* Airborne Altitude */
#define	SPX_ADSB_DEBUG_MSG_VELOCITY	0x00000400	/* Airborne Velocity */
#define	SPX_ADSB_DEBUG_MSG_SURFACE	0x00000800	/* Surface Reports */
#define	SPX_ADSB_DEBUG_MSG_OTHER	0x00010000	/* Other messages */
#define	SPX_ADSB_DEBUG_VERBOSE		0x80000000	/* Verbose */


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
class SPxAsterixDecoder;
class SPxADSBTrack;
class SPxAsterixMsg;
class SPxSQLDatabase;
class SPxNavData;

/*
 * Define our class, derived from the generic packet decoder.
 */
class SPxADSBDecoder :public SPxDecoderRepeater
{
public:
    /*
     * Public variables.
     */

     /* Define the type of the decoded message function. */
    typedef void(*DecodedFn_t)(const char* msg, void* userObj);

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxADSBDecoder(int initiallyDisabled=FALSE);
    virtual ~SPxADSBDecoder(void);

    /* The address/port and other control functions from the base class
     * are also available.
     */
    virtual SPxErrorCode SetSrcSerialManufacturer(SPxDeviceManufacturer_t manufacturer);
    virtual SPxErrorCode SetSrcVersion(const char *version);
    virtual SPxErrorCode GetSrcVersion(char *buf, int bufLen);

    /* The lat/long of the antenna is required to support decoding.
     * If a NavData object is set, the position is obtained from that,
     * but otherwise the value set with SetLatLong() is used instead.
     */
    void SetNavData(SPxNavData *navData) { m_navData = navData; }
    SPxNavData *GetNavData(void)	{ return(m_navData); }
    SPxErrorCode SetLatLong(double latDegs, double longDegs);
    double GetLatDegs(void) const	{ return(m_latDegs); }
    double GetLongDegs(void) const	{ return(m_longDegs); }

    /* Callback handling functions.  The callback will be invoked with
     * a pointer to an SPxADSBTrack object whenever a track is updated, but
     * the handle is only valid for the duration of the callback function.
     */
    SPxErrorCode AddCallback(SPxCallbackListFn_t fn, void *userObj)
    {
	SPxErrorCode err = SPX_ERR_NOT_INITIALISED;
	if( m_callbackList )
	{
	    err = (SPxErrorCode)m_callbackList->AddCallback(fn, userObj);
	}
	return(err);
    }
    SPxErrorCode RemoveCallback(SPxCallbackListFn_t fn, void *userObj)
    {
	SPxErrorCode err = SPX_ERR_NOT_INITIALISED;
	if( m_callbackList )
	{
	    err = (SPxErrorCode)m_callbackList->RemoveCallback(fn, userObj);
	}
	return(err);
    }

    /* Decoded message callbacks. */
    SPxErrorCode SetDecodedCallback(DecodedFn_t fn, void* userObj);

    /* Track iteration functions. */
    SPxErrorCode IterateTracks(SPxCallbackListFn_t fn, void *userArg);
    unsigned int GetNumTracks(int applyFilters=FALSE) const;
    SPxADSBTrack *GetTrack(UINT32 aa, int alreadyLocked=FALSE);
    SPxADSBTrack *FindNearestTrack(double latDegs,
				   double longDegs,
				   double *distMetresRtn=NULL,
				   const SPxTime_t *atTimestamp=NULL);
    SPxADSBTrack *FindFurthestTrack(double latDegs,
				    double longDegs,
				    double *distMetresRtn=NULL,
				    const SPxTime_t *atTimestamp=NULL);
    int IsTrackHistoryClose(double latDegs,
			    double longDegs,
			    double maxDistMetres);
    SPxADSBTrack *InjectTrack(UINT32 aa, const char *callsign,
			    double latDegs, double longDegs,
			    double altitude=0.0,
			    double speed=0.0, double course=0.0,
                            SPxTime_t *timestamp=NULL,
                            int altitudeIsBarometric=FALSE);
    SPxErrorCode SelectTrack(SPxADSBTrack* track);
    SPxADSBTrack* GetSelectedTrack(void);
    SPxErrorCode DeleteTrack(SPxADSBTrack* track);
    SPxErrorCode DeleteAll(void);
    SPxErrorCode GetLastMsgTimestamp(SPxTime_t *timestamp);

    /* Control options. */
    SPxErrorCode SetUseSourceTimestamps(int use)
    {
	m_useSourceTimestamps = (use ? TRUE : FALSE);
	return(SPX_NO_ERROR);
    }
    int GetUseSourceTimestamps(void) { return(m_useSourceTimestamps); }
    void SetDefaultActiveState(int active) { m_defaultActiveState = active; }
    int GetDefaultActiveState(void)	{ return(m_defaultActiveState); }
    void SetIgnoreNogo(int nogo) { m_ignoreNogo = nogo; }
    int GetIgnoreNogo(void) { return(m_ignoreNogo); }

    /* Timeout controls. */
    void SetTimeoutLost(int secs)	{ m_timeoutLostSecs = secs; }
    int GetTimeoutLost(void)		{ return(m_timeoutLostSecs); }
    void SetTimeoutAutoDelete(int secs)	{ m_timeoutAutoDeleteSecs = secs; }
    int GetTimeoutAutoDelete(void)	{ return(m_timeoutAutoDeleteSecs);}

    /* History controls. */
    void SetHistorySecs(unsigned int secs)	{ m_historySecs = secs; }
    unsigned int GetHistorySecs(void) const	{ return(m_historySecs); }
    void SetMaxNumTrailPoints(unsigned int max)	{ m_maxNumTrailPoints = max; }
    unsigned int GetMaxNumTrailPoints(void) const { return(m_maxNumTrailPoints); }

    /* Status retrieval. */
    UINT32 GetMsecsSinceLastMsg(void);

    SPxErrorCode Cull(void);

    /* Install an optional SQL database. */
    SPxErrorCode SetSQLDatabase(SPxSQLDatabase *sqlDB, UINT8 channelIndex=0);
    SPxSQLDatabase *GetSQLDatabase(void) { return m_sqlDB; } 

    /* Generic parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Static public functions for log file debug support. */
    static void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    static FILE *GetLogFile(void)		{ return(m_logFile); }
    static void SetDebug(UINT32 debug)		{ m_debug = debug; }
    static UINT32 GetDebug(void)		{ return(m_debug); }

    /* Static utility functions. */
    static int IsPayloadAsterixCat21(const unsigned char *payload,
				     unsigned int numBytes);

protected:
    /* Protected functions for testing. */
    SPxADSBTrack *getTrackForAA(UINT32 aa, SPxTime_t *timestamp,
					int trackNumber=-1);
    void setTrackPosition(SPxADSBTrack *track, double latDegs, double longDegs,
			  SPxTime_t *timestamp);
    void setTrackCallsign(SPxADSBTrack *track, const char *callsign,
			  SPxTime_t *timestamp);

    /* This is protected so it can be overriden in the derived class
     * used for decoding FR24-based ADSB data.
     */
    virtual SPxErrorCode decodePacket(UINT32 packetType,
                                      struct sockaddr_in *from,
                                      SPxTime_t *timestamp,
                                      const unsigned char *payload,
                                      unsigned int numBytes);

    virtual void reportTrackUpdate(SPxADSBTrack *track, SPxTime_t *time,
                                   int messageType=-1);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Asterix decoder. */
    SPxAsterixDecoder *m_asterixDecoder;	/* Asterix decoder. */
    SPxADSBTrack *m_track;			/* Current ADS-B track being created. */
    char *m_srcVersion;				/* Version expected */

    /* Antenna location (required for decoding).  See notes for functions
     * above for precedence.
     */
    SPxNavData *m_navData;			/* Object to use if set */
    double m_latDegs;				/* Positive north */
    double m_longDegs;				/* Positive east */

    /* Lists of ADSB tracks. */
    mutable SPxCriticalSection m_listMutex;	/* Mutex to protect lists */
    SPxADSBTrack *m_activeTracks;		/* List of active tracks */
    SPxADSBTrack *m_freeTracks;			/* List of free tracks */

    /* Callback list, i.e. functions to invoke with new reports. */
    SPxCallbackList *m_callbackList;

    /* Control flags. */
    int m_useSourceTimestamps;			/* Use timestamps from src? */
    int m_defaultActiveState;			/* Default for new tracks */
    int m_timeoutLostSecs;			/* Timeout for lost tracks */
    int m_timeoutAutoDeleteSecs;		/* Deletion after being lost */
    unsigned int m_historySecs;			/* Number of seconds of history to save. */
    unsigned int m_maxNumTrailPoints;		/* Upper limit on history */
    int m_ignoreNogo;				/* Dont use NOGO to reject tracks. */

    /* Status. */
    SPxTime_t m_lastPktTime;    /* Timestamp for last packet received. */
    UINT32 m_lastMsgMsecs;	/* Timestamp for last message accepted */

    /* Optional SQL database. */
    SPxSQLDatabase *m_sqlDB;
    UINT8 m_sqlDBChannelIndex;

    /* Static log file debug support. */
    static FILE *m_logFile;			/* Destination for msgs */
    static UINT32 m_debug;			/* Debug flags */

    /*
     * Private functions.
     */
    static void staticDecodePacket(SPxPacketDecoder *packetDecoder,
					void *userArg,
					UINT32 packetType,
					struct sockaddr_in *from,
					SPxTime_t *timestamp,
					const unsigned char *payload,
					unsigned int numBytes);
    SPxErrorCode decodeMsg1to4(SPxADSBTrack *track,
				SPxTime_t *timestamp, unsigned int meType,
				const unsigned char *data, unsigned int threeBits);
    SPxErrorCode decodeMsg5to8(SPxADSBTrack *track,
				SPxTime_t *timestamp, unsigned int meType,
				const unsigned char *data, unsigned int threeBits);
    SPxErrorCode decodeMsg9to22not19(SPxADSBTrack *track,
				SPxTime_t *timestamp, unsigned int meType,
				const unsigned char *data, unsigned int threeBits);
    SPxErrorCode decodeMsg19(SPxADSBTrack *track,
				SPxTime_t *timestamp, unsigned int meType,
				const unsigned char *data, unsigned int threeBits);
    SPxErrorCode decodeMsg28(SPxADSBTrack *track,
				SPxTime_t *timestamp, unsigned int meType,
				const unsigned char *data, unsigned int threeBits);
    SPxErrorCode decodeMsg31(SPxADSBTrack *track,
				SPxTime_t *timestamp, unsigned int meType,
				const unsigned char *data, unsigned int threeBits);
    SPxErrorCode cullDeadTracks(void);
    SPxErrorCode cullTrackHistory(void);
    SPxErrorCode getLatLongFromCPR(unsigned int xz,
					unsigned int yz,
					unsigned int i,
					double *latDegsPtr,
					double *longDegsPtr,
					int surfaceNotAir);
    static void asterixFnWrapper(const SPxAsterixDecoder *decoder,
				 void *userArg,
				 UINT8 category,
				 SPxAsterixMsg *msg);
    void asterixFn(SPxAsterixMsg *msg);
    static void add128thsDiffToTime(SPxTime_t *timestamp, UINT32 first128, UINT32 second128);

    SPxADSBTrack *findTrack(int findNearest,
                            double latDegs, 
                            double longDegs,
                            double *distMetresRtn,
                            const SPxTime_t *atTimestamp);

    void saveTrackHistory(double latDegs, double longDegs, const SPxTime_t *timestamp);

}; /* SPxADSBDecoder */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_ADSB_DECODE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
