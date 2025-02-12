/*********************************************************************
*
* (c) Copyright 2007 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Scenario Generator.
*
* Revision Control:
*   14/11/19 v1.48   AGC	Suppress spurious cppcheck warning.
*
* Previous Changes:
*   12/11/19 1.47   AGC	Fix data races when used for testing.
*   08/03/17 1.46   AGC	Make various functions const.
*   03/11/15 1.45   AGC	Packet decoder callback data now const.
*   24/02/14 1.44   AGC	Expose base class Set/GetParameter to allow for testing.
*   17/02/14 1.43   AGC	Use const for radarName parameter.
*   18/11/13 1.42   AGC	Fix clang warning.
*   18/10/12 1.41   AGC	Add RemoveAllClutterAreas().
*   31/08/12 1.40   AGC	Reduce header dependencies.
*			Add Set/GetNumSamples().
*   29/06/12 1.39   AGC	Add clutter areas and free text fields.
*   06/06/12 1.38   AGC	Send delete track report when target removed.
*   29/05/12 1.37   AGC	Add TTL support.
*			Add track report capability.
*   29/03/12 1.36   AGC	Add functions to query noise settings.
*   25/11/11 1.35   AGC	Prevent update to target position while drawing. 
*   02/11/11 1.34   AGC	Handle targets being bumped off path.
*			Add Set/GetSendInterval().
*			Add Set/GetPurgeInterval().
*   25/10/11 1.33   AGC	Add target distribution and receipt.
*   24/05/11 1.32   AGC	Add IsTargetId() function.
*   24/05/11 1.31   DGJ	Support attributes of scenario paths
*   12/04/11 1.30   AGC	Set target convenience values immediately when
*			target created or moved.
*   18/02/11 1.29   REW	Re-instate SPX_SCENGEN_MAX_NUM_TARGETS.
*   18/02/11 1.28   AGC	Configurable PRF.
*			Remove maximum target limit.
*			Add RemoveTarget() function.
*			Add callbacks called before processing a specified azi.
*   17/12/10 1.27   AGC	Use SPxPath for storing filename.
*   28/10/10 1.26   AGC	Store last loaded filename.
*   15/09/10 1.25   REW	Make destructor virtual.
*   30/09/09 1.24   REW	Make fractional position flag configurable.
*   14/09/09 1.23   DGJ	Support lat/long in simulated nav data.
*   08/06/09 1.22   SP 	Add new data retrieval functions.
*			Increase number of targets to 100.
*   13/05/09 1.21   DGJ	Get target access functions
*   24/04/09 1.20   DGJ	Support SetRadar command
*   06/04/09 1.19   DGJ	Support North-Up Mode and moving platforms.
*   19/03/09 1.18   REW	Add SetAllRadarsMotion().
*   19/03/09 1.17   DGJ	Change type for target lastUpdateTime to UINT32
*			Support moving platform (UpdateRadar)
*   25/11/08 1.16   REW	Support GetParameter().
*   25/07/08 1.15   DGJ	Support paths
*   15/07/08 1.14   DGJ	Support ReadScenarioFromFile
*   23/01/08 1.13   DGJ	Update targets in sync with radar 1
*   16/11/07 1.12   REW	Add mutex.
*   16/11/07 1.11   REW	Work per azimuth, not per sector.
*   24/09/07 1.10   DGJ	Support SetTargetLevel().
*   11/09/07 1.9    DGJ	Support SetTargetRangeDimension() and
*			SetTargetActive().
*   02/09/07 1.8    DGJ	Access functions to change target state.
*			Reduce max number of targets to 64
*   24/08/07 1.7    DGJ	Support GetRadarDatabase API.
*			Error checking in addRadar()
*   26/07/07 1.6    REW	m_lastUpdateTime needs to be UINT32.
*   29/06/07 1.5    DGJ	Derived from SPxObj, support SetParameter()
*   27/06/07 1.4    DGJ	Add SetTargetRangeSize().
*   07/06/07 1.3    REW	Tidy, comments, format, thread objects,
*			less public functions etc.
*   24/05/07 1.2    REW	Added header, changed to SPx etc.
*   20/05/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_SCENGEN_H
#define _SPX_SCENGEN_H

/*
 * Other headers required.
 */

/* Standard headers. */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For common types. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxCriticalSection class. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/* For SPxPath class. */
#include "SPxLibUtils/SPxSysUtils.h"

/* For SPxReturnHeader structure. */
#include "SPxLibData/SPxRib.h"

