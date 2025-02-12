/*********************************************************************
*
* (c) Copyright 2007 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for area selection and remapping
*
* Revision Control:
*   07/12/21 v1.62   BTB	Allow management of radar lat/long on SPxActiveRegionDB.
*
* Previous Changes:
*   03/12/21 1.61   BTB	Signal shape and region mode changes properly.
*   03/12/21 1.60   BTB	Support remote management of SPxActiveRegionDB.
*   29/11/21 1.59   BTB	Support fetching parent areas for shapes.
*   15/11/21 1.58   BTB	Signal missing area events.
*   01/09/21 1.57   AGC	Support area events.
*			Implement SPxAreaObj::AppendPoint().
*   10/02/21 1.56   AGC	Implement SPxAreaObj::IsItemNameSet().
*   08/10/20 1.55   SP 	Implement SPxAreaObj::GetAreaType().
*   04/02/19 1.54   DGJ	Support control of heading ref for polygon.
*   16/08/18 1.53   SP 	Add IsTimedEnableInProgress().
*   15/08/18 1.52   SP 	Rename a variable.
*   02/08/18 1.51   SP 	Add SPxActiveRegion::SetDelayedEnable().
*   27/07/18 1.50   SP 	Support area timed enable/disable.
*   16/07/18 1.49   SP 	Add support for area alarm area controls.
*   25/06/18 1.48   AGC	Correctly suppress cppcheck warning.
*   03/05/18 1.47   AGC	Support per-radar enabled shapes.
*   20/04/17 1.46   SP 	Support user flags.
*   12/04/17 1.45   AGC	Improve mutex protection.
*   24/02/17 1.44   SP 	Fix GetRegionFromPoint() when poly is lat/long.
*   28/07/16 1.43   AGC	Fix #398 - area points close to azimuth edges.
*   14/07/16 1.42   DGJ	Add nav data for heading adjustment
*   22/10/15 1.41   AGC	Support restricting polygon creation from SPxActiveRegion.
*   14/04/15 1.40   AGC	Preserve comments in region files.
*   02/10/14 1.39   REW	Add DoesLineSegmentHitRegion().
*   21/05/14 1.38   REW	Add lockListOnSuccess option to GetByName().
*			Add LockList() and UnLockList() options.
*   17/02/14 1.37   SP 	Move weighting from poly to base class.
*   13/12/13 1.36   SP 	Add weighting value to SPxActiveRegion.
*                       Save and restore poly weighting value.
*   18/11/13 1.35   AGC	Fix clang warning.
*   08/11/13 1.34   SP 	Make child's direction opposite to parent's default
*                       when creating child using the SPxAreaObj interface.
*   24/10/13 1.33   SP 	Add GetFullItemName().
*   21/10/13 1.32   AGC Support creating polygons through SPxActiveRegionDB.
*			Add GetSelectedPoly/Segment() functions.
*                   SP  Add SPxMasterActiveRegion.  
*                       Move weighting from region to polygon. 
*                       Add SPX_REGION_APP_ALARM application type. 
*                       Add IsApplicationPublic().
*   15/10/13 1.31   SP 	Add weighting parameter to SPxActiveRegion.
*   07/10/13 1.30   AGC	Add SPxAreaObj support.
*   04/10/13 1.29   AGC	Simplify headers.
*   21/12/12 1.28   AGC	Support FILE based Load/Save.
*   11/09/12 1.27   AGC	Move SPxPoint to SPxCommon.h.
*   13/07/12 1.26   AGC	Support fixing region lat/long.
*   29/02/12 1.25   SP 	Various new utility functions.
*   08/02/12 1.24   AGC	Include required headers.
*			Add applicationFixed param to ReadFilesFromFile().
*   09/01/12 1.23   DGJ	Add AddPolyLatLong function
*   21/12/11 1.22   DGJ	Support lat/long specification of areas etc.
*   09/01/11 1.21   AGC	Record and allow query of pending region changes for GUI.
*   17/12/10 1.20   SP 	Allow name and application to be fixed.
*   08/12/10 1.19   DGJ	Add #define for world vector shoreline.
*   28/10/10 1.18   AGC	Override SetConfigFromState().
*   15/10/10 1.17   DGJ	Fix #103 concerning incorrect processing 
*			of active regions that wrap the origin.
*   15/09/10 1.16   REW	Make another destructor virtual.
*   15/09/10 1.15   REW	Make destructors virtual.
*   16/06/10 1.14   REW	Add fromConstructor param to CommonCreate().
*   27/05/10 1.13   DGJ	Add SPX_REGION_APP_DRC.
*   18/02/10 1.12   DGJ	Add access control mutex and class init.
*   09/02/10 1.11   DGJ	Support m_application member.
*   02/02/10 1.10   DGJ	Save azimuth offset in degs and ps azimuths.
*			Support SetRadar() as alternate to SetOrigin()
*			Save filename used to load the active region.
*			Add const to char* for filenames.
*   20/01/10 1.9    REW	Derive more from SPxObj and support
*			Get/SetParameter() for remote control.
*   10/03/09 1.8    DGJ	Correct typo
*   08/03/09 1.7    DGJ	Add mutex control to SPxActiveRegion,
*			Support Lock and Unlock.
*   18/10/08 1.6    DGJ	Support for ReadFilesFromFile()
*   23/06/08 1.5    DGJ	Support for TestPoint()
*   17/01/08 1.4    DGJ	Support SetName, InsertPointAt, 
*			DeletePointAt, RemovePoly
*   07/01/08 1.3    DGJ	Added GetMode() and SetMode(). Add Set/Get
*			UserData.  Store instances of SPxActiveObject
*			in linked list.
*   05/12/07 1.2    DGJ	Add access functions 
*   17/09/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_PROCESS_REGION_H
#define _SPX_PROCESS_REGION_H

/* For SPxPoint. */
#include "SPxLibUtils/SPxCommon.h"

