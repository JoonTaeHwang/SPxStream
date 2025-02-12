/*********************************************************************
*
* (c) Copyright 2014 - 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSafetyNetEngine.h,v $
* ID: $Id: SPxSafetyNetEngine.h,v 1.9 2015/01/13 11:49:38 rew Exp $
*
* Purpose:
*	Header for SPxSafetyNetEngine, the main class providing
*	safety net functionality.
*
*
* Revision Control:
*   13/01/15 v1.9    REW	Support stcaRequireAltitude, stcaFilenameExclude
*				and apwFilenameExclude options.
*
* Previous Changes:
*   24/11/14 1.8    REW	Add alertMajor flag.
*   22/10/14 1.7    REW	Avoid need for map header file.
*   16/10/14 1.6    REW	Support MSAW exclusion region.
*   15/10/14 1.5    REW	Implement MSAW params.
*   02/10/14 1.4    REW	Implement APW params.
*   25/09/14 1.3    REW	Implement STCA params.
*   24/09/14 1.2    REW	Implement alert structure.
*   15/09/14 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SAFETY_NET_ENGINE_H
#define _SPX_SAFETY_NET_ENGINE_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibUtils/SPxSharedPtr.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Debug flags.
 */
#define	SPX_SAFETY_NET_DEBUG_SETUP		0x00000001
#define	SPX_SAFETY_NET_DEBUG_TRACK		0x00000002
#define	SPX_SAFETY_NET_DEBUG_ALERT		0x00000004
#define	SPX_SAFETY_NET_DEBUG_VERBOSE		0x80000000

/*
 * Alert options.
 */
#define	SPX_SAFETY_NET_ALERT_STCA		0x00000001
#define	SPX_SAFETY_NET_ALERT_MSAW		0x00000002
#define	SPX_SAFETY_NET_ALERT_APW		0x00000004

/*
 * Status flags.
 */
#define	SPX_SAFETY_NET_STATUS_DEGRADED		0x00000001 /* Input lost */
#define	SPX_SAFETY_NET_STATUS_OVERFLOW		0x00000002 /* Dropped alerts */
#define	SPX_SAFETY_NET_STATUS_OVERLOAD		0x00000004 /* Unreliable */


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

/* Declare the structure we use for reporting alerts etc. through the API
 * (not on the network - Asterix CAT-4 is used for that).
 */
typedef struct SPxSafetyNetAlert_tag {
    /* Timestamp (different to the Cat4 field). */
    UINT32 alertMsecs;		/* Time that the alert was made/received */

    /* Top level values */
    UINT16 alertID;		/* Alert identifier, or 0 for ALIVE messages */
    UINT8 alertType;		/* SPxAsterix4MsgType */
    UINT8 reserved;

    /* Alert status fields. */
    UINT8 alertMajor;		/* Major alert? */
    UINT8 alertPad2;		/* For alignment */
    UINT8 alertPad3;		/* For alignment */
    UINT8 alertPad4;		/* For alignment */

    /* System status fields. */
    UINT32 statusFlags;		/* Mask of SPX_SAFETY_NET_STATUS_... bits */
    UINT32 alertMask;		/* Mask of SPX_SAFETY_NET_ALERT_... bits */

    /* Time information. */
    UINT32 alertAgeMsecs;	/* Age of alert (duration) */
    UINT32 timeToConflictMsecs;	/* Time remaining to conflict */
    UINT32 timeToCPAMsecs;	/* Time remaining to CPA */
    UINT8 timeToConflictSet;	/* Is the time set? */
    UINT8 timeToCPASet;		/* Is the time set? */
    UINT16 timeFlagsPad;	/* Padding */

    /*
     * Track 1 fields (if relevant).
     */
    /* ID etc. */
    UINT16 track1ID;		/* Track number */
    UINT16 track1Mode3A;	/* Mode 3/A code, if flag set below */

    /* Name. */
    char track1Name[9];		/* Track name, 8 chars + NULL terminated */
    UINT8 track1PadA;
    UINT16 track1PadB;

    /* Position (may be predicted, current etc. depending on alert). */
    REAL32 track1LatDegs;	/* Latitude in degrees, if flag set below */
    REAL32 track1LongDegs;	/* Longitude in degrees, if flag set below */
    REAL32 track1AltMetres;	/* Altitude in metres, if flag set below */

    /* Flags. */
    UINT8 track1Mode3ASet;	/* Flag */
    UINT8 track1LatLongSet;	/* Flag */
    UINT8 track1AltSet;		/* Flag */
    UINT8 track1ReservedFlag;	/* Flag */

    /*
     * Track 2 fields (if relevant).
     */
    /* ID etc. */
    UINT16 track2ID;		/* Track number */
    UINT16 track2Mode3A;	/* Mode 3/A code, if flag set below */

    /* Name. */
    char track2Name[9];		/* Track name, 8 chars + NULL terminated */
    UINT8 track2PadA;
    UINT16 track2PadB;

    /* Position (may be predicted, current etc. depending on alert). */
    REAL32 track2LatDegs;	/* Latitude in degrees, if flag set below */
    REAL32 track2LongDegs;	/* Longitude in degrees, if flag set below */
    REAL32 track2AltMetres;	/* Altitude in metres, if flag set below */

    /* Flags. */
    UINT8 track2Mode3ASet;	/* Flag */
    UINT8 track2LatLongSet;	/* Flag */
    UINT8 track2AltSet;		/* Flag */
    UINT8 track2ReservedFlag;	/* Flag */

    /*
     * Separation information (if relevant).
     */
    REAL32 sepHorizCurMetres;	/* Current horiz separation */
    REAL32 sepHorizMinMetres;	/* Estimated min horiz separation */
    REAL32 sepVertCurMetres;	/* Current vert separation */
    REAL32 sepVertMinMetres;	/* Estimated min vert separation */
    UINT8 sepHorizCurSet;	/* Is the value set? */
    UINT8 sepHorizMinSet;	/* Is the value set? */
    UINT8 sepVertCurSet;	/* Is the value set? */
    UINT8 sepVertMinSet;	/* Is the value set? */

    /*
     * Area infomation (if relevant).
     */
    char areaName[9];		/* Area name, 8 chars + NULL terminated */
    UINT8 areaPadA;
    UINT16 areaPadB;

} SPxSafetyNetAlert_t;


