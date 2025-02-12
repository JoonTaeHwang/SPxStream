/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAISDecoder class which supports the receipt
*	of AIS messages in SPx network format.
*
*
* Revision Control:
*   05/05/22 v1.45   AGC	Add public Cull() function.
*
* Previous Changes:
*   01/02/22 1.44   BTB	Allow configuration of max number of trail points.
*   01/10/21 1.43   AGC	Add max static draught to AIS info.
*   22/03/21 1.42   JGB	Removed redundant callback.
*   18/03/21 1.41   JGB	Added callback for tracks in DN.
*   21/09/20 1.40   AJH	Add callback for decoded message contents.
*   18/09/20 1.39   AGC	Support retrieving filtered number of tracks.
*   12/09/20 1.38   AJH	Add DeleteTrack().
*   07/09/20 1.37   AJH	Add Add GetLastMsgTimestamp().
*   17/07/20 1.36   AGC	Add DeleteAll().
*   29/11/19 1.35   AGC Support AtoNs.
*   20/11/19 1.34   AGC	Support nav data configuration.
*   10/05/19 1.33   AGC	Support building type 2 messages. 
*   15/01/19 1.32   REW	Optionally use extrapolation for Find functions.
*   14/01/19 1.31   REW	Support extrapolation.
*   23/11/18 1.30   REW	Add FindTracksInArea().
*   17/04/18 1.29   AGC	Handle changes packets.
*   12/03/18 1.28   AGC	Add GetTrack().
*   09/03/18 1.27   AGC	Add GetNumTracks().
*   02/02/18 1.26   AGC	Support RAIM configuration.
*			Support Class-B flags.
*   18/01/18 1.25   AGC	Support building Class-B messages.
*			Support building Base station messages.
*			Support Inland AIS messages.
*			Support assignment messages.
*   02/11/16 1.24   REW	Support SetTimeoutStatus() etc.
*   30/09/16 1.23   REW	Support initiallyDisabled constructor arg.
*   16/06/16 1.22   SP 	Support logging to SQL database.
*			Add FindFurthestTrack().
*   17/03/16 1.21   AJH	Update decodeSingleMessage declaration.
*   01/02/16 1.20   AGC	Support disabling passing on nav data.
*   09/11/15 1.19   AGC	Add constructor overload for testing.
*   03/11/15 1.18   AGC	Packet decoder callback data now const.
*   17/09/15 1.17   REW	Add atTimestamp to FindNearestTrack().
*   02/06/14 1.16   AGC	Support building own-ship messages.
*   04/10/13 1.15   AGC	Simplify headers.
*   05/09/13 1.14   SP 	Support multiple messages in single UDP packet.
*   08/04/13 1.13   AGC	Use SPxNMEADecoder.
*   21/21/12 1.12   AGC	Add convenience function SelectTrack().
*   09/11/12 1.11   AGC	Add functions to build AIS messages.
*   15/03/12 1.10   REW	Support GetLastMsgTimestamp().
*   07/03/12 1.9    REW	Support GetOwnMMSI() and SetUseVDOForNavData()
*   09/08/11 1.8    SP 	Derive from SPxDecoderRepeater.
*			Remove replay and record (now in baseclass).
*   27/07/11 1.7    SP 	Return distance from FindNearestTrack().
*   08/07/11 1.6    REW	Support record and replay.
*   29/06/11 1.5    REW	Decode more messages.
*   27/06/11 1.4    REW	Decode more fields for proper rendering etc.
*   06/04/11 1.3    AGC	Use new SPxPacketDecoderNet class.
*   04/04/11 1.2    REW	Add FindNearestTrack().
*   27/01/11 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_AIS_DECODE_H
#define _SPX_AIS_DECODE_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"

/* We need the SPxNMEADecoder class. */
#include "SPxLibNet/SPxNMEADecoder.h"