/* Required for base classes. */
#include "SPxLibUtils/SPxAreaObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxSysUtils.h"
#include "SPxLibUtils/SPxTime.h"

/* The symbol SPX_REGION_THRO is used to indicate that a shape is
 * defining an area that is not subject to processing. The symbol
 * SPX_REGION_LUT indicates that the shape is subject to processing.
 */
#define SPX_REGION_THRO 0
#define SPX_REGION_LUT 1

/* Allow a user of the active region object to assign an application
 * value to the object to say how it is being used. Check if
 * SPxActiveRegion::IsApplicationPublic() needs to be updated
 * when adding new regions.
 */
#define SPX_REGION_APP_UNKNOWN 0
#define SPX_REGION_APP_PROCESSING_AREA 1
#define SPX_REGION_APP_TRACK_PARAMETER_AREA 2
#define SPX_REGION_APP_DRC 3
#define SPX_REGION_APP_WORLD_VECTOR_SHORELINE 4
#define SPX_REGION_APP_ALARM 5


/* Define the maximum length of the name for a SPxActiveRegion object */
#define SPX_MAX_LEN_AR_NAME 64

/* Maximum radar active states per shape. */
#define SPX_REGION_SHAPE_MAX_RADARS (8)

/* Alarm types. */
#define SPX_REGION_ALARM_ENTER  0
#define SPX_REGION_ALARM_EXIT   1

/* Forward declarations. */
class SPxPIM;
class SPxActiveRegion;
class SPxMasterActiveRegion;

/* SPxCrossing stores a crossing point when a polygon is ray traced. */
class SPxCrossing
{
    double m_range;	    /* The range of the crossing in metres */
    int m_direction;	    /* Direction of polygon ON/OFF giving crossing */
    SPxCrossing *m_next;    /* Next in chain. */

public:
    int GetDirection(void) {return m_direction;}
    SPxCrossing *GetNext(void) {return m_next;}
    void SetNext(SPxCrossing *next) {m_next = next;}
    SPxCrossing(double range, int direction);
    virtual ~SPxCrossing(void) {;}
    double GetRange(void) {return m_range;}
};

/*
 * SPxShapeArea is an abstract base class for polygons and range-azimuth
 * segments.
 */
class SPxShapeArea :public SPxObj, public SPxAreaObj
{
public:
    /* Constructor/destructor. */
    SPxShapeArea(int direction, int active=1, const char *name=0, SPxActiveRegion *parent=NULL);
    virtual ~SPxShapeArea(void)=0;

    // cppcheck-suppress virtualCallInConstructor
    virtual void SetActive(int active);
    void SetRadarActive(int active, unsigned int radar);
    int GetRadarActive(unsigned int radar);

    void SetParent(SPxActiveRegion *parent) { m_parent = parent; }
    SPxActiveRegion *GetParent(void) const { return m_parent; }
    virtual void Recalculate(void);

    /* Mode. */
    virtual int IsModeSupported(void)		{return TRUE;}
    virtual SPxErrorCode SetMode(int mode);
    virtual int GetMode(void)			{return m_direction;} /* New interface */
    int GetDirection(void)			{return m_direction;} /* Deprecated */

    /* User data. */
    void SetUserData(void *data) {m_userData = data;}
    void *GetUserData(void)	{return m_userData;}

    /* User flags. */
    void SetUserFlags(UINT32 flags) {m_userFlags = flags;}
    UINT32 GetUserFlags(void) {return m_userFlags;}