/* For base class. */
#include "SPxLibData/SPxRadarSource.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Define the maximum number of radar buffers we'll feed. */
#define	SPX_SCENGEN_MAX_NUM_RIBS	10

/* For backwards compatibility, we still define the following value in
 * case applications are relying on it for sizing things internally etc.,
 * although as of 18/02/11 there is no specific maximum inside the class
 * itself so it is possible to create more targets than this.
 */
#define	SPX_SCENGEN_MAX_NUM_TARGETS	100	/* Not used anymore */

/* Maximum text field name and value lengths.
 * Names and values longer than this will be truncated.
 */
#define SPX_SCENGEN_MAX_TEXT_FIELD_NAME_LEN	132
#define SPX_SCENGEN_MAX_TEXT_FIELD_VALUE_LEN	1024

/* Target alignment */
enum
{
    SPX_SCENGEN_TARGET_ALIGN_CENTRE = 0,	/* Align to centre */
    SPX_SCENGEN_TARGET_ALIGN_LEADING_EDGE = 1	/* Align to leading edge */
};

/* Target state. */
enum SPxSGtargetState
{
    SPX_SCENGEN_TARGET_EXISTS = 0,  /* Target exists or has been created. */
    SPX_SCENGEN_TARGET_REMOVED = 1  /* Target has been removed. */
};

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

/* Forward define the external classes we use. */
class SPxRIB;
class SPxRadar;
class SPxRadarDatabase;
class SPxSGpath;
class SPxSGpathPoint;
class SPxActiveRegion;
class SPxNavData;
class SPxPacketSender;
class SPxPacketDecoderNet;
class SPxPacketDecoder;
class SPxScenarioGenerator;
struct SPxScenGenClutterArea;


/* Define a structure for individual target information. */
typedef struct 
{
    int id;		// An id that we can used to refer to the target
    double x;		// World x
    double y;		// World y
    double breadth;	// Breadth (in metres)
    double length;	// Length (in metres)
    double heading;	// Heading (in degrees)
    double speed;	// Speed (world units per second)
    double pathSpeedWeight;	// Speed weight of path (if any) 
    double pathIntensityWeight;	// Intensity weight of path (if any)
    int isActive;       // This target is active
    int isVisible;	// Is the target visible ?
    int intensity;	// Intensity (0..255);
    int sector;
    UINT32 lastUpdateTime;	// Time last update occurred. 
    /* Position relative to radars. */
    double angleDegsFromRadar[SPX_SCENGEN_MAX_NUM_RIBS];
    double rangeMetresFromRadar[SPX_SCENGEN_MAX_NUM_RIBS];
    double sinAngleRadsHeadToRadar[SPX_SCENGEN_MAX_NUM_RIBS];
    double cosAngleRadsHeadToRadar[SPX_SCENGEN_MAX_NUM_RIBS];
    SPxSGpath *path;	// The SG path (optional) 
    SPxSGpathPoint *nextPathPoint;
    int waiting;	// Are we waiting?
    SPxTime_t waitUntil;    // How long to wait.
    UINT32 lastReceiptTime; // Time last receipt occurred.
    UINT32 lastDrawTime;    // Last time part of target was drawn
} SPxSGtarget;

/* Define a structure for per-radar information. */
typedef struct
{
    /* Number of returns we've output for this since m_baseUpdateTime. */
    unsigned int numSpokesDone;

    /* Handles. */
    SPxRIB *rib;
    SPxRadar *radar;

    UINT32 m_lastUpdateTime;	/* Time last update occurred. */
    double m_speed;		/* The radar's speed in m/s */
    double m_courseDegrees;	/* The course in degrees */
    double m_latitude;		/* Radar's lat/long. */
    double m_longitude;

    /* Incrementing count for RIB. */
    UINT16 incrementingCount;

    /* Last azimuth output. */
    unsigned int lastAzimuth;

    /* North up (value = 1) or ship relative (value = 0) ? */
    int northup;

    /* A Navigation Object - if one exists */
    SPxNavData *navData;

    /* Number of revs of this radar */
    UINT32 numRevs;

} SPxSGradar;