/* We need the SPxDecoderRepeater base class. */
#include "SPxLibNet/SPxDecoderRepeater.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Debug flags. */
#define	SPX_AIS_DEBUG_NEW		0x00000001	/* New target */
#define	SPX_AIS_DEBUG_LOST		0x00000002	/* Lost target */
#define	SPX_AIS_DEBUG_DELETED		0x00000004	/* Deleted target */
#define	SPX_AIS_DEBUG_MSG_OTHER		0x00000010	/* Other data */
#define	SPX_AIS_DEBUG_MSG_DYNAMIC	0x00000020	/* Dynamic data */
#define	SPX_AIS_DEBUG_MSG_STATIC	0x00000040	/* Static data */
#define	SPX_AIS_DEBUG_MSG_BASE		0x00000080	/* Base station */
#define	SPX_AIS_DEBUG_MSG_ATON		0x00000100	/* Aid to Navigation */
#define	SPX_AIS_DEBUG_MSG_SAR		0x00000200	/* Search and Rescue */
#define	SPX_AIS_DEBUG_VERBOSE		0x80000000	/* Verbose */


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

/* Need to forward-declare the class for use in the handler function type. */
class SPxAISDecoder;

/* We also use the SPxAISTrack class. */
class SPxAISTrack;

/* We may want these for record/replay. */
class SPxRunProcess;
class SPxRadarReplay;
class SPxSQLDatabase;

/* We use nav data. */
class SPxNavData;

/* This is for finding tracks in an area. */
struct SPxRendererXYD_tag;

/* AIS station type. */
enum SPxAISStationType
{
    SPX_AIS_STATION_CLASS_A = 0,
    SPX_AIS_STATION_CLASS_B_SO = 1,
    SPX_AIS_STATION_CLASS_B_CS = 2,
    SPX_AIS_STATION_BASE = 3,
    SPX_AIS_STATION_ATON = 4
};

/* AIS information structure. */
struct SPxAISInfo
{
    SPxAISStationType station;
    UINT32 mmsi;
    UINT8 navStatus;
    UINT32 imoNumber;
    char callSign[8];
    UINT8 shipType;
    UINT8 specManInd;
    UINT32 eta;
    char destination[21];
    int raim;
    int classBDisplay;
    int classBDsc;
    int classBBand;
    int classBMessage22;
    int inland;
    UINT32 eni;
    UINT16 eri;
    UINT8 hazardousCargo;
    UINT8 atonType;
    double maxStaticDraughtMetres;
};

/* AIS data for assignment callbacks. */
struct SPxAISAssignmentData
{
    int useMmsis;
    UINT32 mmsi1;
    UINT16 offset1;
    UINT32 mmsi2;
    UINT16 offset2;
    int useLatLong;
    double longDegs1;
    double latDegs1;
    double longDegs2;
    double latDegs2;
    UINT8 stationType;
    UINT8 shipCargoType;
    UINT8 txRxMode;
    UINT8 reportingInterval;
    UINT8 quietTime;
};

/* Required interface for controlling AIS objects. */
class SPxAISIface
{
public:
    SPxAISIface(void) {}
    virtual ~SPxAISIface(void) {}

    virtual int IsAISEditable(void) const = 0;
    virtual SPxErrorCode SetAISInfo(const SPxAISInfo *aisInfo) = 0;
    virtual SPxErrorCode GetAISInfo(SPxAISInfo *aisInfo) const = 0;
};

/*
 * Define our class, derived from the generic packet decoder.
 */
class SPxAISDecoder :public SPxDecoderRepeater
{
public:
    /*
     * Public types.
     */
    /* Define the type of the assignment callback function. */
    typedef void(*AssignmentFn_t)(SPxAISAssignmentData *data, void *userObj);

    /* Define the type of the decoded message function. */
    typedef void(*DecodedFn_t)(const char *msg, void *userObj);

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxAISDecoder(int initiallyDisabled=FALSE);
    virtual ~SPxAISDecoder(void);

    /* The address/port and other control functions from the base class
     * are also available.
     */

    /* Callback handling functions.  The callback will be invoked with
     * a pointer to an SPxAISTrack object whenever a track is updated, but
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

    /* Assignment handling functions. */
    SPxErrorCode AddAssignmentCallback(AssignmentFn_t fn, void *userObj);
    SPxErrorCode RemoveAssignmentCallback(AssignmentFn_t fn, void *userObj);