    /* Linked list handling. */
    void SetNext(SPxShapeArea *shape)	{m_next = shape;}
    SPxShapeArea *GetNext(void) const	{return m_next;}

    int GetHasChangesForGui(void);

    /* Threat weighting. */
    virtual void SetWeighting(double val);
    virtual double GetWeighting(void) { return m_weighting; }

    /* Alarms controls. */
    virtual SPxErrorCode SetAlarmType(int type);
    virtual int GetAlarmType(void) { return m_alarmType; }
    virtual SPxErrorCode SetAlarmRefPointID(int id);
    virtual int GetAlarmRefPointID(void) { return m_alarmRefPointID; }
    virtual SPxErrorCode SetAlarmBearingError(double degs);
    virtual double GetAlarmBearingError(void) { return m_alarmBearingErrDegs; }
    virtual SPxErrorCode SetAlarmMaxSpeed(double mps);
    virtual double GetAlarmMaxSpeed(void) { return m_alarmMaxSpeedMps; }

    /* Timed enable or disable. */
    virtual SPxErrorCode SetTimedEnable(int isEnableNotDisable, unsigned int secs);
    virtual unsigned int GetTimedEnableRemainingSecs(void) const;
    virtual int IsTimedEnableInProgress(void) const { return m_isTimedEnableInProgress; }
    virtual int IsTimedEnableNotDisable(void) const { return m_isTimedEnableNotDisable; }
    virtual int CheckTimedEnable(void);

    /* SPxAreaObj interface. */
    virtual int IsNameSettable(void) const { return TRUE; }
    virtual int IsItemNameSet(void) const { return IsNameSet(); }
    virtual SPxErrorCode SetItemName(const char *name);
    virtual const char *GetItemName(void) const;
    virtual const char *GetFullItemName(void) const;
    virtual SPxErrorCode Update(void) { Recalculate(); return SPX_NO_ERROR; }
    virtual SPxErrorCode SetSelected(int selected);
    virtual int IsSelected(void) const { return m_selected; }
    virtual SPxErrorCode SetSelectedPoint(unsigned int idx);
    virtual unsigned int GetSelectedPoint(void) const { return m_selectedPoint; }
    virtual const SPxAreaObj *GetParentArea(void) const;
    virtual SPxAreaObj *GetParentArea(void);

    /* Static functions used to set defaults. */
    static void SetDefaultAlarmType(int type);
    static void SetDefaultAlarmRefPointID(int id);
    static void SetDefaultAlarmBearingError(double degs);
    static void SetDefaultAlarmMaxSpeed(double mps);

protected:
    int m_hasPendingChangesForGui;  /* Has the shape been changed in a way that might affect a GUI? */

    void SignalEvent(UINT32 flags);

private:
    SPxActiveRegion *m_parent;	    /* Parent active region. */
    int m_direction;		    /* ON or OFF */
    int m_radarActive[SPX_REGION_SHAPE_MAX_RADARS]; /* Radar specific active state. */
    void *m_userData;		    /* Some user-supplied data. */
    UINT32 m_userFlags;             /* Some user-supplied flags. */
    int m_selected;		    /* Is the shape selected? */
    SPxShapeArea *m_next;	    /* Next in chain */
    unsigned int m_selectedPoint;
    mutable char m_fullName[256];   /* Full <region>:<shape> object name. */
    double m_weighting;             /* Weighting for threat calculation. */
    int m_alarmType;                /* Alarm type. */
    int m_alarmRefPointID;          /* Alarm reference point ID or -1 if not set. */
    double m_alarmBearingErrDegs;   /* Alarm bearing error wrt reference point. */
    double m_alarmMaxSpeedMps;      /* Alarm max allowable speed. */
    int m_isTimedEnableInProgress;  /* Is timed enable/disable in progress? */
    SPxTime_t m_timedEnableEndTime; /* Timed enable/disable end time. */
    int m_isTimedEnableNotDisable;  /* Timed enable or timed disable? */
    mutable SPxCriticalSection m_timedEnableMutex;  /* Mutex used for timed enable. */

    void UpdateFullName(void) const;
    unsigned int getTimedEnableRemainingSecs(void) const;
    void setActiveFromTimer(int active);

}; /* SPxShapeArea */

/*
 * SPxPolyArea stores a polygon area.
 */
class SPxPolyArea :public SPxShapeArea
{
public:
    /* Constructor/destructor. */
    SPxPolyArea(int numPts, const SPxPoint *points, int mode, int active=1,
		const char *name=0, int useLatLong = 0, SPxActiveRegion *parent=NULL);
    virtual ~SPxPolyArea(void);