/*
 * Forward-declare other classes we use in case headers are in wrong order.
 */
class SPxActiveRegion;
class SPxCriticalSection;
class SPxRadarTrack;
class SPxTimer;
class SPxTrackDatabase;
class SPxTerrainDatabase;

/*
 * Define our class, derived from generic SPx base class.
 */
class SPxSafetyNetEngine :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxSafetyNetEngine(SPxTrackDatabase *trackDatabase,
			SPxTerrainDatabase *terrainDatabase);
    virtual ~SPxSafetyNetEngine();

    /* Input of observations. */
    SPxErrorCode ProcessNewTrack(SPxRadarTrack *track);

    /* Alert reporting. */
    SPxErrorCode AddAlertCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveAlertCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Options. */
    void SetAlertMask(UINT32 alertMask)	{ m_alertMask = alertMask; }
    UINT32 GetAlertMask(void)		{ return(m_alertMask); }
    void SetPurgeSecs(UINT32 secs)	{ m_purgeSecs = secs; }
    UINT32 GetPurgeSecs(void)		{ return(m_purgeSecs); }

    /* STCA control. */
    SPxErrorCode SetSTCASeconds(unsigned int seconds);
    unsigned int GetSTCASeconds(void) const	{ return(m_stcaSeconds); }
    SPxErrorCode SetSTCADlMajorMetres(double metres);
    double GetSTCADlMajorMetres(void) const	{ return(m_stcaDlMajorMetres);}
    SPxErrorCode SetSTCADlMinorMetres(double metres);
    double GetSTCADlMinorMetres(void) const	{ return(m_stcaDlMinorMetres);}
    SPxErrorCode SetSTCADvMetres(double metres);
    double GetSTCADvMetres(void) const		{ return(m_stcaDvMetres); }
    SPxErrorCode SetSTCARequireAltitude(int requireAltitude);
    int GetSTCARequireAltitude(void) const { return(m_stcaRequireAltitude); }
    SPxErrorCode SetSTCAFilenameExclude(const char *filename);
    const char *GetSTCAFilenameExclude(void) const
    {
	return(m_stcaFilenameExclude);	/* May be NULL */
    }

    /* APW control. */
    SPxErrorCode SetAPWSeconds(unsigned int seconds);
    unsigned int GetAPWSeconds(void) const	{ return(m_apwSeconds); }
    SPxErrorCode SetAPWFilename(const char *filename);
    const char *GetAPWFilename(void) const	{ return(m_apwFilename); }
    SPxErrorCode SetAPWFilenameExclude(const char *filename);
    const char *GetAPWFilenameExclude(void) const
    {
	return(m_apwFilenameExclude);	/* May be NULL */
    }

    /* MSAW control. */
    SPxErrorCode SetMSAWSeconds(unsigned int seconds);
    unsigned int GetMSAWSeconds(void) const	{ return(m_msawSeconds); }
    SPxErrorCode SetMSAWMarginMetres(double metres);
    double GetMSAWMarginMetres(void) const	{ return(m_msawMarginMetres); }
    SPxErrorCode SetMSAWFilenameExclude(const char *filename);
    const char *GetMSAWFilenameExclude(void) const
    {
	return(m_msawFilenameExclude);	/* May be NULL */
    }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Debug. */
    void SetDebug(UINT32 flags)		{ m_debug = flags; }
    UINT32 GetDebug(void)		{ return(m_debug); }
    void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    FILE *GetLogFile(void)		{ return(m_logFile); }