/* A class that defines a point on a path */
class SPxSGpathPoint
{
friend class SPxSGpath;
    double m_x;			// X position in metres.
    double m_y;			// Y position in metres.
    double m_speedWeight;	// A speed modulator to change the target speed.
    double m_intensityWeight;	// An intensity multiplier for target level.
    SPxSGpathPoint *m_next;
    double m_waitFor;		// How long to wait when we get to this point.
public:
    SPxSGpathPoint(double x, double y, double speedWeight = 1.0, double holdTime = 0.0,
					double intensityWeight = 1.0);
    SPxSGpathPoint *GetNext(void) {return m_next;}
    double GetX(void) {return m_x;}
    double GetY(void) {return m_y;}
    double GetWaitForSecs(void) {return m_waitFor;}
    double GetSpeedWeight(void) {return m_speedWeight;}
    double GetIntensityWeight(void) {return m_intensityWeight;}
};


/* Mode for path - what happens when target reaches end of path? */
enum
{
    SPX_SG_PATH_MODE_LOOP = 0,
    SPX_SG_PATH_MODE_STOP_AND_HOLD = 1,
    SPX_SG_PATH_MODE_STOP_AND_CLEAR = 2
};

/* A class that defines a path for targets to move around in the scenario
 * generator.
 */
class SPxSGpath
{
    int m_numPoints;	/* How many point in the path */
    int m_id;		/* The user-assigned if for this path */
    int m_mode;		/* 0 = loop, 1 = stop at end, 2 = clear at end */
    SPxSGpathPoint *m_firstPoint;   /* The first point in the path */
    SPxSGpathPoint *m_lastPoint;    /* The last point in the path */
    static SPxSGpath* m_firstPath;  /* The first STATIC path */
    static SPxSGpath* m_lastPath;   /* The last STATIC path */
    SPxSGpath *m_next;		    /* The next path */

public:
    SPxSGpath(int id = 0, int mode = 0);
    virtual ~SPxSGpath(void);
    SPxSGpath* GetFirstPath(void) {return m_firstPath;}
    SPxSGpath* GetNext(void) {return m_next;}
    double GetHeadingFrom(SPxSGpathPoint *point);
    double GetHeadingTo(SPxSGpathPoint *point, double thisX, double thisY);
    SPxCriticalSection m_mutex;    
    SPxSGpathPoint *AddPoint(double x, double y, double speedWeight = 1.0, 
	double holdTime = 0.0, double intensityWeight = 1.0);
    void RemovePoint(SPxSGpathPoint*);
    void RemovePoint(int pointIndex);
    static SPxSGpath* FindPathFromID(int id);
    int GetNumPoints(void) {return m_numPoints;}
    SPxSGpathPoint *GetFirstPoint(void);
    int GetMode(void) {return m_mode;}
};

/*
 * Callback
 */
typedef void (*SPxSGazimuthFn_t)(SPxScenarioGenerator *scenGen,
				 SPxSGradar *radar,
				 int azimuth,
				 void *userArg);

/* A class that defines the information associated with an azimuth callback function. */
class SPxSGazimuthFnInfo
{
    SPxSGazimuthFn_t m_fn;  /* Callback function. */
    SPxSGradar *m_radar;    /* Radar associated with callback. */
    int m_azimuth;	    /* Azimuth number after which to call callback. */
    void *m_userArg;	    /* User argument to pass to callback. */
    int m_calledThisRev;    /* Has the callback been called this revolution? */

public:
    SPxSGazimuthFnInfo(void)
    {
	m_fn = NULL;
	m_radar = NULL;
	m_azimuth = 0;
	m_userArg = NULL;
	m_calledThisRev = FALSE;
    }
    SPxSGazimuthFnInfo(SPxSGazimuthFn_t fn, SPxSGradar *radar, 
		       int azimuth, void *userArg)
    {
	m_fn = fn;
	m_radar = radar;
	m_azimuth = azimuth;
	m_userArg = userArg;
	m_calledThisRev = FALSE;
    }
    virtual ~SPxSGazimuthFnInfo(void){}
    SPxSGazimuthFn_t GetFn(void) { return m_fn; }
    SPxSGradar *GetRadar(void) { return m_radar; }
    int GetAzimuth(void) { return m_azimuth; }
    void *GetUserArg(void) { return m_userArg; }
    int GetCalledThisRev(void) { return m_calledThisRev; }
    void SetCalledThisRev(int called) { m_calledThisRev = called; }
};

/* Callback for target addition/removal. */
typedef void (*SPxSGtargetFn_t)(SPxScenarioGenerator *obj, 
				int targetId,
				SPxSGtargetState state,
				void *userArg);

/*
 * Define our scenario generator class, derived from a generic radar source.
 */