    /* Point manipulation. */
    SPxErrorCode InsertPointAt(int position, SPxPoint point);
    SPxErrorCode ModifyPointAt(int position, SPxPoint point);
    SPxErrorCode DeletePointAt(int position);
    int IsPointInsidePoly(double xMetres, double yMetres) const;
    int IsLatLongInsidePoly(double latDegs, double longDegs) const;
    int GetClosestPointIndex(double xMetres, double yMetres) const;

    /* Point retrieval. */
    virtual unsigned int GetNumPoints(void) const { return static_cast<unsigned int>(m_numPoints); }
    SPxErrorCode SetPoints(const SPxPoint *points, unsigned int numPoints);
    const SPxPoint *GetPoints(void) const { return m_points.get(); }
    SPxPoint *GetPoints(void) { return m_points.get(); }
    const SPxPoint *GetPointsToUse(void) const { return (m_useLatLong ? m_pointsLatLong.get() : m_points.get()); }
    SPxPoint *GetPointsToUse(void) { return (m_useLatLong ? m_pointsLatLong.get() : m_points.get()); }
    const SPxPoint *GetLatLongPoints(void) const { return m_pointsLatLong.get(); }
    SPxPoint *GetLatLongPoints(void) { return m_pointsLatLong.get(); }
    int UsingLatLong(void) const {return m_useLatLong;}

    /* Linked list handling to preserve old API return type. */
    SPxPolyArea *GetNext(void) const
    {
	return((SPxPolyArea *) SPxShapeArea::GetNext());
    }

    void WriteToFile(FILE *f);

    /* Generic parameter assignment from SPxObj base class. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const { return SPX_AREA_TYPE_POLY; }
    virtual SPxAreaUnits GetUnits(void) const;
    virtual int IsMovable(void) const { return TRUE; }
    virtual SPxErrorCode SetPos(double x, double y);
    virtual SPxErrorCode GetPos(double *x, double *y) const;
    virtual int IsPointWithin(double x, double y, double margin) const;
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio=FALSE);
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y) const;
    virtual int IsEditable(void) const { return TRUE; }
    virtual SPxErrorCode AddPoint(double x, double y);
    virtual SPxErrorCode RemovePoint(unsigned int idx) { return DeletePointAt(static_cast<int>(idx)); }
    virtual SPxErrorCode AppendPoint(double x, double y);

private:
    int m_numPoints;		    /* Number of points */
    SPxAutoPtr<SPxPoint[]> m_points;/* Array of points */
    SPxAutoPtr<SPxPoint[]> m_pointsLatLong; /* Array of lat/longs (NOTE x=lat, y=long) */
    int m_useLatLong;		    /* 1 if we are using lat/longs */
    unsigned int m_selectedPoint;   /* Index of selected point (or SPX_AREA_NO_POINT). */

    int isInsidePoly(double x, double y, int isLatLong) const;

}; /* SPxPolyArea */


/*
 * SPxSegment class stores a range/azimuth segment area.
 */
class SPxSegment :public SPxShapeArea
{
public:
    /* Constructor/destructor. */
    SPxSegment(double sr, double er, double sa, double ea, int mode,
	       int active=1, const char* name = 0, SPxActiveRegion *parent=NULL);
    virtual ~SPxSegment(void);

    /* Editing functions. */
    void SetStartRange(double val);
    void SetEndRange(double val);
    void SetStartAzimuth(double val);
    void SetEndAzimuth(double val);

    /* Info retrieval. */
    double GetStartRange(void)		{return m_startRange;}
    double GetEndRange(void)		{return m_endRange;}
    double GetStartAzimuth(void)	{return m_startAzimuth;}
    double GetEndAzimuth(void)		{return m_endAzimuth;}

    /* Linked list handling to preserve old API return type. */
    SPxSegment *GetNext(void) const
    {
	return((SPxSegment *) SPxShapeArea::GetNext());
    }

    void WriteToFile(FILE *f);

    /* Generic parameter assignment from SPxObj base class. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const { return SPX_AREA_TYPE_SECTOR; }
    virtual SPxAreaUnits GetUnits(void) const { return SPX_AREA_UNITS_METRES; }
    virtual unsigned int GetNumPoints(void) const { return 4; }
    virtual int IsPointWithin(double x, double y, double margin) const;
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio=FALSE);
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y) const;

private:
    double m_startRange;	    /* Start range in metres */
    double m_endRange;		    /* End range in metres */
    double m_startAzimuth;	    /* Start azimuth in degrees */
    double m_endAzimuth;	    /* End azimuth in degrees */

}; /* SPxSegment */