    /* Decoded message callbacks. */
    SPxErrorCode SetDecodedCallback(DecodedFn_t fn, void *userObj);

    /* Track iteration functions. */
    SPxErrorCode IterateTracks(SPxCallbackListFn_t fn, void *userArg);
    unsigned int GetNumTracks(int applyFilters=FALSE) const;
    SPxAISTrack *GetTrack(UINT32 mmsi);
    SPxAISTrack *FindNearestTrack(double latDegs, 
				  double longDegs,
				  double *distMetresRtn=NULL,
				  const SPxTime_t *atTimestamp=NULL,
				  int useExtrapolatedPositions=FALSE);
    SPxAISTrack *FindFurthestTrack(double latDegs, 
				   double longDegs,
				   double *distMetresRtn=NULL,
				   const SPxTime_t *atTimestamp=NULL,
				   int useExtrapolatedPositions=FALSE);
    unsigned int FindTracksInArea(const SPxRendererXYD_tag *latLongPoints,
					unsigned int numPoints,
					SPxAISTrack **trackList,
					unsigned int trackListSize,
					const SPxTime_t *atTimestamp=NULL,
					int alreadyLocked=FALSE,
					int useExtrapolatedPositions=FALSE);
    SPxErrorCode SelectTrack(SPxAISTrack *track);
    SPxAISTrack *GetSelectedTrack(void);
    SPxErrorCode DeleteTrack(SPxAISTrack *track);
    SPxErrorCode DeleteAll(void);
    SPxErrorCode GetLastMsgTimestamp(SPxTime_t *timestamp);

    /* Status functions. */
    UINT32 GetOwnMMSI(void) const	{ return(m_ownMMSI); }
    const SPxTime_t *GetLastMsgTimestamp(void) const
    {
	return(&m_lastMsgTimestamp);
    }

    /* Control options. */
    SPxErrorCode SetUseSourceTimestamps(int use)
    {
	m_useSourceTimestamps = (use ? TRUE : FALSE);
	return(SPX_NO_ERROR);
    }
    int GetUseSourceTimestamps(void) { return(m_useSourceTimestamps); }
    SPxErrorCode SetUseVDOForNavData(int use)
    {
	m_useVDOForNavData = (use ? TRUE : FALSE);
	return(SPX_NO_ERROR);
    }
    int GetUseVDOForNavData(void)	{ return(m_useVDOForNavData); }
    void SetDefaultActiveState(int active) { m_defaultActiveState = active; }
    int GetDefaultActiveState(void)	{ return(m_defaultActiveState); }
    SPxErrorCode SetPassOnNavData(int passOnNavData)
    {
	m_passOnNavData = (passOnNavData ? TRUE : FALSE);
	return(SPX_NO_ERROR);
    }
    int GetPassOnNavData(void)		{ return(m_passOnNavData); }
    void SetNavData(SPxNavData *navData,
		    unsigned int navDataID=0);
    SPxNavData *GetNavData(unsigned int navDataID=0,
 			   int mutexAlreadyLocked=FALSE);
    unsigned int GetNumNavData(void) const;

    /* Extrapolation control (disabled if period set to 0 or less). */
    SPxErrorCode SetExtrapolatePeriodSecs(unsigned int secs)
    {
	m_extrapolatePeriodSecs = secs;
	return(SPX_NO_ERROR);
    }
    unsigned int GetExtrapolatePeriodSecs(void) const
					{ return(m_extrapolatePeriodSecs); }
    SPxErrorCode Extrapolate(void);	/* Checks active and time has passed */

    /* Timeout controls. */
    void SetTimeoutLost(int secs)	{ m_timeoutLostSecs = secs; }
    int GetTimeoutLost(void) const	{ return(m_timeoutLostSecs); }
    void SetTimeoutAutoDelete(int secs)	{ m_timeoutAutoDeleteSecs = secs; }
    int GetTimeoutAutoDelete(void) const { return(m_timeoutAutoDeleteSecs);}
    void SetTimeoutStatus(int secs)	{ m_timeoutStatusSecs = secs; }
    int GetTimeoutStatus(void) const	{ return(m_timeoutStatusSecs);}

