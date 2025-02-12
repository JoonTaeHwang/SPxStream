/*********************************************************************
*
* (c) Copyright 2008, 2010, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxTracker.h,v $
* ID: $Id: SPxTracker.h,v 1.6 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*	Header for the tracker class.
*
* Revision Control:
*   04/10/13 v1.6    AGC	Simplify headers.
*
* Previous Changes:
*   15/09/10 1.5    REW	Make destructor virtual.
*   10/01/08 1.4    REW	Add utility functions and fields.
*   08/01/08 1.3    REW	Add internalObj to public track report.
*   07/01/08 1.2    REW	Support reporting callback installation.
*   04/01/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_TRACKER_H
#define _SPX_TRACKER_H

/*
 * Other headers required.
 */
/* For SPxCallbackListFn_t. */
#include "SPxLibUtils/SPxCallbackList.h"

/* For SPxTime_t. */
#include "SPxLibUtils/SPxTime.h"

/* For base class. */
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
class SPxTargetDB;
class SPxRunProcess;
struct SPxReturn_tag;

/*
 * Public type for track information (not the same as that used internally).
 */
typedef struct SPxTrack_tag {
    /* IDs and status. */
    UINT32 id;			/* Short-form ID, 1 to N. */
    UINT32 id32;		/* Long-form ID, wraps at 0xFFFFFFFF. */
    UINT32 active;		/* Is this report active? */

    /* Coasting information. */
    UINT32 coastingCount;	/* Number of revs track been coasted for */

    /* Position. */
    REAL32 rangeMetres;
    REAL32 azimuthDegrees;
    REAL32 xMetres;
    REAL32 yMetres;

    /* Velocity. */
    REAL32 speedMps;
    REAL32 courseDegrees;

    /* Measured position. */
    REAL32 measXMetres;
    REAL32 measYMetres;
    UINT32 numBlobs;		/* Number of components forming measX/Y */

    /* Time information. */
    SPxTime_t timestamp;

    /* Internal entries. */
    struct SPxTrack_tag *next;			/* Free list, if inactive */
    struct SPxTrackInternal_tag *internalObj;	/* Internal track */
} SPxTrack_t;


/*
 * Tracker class.
 */
class SPxTracker :public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxTracker(SPxTargetDB *blobDB);
    virtual ~SPxTracker();

    /* Function to tell the tracker about a new radar return. */
    int NewReturn(SPxRunProcess *rp, SPxReturn_tag *rtn);

    /* Functions to retrieve or iterate through active track reports. */
    const SPxTrack_t *GetNextTrack(const SPxTrack_t *track);
    const SPxTrack_t *FindNearestTrack(double xMetres, double yMetres);

    /* Functions to install/remove track report callback routines. */
    int AddReportCallback(SPxCallbackListFn_t fn, void *userObject)
    {
	return( m_reportCallbacks->AddCallback(fn, userObject) );
    }
    int RemoveReportCallback(SPxCallbackListFn_t fn, void *userObject)
    {
	return( m_reportCallbacks->RemoveCallback(fn, userObject) );
    }

    /* Pre-supplied reporting routines that may be installed with
     * the AddReportCallback() function.
     */
    static int ReportViaVisualServer(void *invokingObject,
					void *userObject,
					void *arg);

private:
    /*
     * Private functions.
     */
    int processSector(unsigned int sector);	/* Process a sector */
    int associateTrackWithBlobs(struct SPxTrackInternal_tag *track);
    int updateTrack(struct SPxTrackInternal_tag *track);
    int predictTrack(struct SPxTrackInternal_tag *track);
    int performATI(unsigned int sector);	/* Perform ATI for a sector */

    /* Utility functions. */
    REAL32 firstAngleDegs(REAL32 az1, REAL32 az2);
    REAL32 lastAngleDegs(REAL32 az1, REAL32 az2);


    /*
     * Private variables.
     */
    /* Debug */
    UINT32 m_debug;

    /* Parent/control objects. */
    SPxRunProcess *m_rp;		/* RunProcess feeding us data */

    /* Sector information etc. */
    unsigned int m_numSectors;		/* Number of sectors in DB. */
    unsigned int m_numSectorsOver8;	/* Convenience value */
    unsigned int m_currentSector;	/* Which sector we're currently in */
    unsigned int m_azimuthsPerSector;	/* Number of 64k azis per sector */
    double m_radiansPerSector;		/* Radians per sector */
    double m_lastUpdatePeriodSecs;	/* Last update period seen */

    /* Counts etc. */
    unsigned int m_currentRevIdx;	/* What rev number we're on */
    unsigned int m_numTracksSoFar;	/* Total number of tracks */

    /* Databases. */
    SPxTargetDB *m_blobDB;		/* Source of our blobs */
    SPxTargetDB *m_trackDB;		/* Current tracks */

    /* Public track report structures etc. */
    SPxTrack_t *m_trackArray;		/* Array of public reports */
    SPxTrack_t *m_firstFreeTrack;	/* Head of linked list */

    /* List of callback functions for reporting tracks. */
    SPxCallbackList *m_reportCallbacks;	/* Object for handling callbacks */
}; /* SPxTracker object */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_TRACKER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