/* Class for an inteval, being a range/length pair for a specific azimuth. */
class SPxPolarInterval
{
    double m_range;		/* The start range of the interval in metres */
    double m_interval;		/* The length of the interval in metres */
    SPxPolarInterval *m_next;	/* The next interval */

public:
    SPxPolarInterval *GetNext(void)	{return m_next;}
    void SetNext(SPxPolarInterval * next) {m_next = next;}
    double GetRange(void)		{return m_range;}
    double GetInterval(void)		{return m_interval;}
    void SetRange(double range)		{m_range = range;}
    void SetInterval(double interval)	{m_interval = interval;}
    SPxPolarInterval(double range, double interval);
    virtual ~SPxPolarInterval(void)	{;}
};

/* Class for an active region. This class manages a per-azimuth set of
 * intervals that define the area of interest. The area may be changed
 * with polygons or range-azimuth segments.
 */
class SPxActiveRegion :public SPxObj, public SPxAreaObj
{
    struct FnDesc
    {
	SPxAreaEventFn fn;
	void *userArg;
    };

    SPxCrossing *m_crossing;	    /* Crossing points for an azimuth */
    SPxPolarInterval **m_interval;  /* Per-azimuth interval lists */
    int m_numAzis;		    /* Number of azimuths */
    int m_initialDirection;	    /* Initial ON/OFF direction */
    SPxPoint m_origin;		    /* Points in input are relative to this origin. */
    SPxPoint m_originLL;	    /* Lat/long origin. */
    int m_isLatLongFixed;	    /* Set to 1 to prevent lat/long being changed by loading
				     * from a file.
				     */
    int m_originLLset;		    /* Set to 1 when the LL has been set. */
    int m_numCrossings;		    /* Number of crossings for current azimuth */
    int m_numPolys;		    /* The number of polygons. */
    SPxPolyArea *m_firstPolyArea;   /* The first polygon in a linked list */
    SPxPolyArea *m_lastPolyArea;    /* The last polygon */
    int m_numSegments;		    /* The number of range/azimuth segments */
    SPxSegment *m_firstSegment;	    /* The first range-azimuth segment */
    SPxSegment *m_lastSegment;	    /* The last range-azimuth segment. */
    int m_azimuthOffsetInPS;	    /* An azimuth offset in PIM azimuths. */  
    double m_azimuthOffsetDeg;	    /* Azimuth offset in degrees */
    double m_scaleFactor;	    /* Scale Factor. */
    void *m_userData;		    /* User assigned data */
    static SPxCriticalSection m_mutex;	/* List protection mutex */
    static SPxActiveRegion *m_firstObject;	    
				    /* Maintain a linked-list of objects. */
    static SPxActiveRegion *m_lastObject;	    
				    /* End of linked list */
    SPxActiveRegion *m_next;	    /* Next object in the linked list. */
    SPxCriticalSection m_access;    /* Access control */
    mutable SPxPath m_associatedFileName;   /* If active region loaded from file
				     * the filename is stored here.
				     */
    int m_application;		    /* How is this active region being used? */
    int m_isNameFixed;              /* Is object name fixed? */
    int m_isApplicationFixed;       /* Is application type fixed? */
    int m_hasPendingChangesForGui;  /* Has the region been changed in a way that might affect a GUI? */
    int m_selected;		    /* Is the active region selected? */
    UINT32 m_tickerOfLastRecalculate; /* When did we last do a recalculate (ticker count) ? */
    int m_createEnabled;	    /* Is SPxAreaObj creation enabled? */
    SPxMasterActiveRegion *m_master;/* Master for this slave region. */
    double m_weighting;             /* Weighting for threat calculation. */
    int m_isWeightingFixed;         /* Is weighting fixed? */
    int m_polyMetresCreateEnabled;  /* Can metres polygons be created? */
    int m_polyLatLongCreateEnabled; /* Can lat/long polygons be created? */
    int m_segmentCreateEnabled;	    /* Can segments be created? */
    static SPxNavData *m_navData;   /* Nav data object that provides heading */
    int m_headingRef;		    /* Interpret non lat/long polys as
				    heading referenced (default = true) */
    FnDesc m_eventFns[16];	    /* Area event callbacks. */

    int AddCrossing(double range, int direction);
    void ClearCrossings(void);
    int TestCrossings(double azimuthInRadians, int numPts, const SPxPoint *pts, double*);
    int AddInterval(int azimuth, double range, double length, int direction);
    void MergeCrossings(int);    
    int ProcessPoly(SPxPolyArea*);

    SPxPolyArea* AddPolyInternal(int numPts, const SPxPoint *pts, int direction,
			 int active = 1, const char *name = 0);
    SPxPolyArea* AddPolyLatLongInternal(int numPts, const SPxPoint *pts, int direction,
				int active = 1, const char *name = 0);
    void RemovePolyInternal(SPxPolyArea *poly, int destruct = TRUE);