private:
    /*
     * Private fields.
     */
    /* Critical section protection. */
    SPxCriticalSection *m_mutex;	/* Protect updates etc. */

    /* Timer. */
    SPxTimer *m_timer;			/* Timer object */

    /* Debug. */
    FILE *m_logFile;			/* Destination for debug messages */
    UINT32 m_debug;			/* Debug flags */

    /* Target database. */
    SPxTrackDatabase * const m_database;	/* Database of targets */
    UINT32 m_lastDatabaseStatusMsecs;	/* Time of last status message */
    REAL32 m_lastDatabaseLatDegs;	/* Last known latitude */
    REAL32 m_lastDatabaseLongDegs;	/* Last known longitude */

    /* Terrain database (needed for MSAW alerts). */
    SPxTerrainDatabase * const m_terrainDatabase;	/* Terrain data */

    /* Status. */
    UINT32 m_statusFlags;		/* Status of our processing */

    /* Options. */
    UINT32 m_alertMask;			/* SPX_SAFETY_NET_ALERT_... */
    UINT32 m_purgeSecs;			/* Seconds (do not set to 0) */

    /* Alert reporting. */
    SPxCallbackList *m_alertCallbacks;	/* List of callbacks */

    /* Track under test. */
    SPxRadarTrack *m_trackUnderTest;	/* What track are we testing? */

    /*
     * Parameters for alerts.
     */
    /* STCA. */
    unsigned int m_stcaSeconds;		/* Time to look ahead (may be 0) */
    double m_stcaDlMinorMetres;		/* Minor lateral distance */
    double m_stcaDlMajorMetres;		/* Major lateral distance */
    double m_stcaDvMetres;		/* Vertical distance */
    int m_stcaRequireAltitude;		/* Need altitude to declare alert? */
    char *m_stcaFilenameExclude;	/* May be NULL */
    SPxActiveRegion *m_stcaRegionExclude; /* Handle of exclusion region */

    /* APW. */
    unsigned int m_apwSeconds;		/* Time to look ahead (may be 0) */
    char *m_apwFilename;		/* May be NULL */
    SPxActiveRegion *m_apwRegion;	/* Region handle */
    char *m_apwFilenameExclude;		/* May be NULL */
    SPxActiveRegion *m_apwRegionExclude; /* Handle of exclusion region */

    /* MSAW. */
    unsigned int m_msawSeconds;		/* Time to look ahead (may be 0) */
    double m_msawMarginMetres;		/* Vertical margin in metres */
    char *m_msawFilenameExclude;	/* May be NULL */
    SPxActiveRegion *m_msawRegionExclude; /* Handle of exclusion region */

    /*
     * Storage for alerts.
     */
    SPxSafetyNetAlert_t *m_alerts;	/* Pointer to list of alerts */
    UINT16 m_nextAlertID;		/* Next ID to use */

    /*
     * Private functions.
     */
    /*
     * Alert checkers.
     */
    /* STCA. */
    SPxErrorCode checkAlertSTCA(SPxRadarTrack *track);
    static int staticDbIterateSTCA(void *invokingObj, void *userArg, void *arg);
    SPxErrorCode dbIterateSTCA(SPxRadarTrack *track);

    /* APW. */
    SPxErrorCode checkAlertAPW(SPxRadarTrack *track);

    /* MSAW. */
    SPxErrorCode checkAlertMSAW(SPxRadarTrack *track);

    /* Functions to install as callbacks. */
    static int dbTrackUpdateHandler(void *invokingObj, void *userArg,
						void *arg);
    static void timerStatusWrapper(void *userArg);
    void timerStatus(void);

    /* Alert maintenance. */
    SPxSafetyNetAlert_t *getNewAlert(void);
    SPxSafetyNetAlert_t *findAlert(UINT8 alertType,
					UINT16 trackID1, UINT16 trackID2);

}; /* SPxSafetyNetEngine class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SAFETY_NET_ENGINE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