class SPxScenarioGenerator :public SPxRadarSource
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxScenarioGenerator(SPxRadarDatabase *rdb);
    virtual ~SPxScenarioGenerator(void);

    /* Functions for connecting the source to a RIB. */
    SPxErrorCode Connect(SPxRIB *, const char *radarName);
    SPxErrorCode Connect(SPxRIB *, int radarID);

    /* Number of samples per return. */
    SPxErrorCode SetNumSamples(unsigned int numSamples);
    unsigned int GetNumSamples(void);

    /* Functions for distributing targets. */
    SPxErrorCode EnableDistribution(int enable=TRUE);
    int IsDistributionEnabled(void);
    SPxErrorCode SetDistAddress(const char *addr, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetDistAddress(UINT32 addr, int port=0, const char *ifAddr=NULL);
    UINT32 GetDistAddress(void);
    const char *GetDistAddressString(void);
    int GetDistPort(void);
    UINT32 GetDistIfAddress(void);
    const char *GetDistIfAddressString(void);
    SPxErrorCode SetSendInterval(UINT32 intervalMsecs);
    UINT32 GetSendInterval(void);
    SPxErrorCode SetTTL(int ttl);
    int GetTTL(void) { return m_ttl; }

    /* Functions for receiving targets. */
    SPxErrorCode EnableReceipt(int enable=TRUE);
    int IsReceiptEnabled(void);
    SPxErrorCode SetRecvAddress(const char *addr, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetRecvAddress(UINT32 addr, int port=0, const char *ifAddr=NULL);
    UINT32 GetRecvAddress(void);
    const char *GetRecvAddressString(void);
    int GetRecvPort(void);
    UINT32 GetRecvIfAddress(void);
    const char *GetRecvIfAddressString(void);
    SPxErrorCode SetPurgeInterval(UINT32 intervalMsecs);
    UINT32 GetPurgeInterval(void);

    /* Functions for manipulating targets. */
    SPxErrorCode AddTarget(int targetId, double x, double y, double sp, double hdg, int *idRtn=NULL);
    SPxErrorCode RemoveTarget(int targetId);
    int IsTargetId(int id);
    SPxErrorCode SetTargetChangeFn(SPxSGtargetFn_t fn, void *userArg);
    SPxErrorCode SetTargetPosition(int targetId, double x , double y);
    SPxErrorCode GetTargetPosition(int targetId, double *x, double *y);
    double GetTargetRange(int targetId, int radarId);
    double GetTargetAzimuth(int targetId, int radarId);
    SPxErrorCode SetTargetSpeed(int targetId, double speed);
    double GetTargetSpeed(int targetId);
    SPxErrorCode SetTargetHeading(int targetId, double heading);
    double GetTargetHeading(int targetId);
    SPxErrorCode SetTargetActive(int targetId, int active);
    SPxErrorCode SetTargetLevel(int targetId, int level);
    SPxErrorCode SetTargetSize(int targetId, double breadth, double length);
    SPxErrorCode GetTargetSize(int targetId, double *breadth, double *length);
    SPxErrorCode SetTargetPath(int targetId, SPxSGpath *path);
    int GetNumTargets(void);
    void EnableTargetIdErrors(int enable=TRUE) { m_outputTargetIdErrors = enable; }

    SPxErrorCode SetTargetRangeAlignment(int alignment);
    int GetTargetRangeAlignment(void) {return m_targetRangeAlignment;}
    void SetTargetRangeDimension(int rangeSamples);
    void SetTargetRangeSize(int sizeInSamples);
    int GetTargetRangeDimension(void) {return m_targetRangeDimSamples;}

    /* Delete all targets */
    void DeleteAllTargets(void);

    /* Read/write from/to file */
    SPxErrorCode ReadTargets(const char *fileName);
    SPxErrorCode ReadScenarioFromFile(const char *fileName);
    SPxErrorCode WriteScenarioToFile(const char *fileName);

    /* Retrieve file name. */
    const char *GetFullFilePath(void) { return m_fileName.GetFullPath(); }
    const char *GetShortFilePath(void) { return m_fileName.GetShortPath(); }
    
    /* Noise control. */
    void EnableNearNoise(int enable);
    int IsNearNoiseEnabled(void) const;
    void SetNearNoiseGain0(double gain);
    double GetNearNoiseGain0(void) const;
    void SetNearNoiseRange(int range);
    int GetNearNoiseRange(void) const;
    void SetBackgroundNoise(int noisePercent);
    int GetBackgroundNoise(void) const;

    /* Target reports. */
    void EnableTargetReports(int enable=TRUE) { m_targetReportsEnabled = enable; }
    int AreTargetReportsEnabled(void) { return m_targetReportsEnabled; }

    /* Free-text fields. */
    SPxErrorCode SetTextField(const char *name, const char *value);
    SPxErrorCode GetTextField(const char *name, char *valueBuf, int bufLen) const;

    /* Clutter areas. */
    SPxErrorCode SetClutterArea(const char *name, 
	double startXMetres, double endXMetres, 
	double startYMetres, double endYMetres,
	unsigned int plotCount);
    SPxErrorCode GetClutterArea(const char *name,
	double *startXMetres, double *endXMetres, 
	double *startYMetres, double *endYMetres,
	unsigned int *plotCount) const;
    SPxErrorCode RemoveClutterArea(const char *name);
    SPxErrorCode RemoveAllClutterAreas(void);
    SPxErrorCode SetClutterIntensity(UINT8 intensity);
    UINT8 GetClutterIntensity(void) const;

    /* Other functions. */
    /* Expose base class Set/GetParameter to allow for testing. */
    using SPxObj::SetParameter;
    using SPxObj::GetParameter;
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);
    SPxRadarDatabase * GetRadarDatabase(void) {return m_rdb;}
    void UpdateLatLong(SPxSGradar *radarInfo, UINT32 updateTimeMS);

    void SetWorld(double x, double y, double w, double h)
    {
	m_worldX = x;
	m_worldY = y;
	m_worldW = w;
	m_worldH = h;
    }

     /* Moving the radar */
    int SetRadarMotion(const char *radarName, double speed, double heading);
    int SetRadarMotion(int radarId, double speed, double heading);
    int SetAllRadarsMotion(double speed, double heading);

    int SetRadar(const char *radarName,
			double x, double y,
			double speed, double heading);
    int SetRadar(int radarID,
			double x, double y,
			double speed, double heading);

    /* Set the radar lat/long. This isn't used for target display
     * calculations, but it is updated by the programmed speed and
     * course and output through navigation message. The lat/long is
     * only significant is SetNavData() has been used to associated
     * a navigation object.
     */
    int SetRadarLatLong(int radarID, double latitude, double longitude);
    int SetRadarLatLong(const char *radarName, double latitude, double longitude);

    /* Set the North Up or Ship Heading Up Modes */
    int SetNorthUp(int radar, int state);
    int SetNorthUp(const char *radar, int state);

    /* Set radar period */
    int SetRadarPeriod(int radarID, double period);
    int SetRadarPeriod(const char *radar, double period);

    /* Set radar PRF */
    int SetRadarPrf(int radarID, int prf);
    int SetRadarPrf(const char *radar, int prf);

    /* Set radar beamwidth. */
    int SetRadarBeamwidth(int radarID, double beamwidth);
    int SetRadarBeamwidth(const char *radar, double beamwidth);

    /* Associate a Nav data object with a Radar */
    int SetNavData(const char *radarName, SPxNavData *navData);
    int SetNavData(int radarId, SPxNavData *navData);
    
    /* Install callbacks when radar passes specified azimuth */
    SPxErrorCode SetAzimuthFn(int radarId, SPxSGazimuthFn_t fn, 
			      int azimuth, void *userArg);
    SPxErrorCode SetAzimuthFn(const char *radar, SPxSGazimuthFn_t fn, 
			      int azimuth, void *userArg);
    SPxErrorCode RemoveRadarAzimuthFns(int radarId);
    SPxErrorCode RemoveRadarAzimuthFns(const char *radar);

    /* Get number of revs for a radar */
    UINT32 GetRadarNumRevs(int radarId);

    /* Fractional positioning should normally be enabled for higher
     * accuracy, but may be disabled for certain test programs to clarify
     * the scenario.  It is enabled by default.
     */
    void SetUseFractionalPosition(int enable) {m_useFractionalPosition = enable;}
    int GetUseFractionalPosition(void) 	{return(m_useFractionalPosition);}

    /* Thread function. */
    virtual void *ThreadFunc(SPxThread *thread);

protected:
    SPxScenarioGenerator(SPxRadarDatabase *rdb, int startThread);
    void StartThread(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

    /* Internal function. */
    SPxErrorCode OutputReturns(UINT32 elapsedTime);

private:
    /*
     * Private fields.
     */
    struct impl;
    SPxAutoPtr<impl> m_p; /* Private structure. */

    /* Last file loaded. */
    SPxPath m_fileName;

    int m_outputTargetIdErrors;
    int m_lastAddTargetId;	/* The last ID successfully used by AddTarget(). */
    
    /* Lists of radars/RIBs we're feeding, and associated info. */
    SPxSGradar m_radarInfo[SPX_SCENGEN_MAX_NUM_RIBS];
    int m_numRadars;
    SPxRadarDatabase *m_rdb;

    /* Timing and update information. */
    mutable SPxCriticalSection m_mutex;	/* Protection */
    UINT32 m_baseUpdateTime;	/* When our timing is calculated from */

    /* Update the radar position (moving platform) */
    void UpdateRadar(SPxSGradar *radar);

    /* Control parameters. */
    int m_addNearNoise;		/* TRUE if near-field nosie is to be added */
    double m_nearGain;		/* Gain on noise at range 0. */
    int m_nearNoiseRangeLimit;	/* Extent of near-range noise.
				 * (gain is 1.0 at this range).
				 */
    int m_noisePercent;		/* Additional background noise (0..100) */
    int m_targetRangeDimSamples;/* Num samples used to draw the target */
    int m_samplesPerReturn;	/* Number of samples per return */
    double m_updatePeriod;	/* Update period */
    double m_worldX, m_worldY, m_worldW, m_worldH;
    int m_targetRangeAlignment; /* Alignment in range */
    int m_useFractionalPosition;/* Draw targets with higher accuracy */

    /* Return header, and space for data. */
    SPxReturnHeader m_returnHeader;
    unsigned char *m_returnData;

    /* Target distribution. */
    int m_targetDistEnabled;	    /* Is target distribution enabled? */
    SPxPacketSender *m_targetSender; /* Object used to send target info. */
    UINT32 m_lastTargetSendTime;    /* Last time target info was sent. */
    UINT32 m_targetSendInterval;    /* Target send interval in milliseconds. */

    /* Target receipt. */
    int m_targetReceiptEnabled;	    /* Is target receipt enabled? */
    SPxPacketDecoderNet *m_targetRecv; /* Object used to receive target info. */
    UINT32 m_purgeTargetAgeMsecs;   /* Age of targets before purging. */
    UINT32 m_lastPurgeTime;	    /* Last time purge was performed. */

    /* Time to live. */
    int m_ttl;

    /* Target callback. */
    SPxSGtargetFn_t m_targetFn;	    /* Function called when target added/removed. */
    void *m_targetUserArg;	    /* User argument for target function. */

    /* Target reports. */
    int m_targetReportsEnabled;	    /* Are target reports enabled? */

    /* Worker thread object */
    SPxThread *m_thread;

    /* The last sector that we updated for the first radar */
    int m_lastSectorUpdated;

    /*
     * Private functions.
     */
    void initialise(SPxRadarDatabase *rdb, int startThread);
    void resetTiming(void);
    int addRadar(SPxRIB *buffer, SPxRadar *radar);
    void fillReturnHeader(void);
    void resetAzimuthFns(void);
    void callAzimuthFns(SPxSGradar *radar, UINT16 azimuth);

    /* Update, draw and output targets. */
    void updateTargets(int sectorNumber);
    void updateClutterAreas(int sectorNumber);
    void calcPosRelativeToRadar(const SPxSGradar &radarInfo,
	double worldX, double worldY, double *angle, double *range);
    void updateClutterAreaPlots(SPxScenGenClutterArea &area);
    void calcRadarVals(SPxSGtarget &target);
    void buildDataForReturn(unsigned int radarIdx, unsigned int aziIdx);
    void resetTargets(void);

    /* Send target information. */
    SPxErrorCode sendTargetInfo(void);
    SPxErrorCode sendTargetReport(unsigned int radarIdx,
	const SPxSGtarget &target, int status);

    /* Target receipt functions. */
    static void targetRecvFnWrapper(SPxPacketDecoder *decoder,
				    void *userArg,
				    UINT32 packetType,
				    struct sockaddr_in *from,
				    SPxTime_t *time,
				    const unsigned char *payload,
				    unsigned int numBytes);
    void targetRecvFn(SPxPacketDecoder *decoder,
		      void *userArg,
		      UINT32 packetType,
		      struct sockaddr_in *from,
		      SPxTime_t *time,
		      const unsigned char *payload,
		      unsigned int numBytes);
    void purgeTargets(void);

};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SCENGEN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