    int isOdd(int x) {return (x/2!=(x+1)/2);}
    int TestIntersect(double azi, const SPxPoint *p1, const SPxPoint *p2, 
			double azimuthP1, double azimuthP2,
			double *x, double *y);    
    void ClearIntervals(void);
    int ProcessSegment(SPxSegment *segment);
    void CommonCreate(int numAzis, int initialDirection, const char *name,
			int fromConstructor);
    void ClearAll(void);

    static SPxActiveRegion *getRegionFromPoint(double x, double y,
                                               int isPointLatLong,
                                               int application,
                                               int alignedOnly,
                                               SPxPolyArea **polyRtn,
                                               int *pointIndex);

public:
    /* Access to linked list of allocated objects. */
    static SPxActiveRegion *GetFirstObject(int incSlaveRegions=FALSE);
    static SPxActiveRegion *GetLastObject(int incSlaveRegions=FALSE);
    static SPxActiveRegion *GetByName(const char *name,
					int requirePreviousSet = TRUE,
					int lockListOnSuccess = FALSE);
    static void LockList(void);
    static void UnLockList(void);
    static SPxActiveRegion *GetRegionFromPoint(double xMetres, 
                                               double yMetres,
                                               int application,
                                               int alignedOnly,
                                               SPxPolyArea **polyRtn,
                                               int *pointIndexRtn);

    static SPxActiveRegion *GetRegionFromLatLong(double latDegs, 
                                                 double longDegs,
                                                 int application,
                                                 int alignedOnly,
                                                 SPxPolyArea **polyRtn,
                                                 int *pointIndexRtn);

    /* Check application type. */
    static int IsApplicationPublic(int application);

    SPxActiveRegion *GetNext(int incSlaveRegions=FALSE);

    /* Get time for last recalculate */
    UINT32 GetLastRecalculateTicker(void) {return m_tickerOfLastRecalculate;}

    /* Init/Shutdown functions for class */
    static int SPxActiveRegionInit(void);
    static int SPxActiveRegionShutdown(void);

    void SetScaleFactor(double scale) {m_scaleFactor = scale;}
    double GetScaleFactor(void) const {return m_scaleFactor;}

    /* Set/Get heading ref */
    void SetHeadingRef(int headingRef) {m_headingRef = headingRef;}
    int GetHeadingRef(void) {return m_headingRef;}

    /* Reset everything to its start-up state */
    void Reset(void);

    /* Get and set the application. */
    int GetApplication(void) const {return m_application;}
    void SetApplication(int app);
    int IsApplicationPublic(void) { return IsApplicationPublic(m_application); }

    /* Should active areas adjust the azimuth by using heading from nav data ? */
    static void AdjustAzimuthsUsingHeading(SPxNavData *navData) {m_navData = navData; }

    /* Test to see if a range, azimuth point is inside the active region. */
    int TestPoint(double range, double azimuth);

    /* Test to see if a finite line segment intersects the region. */
    int DoesLineSegmentHitRegion(double x1, double y1, double x2, double y2,
					double *xRet, double *yRet);

    /* Set and get user data */
    void SetUserData(void *user) {m_userData = user;}
    void *GetUserData(void) {return m_userData;}

    virtual int IsModeSupported(void) {return TRUE;}
    virtual SPxErrorCode SetMode(int mode);
    virtual int GetMode(void) {return m_initialDirection;}

    /* Read/write a specification of polygons and segments from a file. */
    int ReadFromFile(const char *fileName);
    int ReadFromFile(FILE *f);
    int SaveToFile(const char *fileName) const;
    int SaveToFile(FILE *f) const;
    static int ReadFilesFromFile(const char *fileName, int numAzis, int mode,
				int application = SPX_REGION_APP_UNKNOWN,
				int applicationFixed = TRUE,
				SPxActiveRegion **list = NULL,
				int size = 0, int *numReturned = NULL);

    const char *GetAssociatedFileName(void) const {return m_associatedFileName.GetShortPath();}
    const char *GetAssociatedFilePath(void) const {return m_associatedFileName.GetFullPath();}

    /* Set an azimuth offset in degrees */
    void SetAzimuthOffset(double azimuthOffset);
    double GetAzimuthOffset(void) const;

    /* Set/Get a new origin (radar position) for the polygons. */
    void SetOrigin(double x, double y) {SetRadar(x,y);}
    void SetOriginLatLong(double lat, double lon) {SetRadarLatLong(lat,lon);}
    void SetRadar(double x, double y) {m_origin.x = x; m_origin.y = y; m_hasPendingChangesForGui = TRUE;}
    void SetRadarLatLong(double lat, double lon) {m_originLL.x = lat; m_originLL.y = lon; m_originLLset = 1; m_hasPendingChangesForGui = TRUE;}
    void SetLatLongFixed(int fixed=TRUE) { m_isLatLongFixed = fixed; }