    /* History controls. */
    void SetMaxNumTrailPoints(unsigned int max)	{ m_maxNumTrailPoints = max; }
    unsigned int GetMaxNumTrailPoints(void) const { return(m_maxNumTrailPoints); }

    SPxErrorCode Cull(void);

    /* Install an optional SQL database. */
    SPxErrorCode SetSQLDatabase(SPxSQLDatabase *sqlDB, UINT8 channelIndex=0);
    SPxSQLDatabase *GetSQLDatabase(void) { return m_sqlDB; } 
    
    /* Convenience functions to build NMEA sentences. */
    static size_t BuildVdm1(char *sentence, size_t sizeBytes,
	UINT32 mmsi, UINT8 navStatus=15, double turnDegsPerMin=-128.0, double sogKnots=-1.0,
	int accPos=0, double longDegs=181.0, double latDegs=91.0, double cogDegs=-1.0,
	double trueHeadingDegs=-1.0, UINT8 timestamp=60, UINT8 specManInd=0, int raim=FALSE,
	int ownShip=FALSE, char channel='A')
    {
	return BuildVdm1or2(sentence, sizeBytes,
			    mmsi, navStatus, turnDegsPerMin, sogKnots,
			    accPos, longDegs, latDegs, cogDegs,
			    trueHeadingDegs, timestamp, specManInd, raim,
			    ownShip, channel, FALSE);
    }
    static size_t BuildVdm1or2(char *sentence, size_t sizeBytes,
	UINT32 mmsi, UINT8 navStatus=15, double turnDegsPerMin=-128.0, double sogKnots=-1.0,
	int accPos=0, double longDegs=181.0, double latDegs=91.0, double cogDegs=-1.0,
	double trueHeadingDegs=-1.0, UINT8 timestamp=60, UINT8 specManInd=0, int raim=FALSE,
	int ownShip=FALSE, char channel='A', int assigned=FALSE);
    static size_t BuildVdm4(char *sentence, size_t sizeBytes,
	UINT32 mmsi, SPxTime_t *time=NULL, int accPos=0,
	double longDegs=181.0, double latDegs=91.0, int raim=FALSE,
	int ownShip=FALSE, char channel='A');
    static size_t BuildVdm5(char *sentence, size_t sizeBytes, 
	UINT32 mmsi, UINT8 aisVersion, UINT32 imoNum=0,
	const char *callSign=NULL, const char *name=NULL,
	UINT8 shipType=0, double metresToBow=0.0, double metresToStern=0.0,
	double metresToPort=0.0, double metresToStarboard=0.0,
	UINT8 posDefType=0, UINT32 eta=1596, double maxStaticDraughtMetres=0.0,
	const char *destination=NULL, int dte=1, int ownShip=FALSE,
	char channel='A', int id=-1);
    static size_t BuildVdmInland10(char *sentence, size_t sizeBytes,
	UINT32 mmsi, UINT32 eni=0,
	double lengthMetres=0.0, double beamMetres=0.0,
	UINT16 eri=0, int hazardousCargo=5, double maxStaticDraughtMetres=0.0,
	int ownShip=FALSE, char channel='A');
    static size_t BuildVdm16(char *sentence, size_t sizeBytes,
	UINT32 sourceMmsi, UINT32 destMmsiA, UINT32 offsetA, UINT32 incrementA,
	UINT32 destMmsiB, UINT32 offsetB, UINT32 incrementB, char channel='A');
    static size_t BuildVdm18(char *sentence, size_t sizeBytes,
	SPxAISStationType station,
	UINT32 mmsi, double sogKnots=-1.0,
	int accPos=0, double longDegs=181.0, double latDegs=91.0,
	double cogDegs=-1.0, double trueHeadingDegs=-1.0, UINT8 timestamp=60,
	int displayFlag=0, int dscFlag=0, int bandFlag=0, int message22Flag=0,
	int mode=0, int raim=FALSE,
	int ownShip=FALSE, char channel='A');
    static size_t BuildVdm19(char *sentence, size_t sizeBytes,
	UINT32 mmsi, double sogKnots=-1.0,
	int accPos=0, double longDegs=181.0, double latDegs=91.0,
	double cogDegs=-1.0, double trueHeadingDegs=-1.0, UINT8 timestamp=60,
	const char *name=NULL, UINT8 shipCargoType=0,
	double metresToBow=0.0, double metresToStern=0.0,
	double metresToPort=0.0, double metresToStarboard=0.0,
	UINT8 posDefType=0, int raim=FALSE, int dte=1, int mode=0,
	int ownShip=FALSE, char channel='A', int id=-1);
    static size_t BuildVdm21(char *sentence, size_t sizeBytes,
	UINT32 mmsi, UINT8 atonType=0,
	const char *name=NULL, int accPos=0,
	double longDegs=181.0, double latDegs=91.0,
	double metresToBow=0.0, double metresToStern=0.0,
	double metresToPort=0.0, double metresToStarboard=0.0,
	UINT8 timestamp=60, int raim=FALSE,
	int assigned=FALSE, char channel='A');
    static size_t BuildVdm23(char *sentence, size_t sizeBytes, 
	UINT32 sourceMmsi, double longDegs1, 
	double latDegs1, double longDegs2, 
	double latDegs2, UINT8 stationType, 
	UINT8 shipCargoType, UINT8 transmitMode, 
	UINT8 reportingInterval, UINT8 quietTime, 
	char channel='A');
    static size_t BuildVdm24A(char *sentence, size_t sizeBytes,
	UINT32 mmsi, const char *name=NULL, int ownShip=FALSE,
	char channel='A');
    static size_t BuildVdm24B(char *sentence, size_t sizeBytes,
	UINT32 mmsi, UINT8 shipType=0,
	const char *vendorID=NULL,
	const char *callSign=NULL,
	double metresToBow=0.0, double metresToStern=0.0,
	double metresToPort=0.0, double metresToStarboard=0.0,
	UINT8 posDefType=0, int ownShip=FALSE,
	char channel='A');