    double GetOriginX(void) const {return m_origin.x;}
    double GetOriginY(void) const {return m_origin.y;}
    double GetRadarLat(void) const {return m_originLL.x;}
    double GetRadarLong(void) const {return m_originLL.y;}

    /* Get the first interval for an azimuth index. */
    SPxPolarInterval *GetInterval(int azi);

    /* Lock and unlock for access to the object */
    void Lock(void) {m_access.Enter();}
    void UnLock(void) {m_access.Leave();}

    /* Return number of azimuths that the class was constructed to handle. */
    int GetNumAzis(void) {return m_numAzis;}

    /* Return inital direction set in the constructor. */
    int GetInitialDirection(void) {return m_initialDirection;}

    /* Create a new active region with a number of azimuths and an initial
     * direction.
     */
    SPxActiveRegion(int numAzis, int initialDirection,
				const char *name = NULL);
    SPxActiveRegion(SPxPIM *sourcePIM, int initialDirection,
				const char *name = NULL);

    /* Delete */
    virtual ~SPxActiveRegion(void);

    /* Add/remove a polygon. */
    SPxPolyArea* AddPoly(int numPts, const SPxPoint *pts, int direction,
				int active = 1, const char *name=0);
    SPxPolyArea* AddPolyLatLong(int numPts, const SPxPoint *pts, int direction,
				int active = 1, const char *name=0);

    void RemovePoly(SPxPolyArea *poly, int destruct=TRUE);

    /* Add a range-azimuth segment. */
    SPxSegment *AddSegment(double rangeStart, double rangEnd, 
				double azimuthStart,double azimuthEnd, 
				int direction, int active=1,
				const char *name = 0);
    void RemoveSegment(SPxSegment *segment, int destruct=TRUE);

    /* Access functions */
    SPxPolyArea *GetPolyByName(const char* name) const;
    SPxSegment *GetSegmentByName(const char* name) const;
    SPxPolyArea *GetFirstPoly(void) const {return m_firstPolyArea;}
    SPxSegment *GetFirstSegment(void) const {return m_firstSegment;}
    int GetNumPolys(void) const {return m_numPolys;}
    int GetNumSegments(void) const {return m_numSegments;}

    /* Recalculate the intervals, perhaps because the offset or origin
     * has changed.
     */
    virtual void Recalculate(void);

    /* Generic parameter assignment from SPxObj base class. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* SetConfigFromState() */
    virtual SPxErrorCode SetConfigFromState(void);

    /* Prevent certain attributes from being changed */
    void SetNameFixed(int isFixed);
    void SetApplicationFixed(int isFixed);

    int GetHasChangesForGui(void);

    /* Threat weighting. */
    virtual void SetWeighting(double val);
    virtual void SetWeightingFixed(int isFixed);
    virtual double GetWeighting(void) { return m_weighting; }

    /* Timed enable. */
    virtual SPxErrorCode SetTimedEnable(int isEnable, 
                                        unsigned int secs,
                                        int operation);
    virtual int CheckTimedEnable(void);

    /* SPxAreaObj interface. */
    virtual int IsNameSettable(void) const { return TRUE; }
    virtual int IsItemNameSet(void) const { return IsNameSet(); }
    virtual SPxErrorCode SetItemName(const char *name);
    virtual const char *GetItemName(void) const;
    virtual SPxErrorCode SetSelected(int selected);
    virtual int IsSelected(void) const;
    SPxErrorCode EnableCreate(int enable);
    SPxPolyArea *GetSelectedPoly(void) const;
    SPxSegment *GetSelectedSegment(void) const;
    virtual unsigned int GetNumTypes(void) const;
    virtual const char *GetTypeName(unsigned int typeIndex) const;
    virtual SPxAreaObj *CreateArea(unsigned int typeIndex);
    virtual SPxErrorCode IterateObjects(SPxAreaObjFn fn, void *userArg) const;

    virtual int IsSaveSupported(void) const { return TRUE; }
    virtual SPxErrorCode Load(const char *name) { return static_cast<SPxErrorCode>(ReadFromFile(name)); }
    virtual SPxErrorCode Save(const char *name) const { return static_cast<SPxErrorCode>(SaveToFile(name)); }
    virtual const char *GetFullPath(void) const { return GetAssociatedFilePath(); }
    virtual const char *GetShortPath(void) const { return GetAssociatedFileName(); }

    virtual SPxErrorCode AddEventCallback(SPxAreaEventFn fn, void *userArg);
    virtual SPxErrorCode RemoveEventCallback(SPxAreaEventFn fn, void *userArg);
    void SignalEvent(SPxAreaObj *obj, UINT32 flags, int mutexAlreadyLocked=FALSE);

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const { return SPX_AREA_TYPE_GROUP_LAT_LONG; }

    /* Allow/disallow object creation via SPxAreaObj interface. */
    SPxErrorCode EnablePolyMetresCreate(int enable);
    SPxErrorCode EnablePolyLatLongCreate(int enable);
    SPxErrorCode EnableSegmentCreate(int enable);
    int IsPolyMetresCreateEnabled(void) const { return m_polyMetresCreateEnabled; }
    int IsPolyLatLongCreateEnabled(void) const { return m_polyLatLongCreateEnabled; }
    int IsSegmentCreateEnabled(void) const { return m_segmentCreateEnabled; }

    /* Master region that this region is a slave to. */
    virtual void SetMaster(SPxMasterActiveRegion *master) { m_master = master; }
    virtual SPxMasterActiveRegion *GetMaster(void) { return m_master; }

    friend class SPxMasterActiveRegion;
    friend class SPxActiveRegionDB;
};

/* Class for creating a master active region that defines the
 * lat/long polygons to be used by one or more slave active regions. 
 */
class SPxMasterActiveRegion : public SPxActiveRegion
{
public:
    explicit SPxMasterActiveRegion(int numAzis=1, 
                                   int initialDirection=SPX_REGION_THRO,
				   const char *name=NULL);
    virtual ~SPxMasterActiveRegion(void) {}
    virtual void Recalculate(void);
    virtual void SetActive(int active);

private:

};

/* Class for managing all the active regions. */
class SPxActiveRegionDB :public SPxObj, public SPxAreaObj
{
public:
    explicit SPxActiveRegionDB(int numAzis);
    virtual ~SPxActiveRegionDB(void);

    /* Generic parameter assignment from SPxObj base class. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    virtual const char *GetItemName(void) const { return "ActiveRegions"; }

    SPxErrorCode EnableRegionCreate(int enable);
    SPxErrorCode EnablePolyMetresCreate(int enable);
    SPxErrorCode EnablePolyLatLongCreate(int enable);
    SPxErrorCode EnableSegmentCreate(int enable);
    SPxPolyArea *GetSelectedPoly(void) const;
    SPxSegment *GetSelectedSegment(void) const;
    virtual unsigned int GetNumTypes(void) const;
    virtual const char *GetTypeName(unsigned int typeIndex) const;
    virtual SPxAreaObj *CreateArea(unsigned int typeIndex);
    virtual SPxErrorCode AddEventCallback(SPxAreaEventFn fn, void *userArg);
    virtual SPxErrorCode RemoveEventCallback(SPxAreaEventFn fn, void *userArg);
    void SignalEvent(SPxAreaObj *obj, UINT32 flags);

    /* Get and set the application for managed active regions. */
    int GetApplication(void) const {return m_application;}
    void SetApplication(int app) {m_application = app;}

    /* Set/Get a new origin (radar position) for managed active regions. */
    void SetRadarLatLong(double lat, double lon);
    double GetRadarLong(void) const {return m_originLL.y;}

    /* Iterate over children. */
    virtual SPxErrorCode IterateObjects(SPxAreaObjFn fn, void *userArg) const;

    /* Access to linked list of allocated objects. */
    static SPxActiveRegionDB *GetFirstObject(void) { return m_firstObject; }
    static SPxActiveRegionDB *GetLastObject(void) { return m_lastObject; }

    SPxActiveRegionDB *GetNext(void) { return m_next; }

private:
    struct FnDesc
    {
        SPxAreaEventFn fn;
        void *userArg;
    };

    int m_numAzis;
    int m_regionCreateEnabled;
    int m_polyMetresCreateEnabled;
    int m_polyLatLongCreateEnabled;
    int m_segmentCreateEnabled;
    int m_application;
    mutable char m_typeNameCache[64];
    SPxCriticalSection m_access;    /* Access control */
    FnDesc m_eventFns[16];	    /* Area event callbacks. */
    SPxPoint m_originLL;	    /* Lat/long origin. */
    int m_originLLset;		    /* Set to 1 when the LL has been set. */

    static SPxCriticalSection m_mutex;	/* List protection mutex */
    static SPxActiveRegionDB *m_firstObject;	    
    /* Maintain a linked-list of objects. */
    static SPxActiveRegionDB *m_lastObject;	    
    /* End of linked list */
    SPxActiveRegionDB *m_next;	    /* Next object in the linked list. */

    int GetDirectionForChild(SPxActiveRegion *region);
};

#endif /* _SPX_PROCESS_REGION_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