    /* Convenience functions to get AIS message intervals. */
    static int IsChangingCourse(double headingDegs,
				double meanHeadingDegs,
				UINT32 *changeCourseTime);
    static UINT32 GetPosIntervalMS(SPxAISStationType station,
				   double sogKnots,
				   UINT8 navStatus=15,
				   int isChangingCourse=FALSE);
    static UINT32 GetStaticIntervalMS(void);

    /* Generic parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Static public functions for log file debug support. */
    static void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    static FILE *GetLogFile(void)		{ return(m_logFile); }
    static void SetDebug(UINT32 debug)		{ m_debug = debug; }
    static UINT32 GetDebug(void)		{ return(m_debug); }

protected:
    /* Constructor for testing. */
    explicit SPxAISDecoder(SPxPacketDecoderNet *netPacketDecoder);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Lists of AIS tracks. */
    mutable SPxCriticalSection m_listMutex;	/* Mutex to protect lists */
    SPxAISTrack *m_activeTracks;		/* List of active tracks */
    SPxAISTrack *m_freeTracks;			/* List of free tracks */

    /* Callback list, i.e. functions to invoke with new reports. */
    SPxCallbackList *m_callbackList;

    /* NMEA decoder. */
    SPxNMEADecoder m_nmeaDecoder;		/* NMEA decoder object. */

    /* Control flags. */
    int m_useSourceTimestamps;			/* Use timestamps from src? */
    int m_useVDOForNavData;			/* Pass VDO data to NavData */
    int m_passOnNavData;			/* Pass nav data to NavData */
    int m_defaultActiveState;			/* Default for new tracks */
    int m_timeoutLostSecs;			/* Timeout for lost tracks */
    int m_timeoutAutoDeleteSecs;		/* Deletion after being lost */
    int m_timeoutStatusSecs;			/* Timeout for health status */
    unsigned int m_maxNumTrailPoints;		/* Upper limit on history */

    /* Extrapolation control. */
    unsigned int m_extrapolatePeriodSecs;	/* 0 to disable */
    SPxTime_t m_lastExtrapolateTimestamp;	/* Time of last action */

    /* Fields for handling multi-part messages. */
    unsigned char *m_multiBuffer;		/* For gathering messages */
    unsigned int m_multiBufferSize;		/* Size of buffer in bytes */
    unsigned int m_multiBufferBytesPresent;	/* Number of bytes so far */
    unsigned int m_multiSeqId;			/* Numerical seq ID. */
    int m_multiSeqIdValid;			/* Is the numerical seq ID valid? */

    /* Status fields etc. */
    SPxTime_t m_creationTimestamp;		/* Time of creation */
    SPxTime_t m_lastMsgTimestamp;		/* Time of last message */
    UINT32 m_ownMMSI;				/* Own ship MMSI from VDO */

    /* Optional SQL database. */
    SPxSQLDatabase *m_sqlDB;
    UINT8 m_sqlDBChannelIndex;

    /* Static log file debug support. */
    static FILE *m_logFile;			/* Destination for msgs */
    static UINT32 m_debug;			/* Debug flags */



    /*
     * Private functions.
     */
    void init(void);
    static UINT16 staticGetSystemSourceHealth(void *userArg);
    UINT16 getSystemSourceHealth(void);
    static void staticDecodePacket(SPxPacketDecoder *packetDecoder,
				   void *userArg,
				   UINT32 packetType,
				   struct sockaddr_in *from,
				   SPxTime_t *timestamp,
				   const unsigned char *payload,
				   unsigned int numBytes);
    SPxErrorCode decodePacket(UINT32 packetType,
			      struct sockaddr_in *from,
			      SPxTime_t *timestamp,
			      const unsigned char *payload,
			      unsigned int numBytes);
    SPxErrorCode decodeSingleMsg(UINT32 packetType,
				 SPxTime_t *timestamp,
				 const unsigned char *payload,
				 unsigned int numBytes,
				 struct sockaddr_in *from);
    SPxAISTrack *decodeMsg1to3(SPxTime_t *timestamp, int msgType,
			       const unsigned char *data, int dataLen);
    SPxAISTrack *decodeMsg4(SPxTime_t *timestamp, int msgType,
			    const unsigned char *data, int dataLen);
    SPxAISTrack *decodeMsg5(SPxTime_t *timestamp, int msgType,
			    const unsigned char *data, int dataLen);
    SPxAISTrack *decodeMsg8(SPxTime_t *timestamp, int msgType,
			    const unsigned char *data, int dataLen);
    SPxAISTrack *decodeMsg9(SPxTime_t *timestamp, int msgType,
			    const unsigned char *data, int dataLen);
    void decodeMsg16(SPxTime_t *timestamp, int msgType,
		     const unsigned char *data, int dataLen);
    SPxAISTrack *decodeMsg18(SPxTime_t *timestamp, int msgType,
			     const unsigned char *data, int dataLen);
    SPxAISTrack *decodeMsg19(SPxTime_t *timestamp, int msgType,
			     const unsigned char *data, int dataLen);
    SPxAISTrack *decodeMsg21(SPxTime_t *timestamp, int msgType,
			     const unsigned char *data, int dataLen);
    void decodeMsg23(SPxTime_t *timestamp, int msgType,
		     const unsigned char *data, int dataLen);
    SPxAISTrack *decodeMsg24(SPxTime_t *timestamp, int msgType,
			     const unsigned char *data, int dataLen);
    SPxAISTrack *getTrackForMMSI(UINT32 mmsi);
    SPxErrorCode cullDeadTracks(void);
    SPxErrorCode updateNavDataFromTrack(SPxAISTrack *track);

    SPxAISTrack *findTrack(int findNearest,
			   double latDegs,
			   double longDegs,
			   double *distMetresRtn,
			   const SPxTime_t *atTimestamp,
			   int useExtrapolatedPositions);

}; /* SPxAISDecoder */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_AIS_DECODE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
