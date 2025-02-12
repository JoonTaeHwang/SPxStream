/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for SPxViewControl class.
*
*
* Revision Control:
*   16/03/21 v1.42   AGC	Support display rotation with web mercator.
*
* Previous Changes:
*   08/03/21 1.41   REW	Add SPxLatLongH_t type.
*   03/08/21 1.40   AGC	Support web mercator adjusted radar/platform position.
*   29/11/19 1.39   AGC	Add MOTION_MODE_STATIC.
*   15/10/19 1.38   BTB	Support limiting the maximum view offset.
*   11/06/19 1.37   AGC	Support Web Mercator projection for lat/long conversions.
*   10/01/19 1.36   BTB	Support loading/saving/storing of home view.
*   30/11/18 1.35   AGC	Allow manually set device offsets.
*   26/11/18 1.34   AGC	Do not update initial platform lat/long when expired.
*   20/11/18 1.33   AGC	Support use with world map.
*   19/06/18 1.32   AJH	Add m_logFile support.
*   14/03/18 1.32   SP 	Add Set/GetViewMinWidthDegs().
*                       Add Set/GetViewMinWidthMetres().
*                       Add Set/GetViewMaxWidthMetres().
*   21/12/17 1.31   SP  Add SetNumPlatforms().
*   16/05/17 1.30   AGC	Add platform speed support.
*			Support constant view width in degrees or metres.
*			Overhaul mutex locking.
*   21/02/17 1.29   AGC	Add various metres based functions.
*   19/10/16 1.28   SP 	Add GetViewExtentLatLong().
*   18/05/16 1.27   AGC	Support course-up display.
*   11/05/16 1.26   AGC	Add moving platform reference mode support.
*   04/12/15 1.25   REW	Support stored views.
*   17/11/15 1.24   AGC	Change max latitude to 89.9 degrees.
*   09/11/15 1.23   AGC	Add normalise flag to Metres/PixelsToLatLong().
*			Change max latitude to 90.0 degrees.
*   22/04/15 1.22   AGC	Support unlimited number of radars.
*			Support devices (same as radars).
*   26/03/15 1.21   SP 	Support unlimited number of platforms.
*   16/02/15 1.20   SP 	Support removal of nav data objects.
*                       Add SetViewFitToRadars() function.
*   18/07/14 1.19   AGC	Make more functions const.
*   01/05/14 1.18   AGC	Make some functions const.
*   13/12/13 1.17   SP 	Support maximum view width limit.
*   21/10/13 1.16   AGC	Use private structure.
*   04/10/13 1.15   AGC	Simplify headers.
*   04/10/13 1.14   SP 	Add GetPlatformNavData().
*   27/09/13 1.13   SP 	Remove need for AddPlatform(). 
*                       Add IsPlatformOnScreen().
*                       Add SetAutoViewRecentreEnabled().
*   17/09/13 1.12   SP 	Add support for multiple platforms. 
*   14/08/13 1.11   SP 	Add SetViewWidthMetres().
*                       Add SetViewRectPixels().
*   23/11/12 1.10   AGC	Tidy forward declarations.
*   09/11/12 1.9    SP 	Add normaliseLatLong().
*   26/10/12 1.8    AGC	Add tag name for LatLong structure.
*			Add LatLongToPixels/PixelsToLatLong() functions.
*   12/06/12 1.7    SP 	Add SetWindowSizeAuto().
*			Add PollWindowSize().
*   08/05/12 1.6    SP 	Add configurable rotation threshold.
*   30/04/12 1.5    SP 	Support change buffering.
*   19/04/12 1.4    SP 	Add SPX_VC_CLEAR_RADAR_TRAILS flag.
*   08/03/12 1.3    SP 	Move radar adjustment to SPxNavData.
*   29/02/12 1.2    SP 	Add m_prevDisplayRotationDegs.
*                       Add GetRadarLatLong().
*   08/02/12 1.1    SP 	Initial Version.
*
**********************************************************************/

#ifndef _SPX_VIEW_CONTROL_H
#define _SPX_VIEW_CONTROL_H

/*
 * Other headers required.
 */

/* Library headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxLatLongUTM.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibUtils/SPxStrings.h"

/* Forward declare other classes required. */
class SPxNavData;
class SPxWorldMap;
class SPxScFollow;
class SPxRenderer;

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Max/min latitude is limited to +/- this value. The value of 
 * (2 * 85.0511) is the height of the world in degrees latitude 
 * using a Mercator projection.
 * Changed to +/-90.0 on 06/11/2015 as other changes mean we are
 * no longer limited to +/-85.0511 in displaying the tiled map.
 * Changed to +/-89.9 on 17/11/2015 as otherwise the zoom level
 * is unbounded as you pan towards +/-90.0.
 */
#define SPX_VC_MAX_LAT_DEGS 89.9

/* 
 * Change flags. 
 *
 * These flags indicate a change in the platform position/rotation 
 * or view position/scale that exceeds a specified threshold
 * defined in pixels. These flags are intended to be used on a moving 
 * platform display to redraw a PPI window after a significant change 
 * in the platform position/rotation or view position/scale. Setting 
 * the change threshold in pixels (as opposed to real world units) makes 
 * the redraw frequency dependent on the platform speed and view scale, 
 * and avoids unnecessary redraws after each platform update.
 * Refer to SetUpdateThreshPixels() for further details.
 */

/* The view centre position in the PPI window defined in 
 * lat/long has changed by more than the threshold amount.
 */
#define SPX_VC_CHANGE_VIEW_CENTRE_LATLONG       (1U << 0)

/* The view centre position in the PPI window defined in 
 * metres from the primary platform has changed by more than the 
 * threshold amount. As the view centre in metres is referenced from 
 * the primary platform position, a change in primary platform position 
 * may cause a change to the view defined in metres but not to the view 
 * defined in lat/long.
 */
#define SPX_VC_CHANGE_VIEW_CENTRE_METRES        (1U << 1)

/* The view width or scale in the PPI window changed by more 
 * than the threshold amount.
 */
#define SPX_VC_CHANGE_VIEW_SCALE                (1U << 2)

/* The rotation of the display has changed in the PPI window. */
#define SPX_VC_CHANGE_DISPLAY_ROTATION          (1U << 3)

/* The position or rotation of any platform or any radar 
 * on a platform has changed. 
 */
#define SPX_VC_CHANGE_RADAR                     (1U << 4)

/* This flag may be set by a third party to request a redraw
 * of the display even though the view and other parameters may 
 * be unchanged. Typically this used by renderer classes to
 * force a redraw when graphics are changed, e.g. shown or hidden.
 */
#define SPX_VC_CHANGE_REQUEST_REDRAW            (1U << 5)

/* This flag is set if the change to the view or radar position
 * was a result of a user action as opposed to just a moving platform 
 * update. The only exception is a view auto recentre due to primary 
 * platform leaving the window, which is reported as a user action.
 * It's typically used by scan converters to determine if trails
 * should be cleared. 
 */
#define SPX_VC_USER_ACTION                      (1U << 6)

/* All change flags. */
#define SPX_VC_CHANGE_ANY                       0xFFFFFFFF

/*
 * Maximum number of stored views supported.
 */
#define	SPX_VC_MAX_STORED_VIEWS			4


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
struct SPxNavDataDeviceInfo;
struct SPxTime_tag;

/* Lat/long coordinate pair. This must be scalar, so 
 * that "=" works for copying. 
 */
typedef struct SPxLatLong_tag
{
    double latDegs;
    double longDegs;
} SPxLatLong_t;

/* Lat/long and height.  Use SPxLatLong_t as a member so functions that use
 * this can pass the internal bit around to other functions that don't need h.
 */
typedef struct SPxLatLongH_tag
{
    SPxLatLong_t ll;
    double h;		/* Height in metres, nominally above sea level. */
} SPxLatLongH_t;

/* Define the type of the range callback function used in SetViewFitToRadars(). 
 * This function must return the range of the radar in metres or 0.0
 * to exclude the radar from the view calculation.
 */
class SPxViewControl;
typedef double (*SPxViewControlRangeFn_t)(SPxViewControl *viewCtrl,
                                          unsigned int platformIndex,
                                          unsigned int radarIndex,
                                          void *userArg);

/*********************************************************************
*
*   Class definitions
*
**********************************************************************/

/*
 * Define our class, derived from the SPx base object.
 */
class SPxViewControl : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Display motion modes. */
    typedef enum
    {
        /* The primary platform remains at a fixed location in the window 
         * and the world moves around it. A stationary target or landmass 
         * appears at a different location in the window as the primary 
         * platform moves.
         */
        MOTION_MODE_RELATIVE, 

        /* The primary platform moves about the window, as does the position 
         * of platform relative features such as tracks. Radar returns 
         * associated with static targets appear in the same position.  
         * Where the display features a map, the characteristic of a 
         * true motion display is that, for the same view, the map 
         * remains fixed and the primary platform moves across the map.
         */
        MOTION_MODE_TRUE,

        /* The view centre remains fixed unless explicity modified. */
        MOTION_MODE_STATIC
    
    } MotionMode_t;

    /* Display reference modes. */
    typedef enum
    {
        /* The display is oriented with true north at 12 o'clock. If a 
         * platform heading marker is displayed, it will rotate to 
         * show the ship's heading.  
         */
        REF_MODE_NORTH,

        /* The display is oriented with primary platform's heading at 
         * 12 o'clock. With a heading referenced display the radar 
         * video and graphics, including the map, are rotated as the 
         * primary platform's heading changes.
         */
        REF_MODE_HEADING,

	/* The display is oriented with primary platform's course at
	 * 12 o'clock. With a course referenced display the radar
	 * video and graphics, including the map, are rotated as the
	 * primary platform's course changes.
	 */
	REF_MODE_COURSE
    
    } RefMode_t;
    
    /* View width modes. */
    typedef enum
    {
	/* Keep the view width constant in degrees of longitude
	 * as the platform moves.
	 * The view width in metres will change as the platform
	 * moves North or South.
	 */
	VIEW_WIDTH_MODE_CONST_DEGS,

	/* Keep the view width constant in metres as the platform
	 * moves.
	 * The view width in degrees of longitude will change as
	 * the platform moves North or South.
	 */
	VIEW_WIDTH_MODE_CONST_METRES

    } ViewWidthMode_t;

    /* Web Mercator modes. */
    enum WebMercatorMode_t
    {
	/* Never use Web Mercator projection for lat/long pixel
	 * conversions.
	 */
	WEB_MERCATOR_MODE_OFF = 0,

	/* Always use Web Mercator projection for lat/long pixel
	 * conversions.
	 */
	WEB_MERCATOR_MODE_ON = 1
    };

    /* Callback info structure. */
    typedef struct _CallbackInfo_t
    {
        _CallbackInfo_t *next;      /* Next structure in the list. */
        void *targetObj;            /* The object to control. */
	unsigned int deviceIndex;   /* Assigned device index. */
	unsigned int radarIndex;    /* Assigned radar index (same as device index). */
        unsigned int platformIndex; /* Assigned platform index. */
    
    } CallbackInfo_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxViewControl(SPxNavData *navData=NULL, SPxWorldMap *worldMap=NULL);

    virtual ~SPxViewControl(void);

    /* Window. */
    SPxErrorCode SetWindowSizeAuto(SPxScFollow *follow);

    SPxErrorCode SetWindowSizePixels(unsigned int widthPixels,
                                     unsigned int heightPixels);

    SPxErrorCode GetWindowSizePixels(unsigned int *widthPixels,
                                     unsigned int *heightPixels) const;

    /* View centre. */
    SPxErrorCode SetViewCentreLatLong(const SPxLatLong_t *centreLL);

    SPxErrorCode GetViewCentreLatLong(SPxLatLong_t *centreLL) const;

    SPxErrorCode SetViewCentrePixels(int xPixels, 
                                     int yPixels);

    SPxErrorCode GetViewCentrePixels(int *xPixels, 
                                     int *yPixels) const;

    SPxErrorCode SetViewCentreMetres(double cXMetres,
				     double cYMetres);

    SPxErrorCode GetViewCentreMetres(double *cXMetres, 
                                     double *cYMetres) const;
    
    SPxErrorCode SetViewCentreMetresDisplay(double cXMetres,
					    double cYMetres);

    SPxErrorCode GetViewCentreMetresDisplay(double *cXMetres,
					    double *cYMetres) const;

    SPxErrorCode SetViewAutoRecentreEnabled(int isEnabled);

    int IsViewAutoRecentreEnabled(void) const {return m_isViewAutoRecentreEnabled;}

    SPxErrorCode SetMaxViewOffsetPercent(double percent);

    SPxErrorCode GetMaxViewOffsetPercent(double *percent) const;

    /* View scale. */
    SPxErrorCode SetViewWidthDegs(double widthDegs);

    SPxErrorCode GetViewWidthDegs(double *widthDegs) const;

    SPxErrorCode SetViewMinWidthDegs(double widthDegs);

    SPxErrorCode GetViewMinWidthDegs(double *widthDegs) const;

    SPxErrorCode SetViewMaxWidthDegs(double widthDegs);

    SPxErrorCode GetViewMaxWidthDegs(double *widthDegs) const;

    SPxErrorCode SetViewWidthMetres(double widthMetres);

    SPxErrorCode GetViewWidthMetres(double *widthMetres) const;

    SPxErrorCode SetViewMinWidthMetres(double widthMetres);

    SPxErrorCode GetViewMinWidthMetres(double *widthMetres) const;

    SPxErrorCode SetViewMaxWidthMetres(double widthMetres);

    SPxErrorCode GetViewMaxWidthMetres(double *widthMetres) const;

    SPxErrorCode SetViewWidthMode(ViewWidthMode_t mode);

    SPxErrorCode GetViewWidthMode(ViewWidthMode_t *mode) const;

    SPxErrorCode SetWebMercatorMode(WebMercatorMode_t mode);

    SPxErrorCode GetWebMercatorMode(WebMercatorMode_t *mode) const;

    SPxErrorCode SetViewRectPixels(int xPixels, int yPixels, 
                                   unsigned int widthPixels,
                                   unsigned int heightPixels);

    SPxErrorCode GetViewPixelsPerMetre(double *ppm) const;

    SPxErrorCode GetViewExtentLatLong(double *northLatDegs,
                                      double *eastLongDegs,
                                      double *southLatDegs,
                                      double *westLongDegs,
                                      int corners,
                                      int normalise=TRUE) const;

    /* Automatic view centre and scale. */
    SPxErrorCode SetViewFitToRadars(SPxViewControlRangeFn_t fn, 
                                    void *userArg,
                                    int fitToWidth=TRUE,
                                    int fitToHeight=TRUE);

    /* Home view. */
    int IsHomeViewSet(void) const { return(m_homeViewWidthDegs > 0.0); }
    SPxErrorCode GotoHomeView(void);
    SPxErrorCode SetHomeViewCentreLatLong(const SPxLatLong_t *centreLL);
    SPxErrorCode GetHomeViewCentreLatLong(SPxLatLong_t *centreLL) const;
    SPxErrorCode SetHomeViewWidthDegs(double degs);
    SPxErrorCode GetHomeViewWidthDegs(double *degsPtr) const;

    /* Stored views. */
    SPxErrorCode GotoStoredView(unsigned int idx);
    SPxErrorCode SetStoredViewCentreLatLong(unsigned int idx,
						const SPxLatLong_t *centreLL);
    SPxErrorCode GetStoredViewCentreLatLong(unsigned int idx,
						SPxLatLong_t *centreLL) const;
    SPxErrorCode SetStoredViewWidthDegs(unsigned int idx, double degs);
    SPxErrorCode GetStoredViewWidthDegs(unsigned int idx,
						double *degsPtr) const;
    
    /* Panning and zooming. */
    SPxErrorCode PanView(double deltaLatDegs, 
                         double deltaLongDegs);

    SPxErrorCode PanViewPixels(int xDeltaPixels, 
                               int yDeltaPixels);

    SPxErrorCode PanViewMetres(double xDeltaMetres,
			       double yDeltaMetres);
    
    SPxErrorCode PanViewMetresDisplay(double xDeltaMetres,
				      double yDeltaMetres);

    SPxErrorCode ZoomViewAtLatLong(double factor, 
                                   const SPxLatLong_t *latLong);

    SPxErrorCode ZoomViewAtPixel(double factor, 
                                 int xPixels, int yPixels);

    SPxErrorCode ZoomView(double factor);
    
    /* Platforms. */
    SPxErrorCode SetNumPlatforms(unsigned int numPlatforms);

    SPxErrorCode SetPlatformNavData(SPxNavData *navData,
                                    unsigned int platformIndex=0);

    SPxErrorCode GetPlatformNavData(SPxNavData **navData,
                                    unsigned int platformIndex=0) const;

    SPxErrorCode SetPlatformLatLong(const SPxLatLong_t *latLong,
                                    unsigned int platformIndex=0);
    SPxErrorCode GetPlatformLatLong(SPxLatLong_t *latLong,
                                    unsigned int platformIndex=0) const;

    SPxErrorCode SetPlatformMagVarDegs(double degs, 
                                       unsigned int platformIndex=0);
    SPxErrorCode GetPlatformMagVarDegs(double *degs, 
                                       unsigned int platformIndex=0) const;

    SPxErrorCode SetPlatformHeadingDegs(double degs, 
                                        unsigned int platformIndex=0);
    SPxErrorCode GetPlatformHeadingDegs(double *degs, 
                                        unsigned int platformIndex=0) const;

    SPxErrorCode SetPlatformSpeedMps(double mps,
				     unsigned int platformIndex = 0);
    SPxErrorCode GetPlatformSpeedMps(double *mps,
				     unsigned int platformIndex = 0) const;

    SPxErrorCode SetPlatformCourseDegs(double degs,
				       unsigned int platformIndex = 0);
    SPxErrorCode GetPlatformCourseDegs(double *degs,
				       unsigned int platformIndex = 0) const;

    SPxErrorCode SetPlatformSpeedOverWaterMps(double mps,
					      unsigned int platformIndex = 0);
    SPxErrorCode GetPlatformSpeedOverWaterMps(double *mps,
					      unsigned int platformIndex = 0) const;

    SPxErrorCode SetPlatformCourseOverWaterDegs(double degs,
						unsigned int platformIndex = 0);
    SPxErrorCode GetPlatformCourseOverWaterDegs(double *degs,
						unsigned int platformIndex = 0) const;

    SPxErrorCode GetPlatformPositionMetres(double *xMetres, 
                                           double *yMetres,
                                           unsigned int platformIndex=0) const;

    SPxErrorCode GetPlatformMotionMetres(UINT32 msecs,
					 double *xMetres,
					 double *yMetres,
					 unsigned int platformIndex = 0,
					 int groundRel = TRUE) const;

    int IsPlatformOnScreen(unsigned int platformIndex=0) const;

    /* Radars. */
    SPxErrorCode GetRadarLatLong(SPxLatLong_t *latLong, 
                                 unsigned int radarIndex=0,
                                 unsigned int platformIndex=0) const;

    SPxErrorCode GetRadarPositionMetres(double *xMetres, 
                                        double *yMetres,
                                        unsigned int radarIndex=0,
                                        unsigned int platformIndex=0,
                                        int adjustForWebMercator=FALSE) const;

    /* Devices. */
    SPxErrorCode SetDeviceOffsetMetres(double xOffMetres,
                                       double yOffMetres,
                                       unsigned int deviceIndex=0,
                                       unsigned int platformIndex=0);

    SPxErrorCode GetDeviceLatLong(SPxLatLong_t *latLong, 
                                  unsigned int deviceIndex=0,
                                  unsigned int platformIndex=0) const;

    SPxErrorCode GetDevicePositionMetres(double *xMetres, 
                                         double *yMetres,
                                         unsigned int deviceIndex=0,
                                         unsigned int platformIndex=0) const;

    /* Display. */
    SPxErrorCode SetDisplayMotionMode(MotionMode_t mode);
    MotionMode_t GetDisplayMotionMode(void) const;

    SPxErrorCode SetDisplayRefMode(RefMode_t mode);
    RefMode_t GetDisplayRefMode(void) const;
    
    SPxErrorCode GetDisplayRotationDegs(double *degs) const;

    /* Update callbacks. */
    SPxErrorCode AddUpdateCallback(SPxCallbackListFn_t fn, 
                                   void *targetObj,
                                   unsigned int deviceIndex=0,
                                   unsigned int platformIndex=0);

    SPxErrorCode RemoveUpdateCallback(SPxCallbackListFn_t fn, 
                                      void *targetObj,
                                      unsigned int deviceIndex=0,
                                      unsigned int platformIndex=0);

    SPxErrorCode SetUpdateThreshPixels(unsigned int pixels);
    unsigned int GetUpdateThreshPixels(void) const;

    SPxErrorCode SetUpdateThreshDegs(double degs);
    double GetUpdateThreshDegs(void) const;

    SPxErrorCode SetBufferIntervalMs(UINT32 ms);
    UINT32 GetBufferIntervalMs(void) const;

    /* Periodic updates. */
    SPxErrorCode FlushChanges(void);
    SPxErrorCode PollNavData(void);
    SPxErrorCode PollWindowSize(void);

    /* Coordinate conversion. */
    SPxErrorCode MetresToLatLong(double xMetres, 
                                 double yMetres, 	
                                 SPxLatLong_t *latLong,
                                 int hideConvErr=FALSE,
				 int normalise=TRUE) const;

    SPxErrorCode LatLongToMetres(const SPxLatLong_t *latLong,
                                 double *xMetres, 
                                 double *yMetres,
                                 int hideConvErr=FALSE) const;

    SPxErrorCode MetresToPixels(double xMetres, 
                                double yMetres, 
                                int *xPixels, 
                                int *yPixels) const;

    SPxErrorCode PixelsToMetres(int xPixels, 
                                int yPixels,
                                double *xMetres, 
                                double *yMetres) const;

    SPxErrorCode LatLongToPixels(const SPxLatLong_t *latLong,
				 int *xPixels,
				 int *yPixels,
                                 int hideConvErr=FALSE) const;

    SPxErrorCode PixelsToLatLong(int xPixels,
				 int yPixels,
				 SPxLatLong_t *latLong,
                                 int hideConvErr=FALSE,
				 int normalise=TRUE) const;

    SPxErrorCode NorthToDisplay(double xNR, double yNR,
                                double *xDR, double *yDR) const;

    SPxErrorCode DisplayToNorth(double xDR, double yDR,
                                double *xNR, double *yNR) const;
        
    /* Change flags. */
    UINT32 GetChangeFlags(int clearFlags=FALSE);
    void ClearChangeFlags(void);

    /* Misc. */
    SPxErrorCode RequestRedraw(void);

    static void SetLogFile(FILE *logFile) { m_logFile = logFile; }
    static FILE *GetLogFile(void) { return( m_logFile ); }
    
protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Functions for testing. */
    virtual double GetLatLongInferSecs(const SPxTime_tag *lastLLTime,
				       SPxTime_tag *lastPlatInferTime);

    /* Parmeter setting/getting */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* Window. */
    unsigned int m_windowWidthPixels;   /* Window width in pixels. */
    unsigned int m_windowHeightPixels;  /* Window height in pixels. */
    SPxScFollow *m_follow;              /* Installed follow object. */

    /* View. */
    ViewWidthMode_t m_viewWidthMode;	/* View width mode. */
    SPxLatLong_t m_viewCentreLL;        /* View centre LL. */
    double m_viewWidthDegs;             /* View width in degrees. */
    double m_viewMinWidthDegs;          /* Minimum view width in degrees. */
    double m_viewMaxWidthDegs;          /* Maximum view width in degrees. */
    double m_viewMinWidthMetres;        /* Minimum view width in metres. */
    double m_viewMaxWidthMetres;        /* Maximum view width in metres. */
    double m_viewCXMetres;              /* View centre X metres (+ve east). */
    double m_viewCYMetres;              /* View centre Y metres (+ve north).*/
    double m_viewWidthMetres;           /* View width in metres. */
    double m_viewPixelsPerMetre;        /* View scale. */
    SPxLatLong_t m_refViewCentreLL;     /* Reference view centre LL. */
    double  m_refViewCXMetres;          /* Reference view centre X metres. */
    double  m_refViewCYMetres;          /* Reference view centre Y metres. */
    double m_refViewPixelsPerMetre;     /* Reference view scale. */
    int m_isViewAutoRecentreEnabled;    /* Auto recentre view on platform? */
    WebMercatorMode_t m_webMercatorMode;/* Web mercator conversion mode. */
    double m_maxViewOffsetPercent;	/* Max view offset as percent of view width. */

    /* Home view. */
    SPxLatLong_t m_homeViewCentreLL;
    double m_homeViewWidthDegs;

    /* Stored views. */
    SPxLatLong_t m_storedViewCentreLL[SPX_VC_MAX_STORED_VIEWS];
    double m_storedViewWidthDegs[SPX_VC_MAX_STORED_VIEWS];

    /* Platforms. */
    struct impl;
    SPxAutoPtr<impl> m_p;		/* Private structure - contains platforms. */
    SPxLatLong_t m_refPlatformLL;       /* Reference platform lat/long. */
    SPxLatLong_t m_prevPlatformLL;      /* Previous platform lat/long. */
    
    /* Display. */
    MotionMode_t m_displayMotionMode;   /* How plat motion is displayed. */
    RefMode_t m_displayRefMode;         /* How the display is orientated. */
    double m_displayRotationDegs;       /* Display rotation. */
    double m_refDisplayRotationDegs;    /* Reference display rotation. */
    double m_prevDisplayRotationDegs;   /* Previous display rotation. */

    /* Update callbacks. */
    SPxCallbackList *m_updateCBList;    /* Installed update callbacks. */
    unsigned int m_updateThreshPixels;  /* Window update threshold. */
    double m_updateThreshDegs;          /* Window rotation threshold. */
    CallbackInfo_t *m_cbInfoList;       /* Callback info linked list. */
    UINT32 m_lastUpdateTimeMs;          /* Last update time in ms. */
    UINT32 m_updateIntervalMs;          /* MS over which to buffer updates. */

    /* Change flags. */
    UINT32 m_pendingChangeFlags;        /* For use outside of update cb. */
    UINT32 m_bufferedChangeFlags;       /* Buffer change flags. */

    /*
     * Private functions.
     */
    SPxErrorCode setWindowSizePixels(unsigned int widthPixels,
				     unsigned int heightPixels);
    SPxErrorCode setViewWidthDegs(double widthDegs);
    SPxErrorCode setViewWidthMetres(double widthMetres);
    SPxErrorCode setViewWidthMetresFromDegs(void);
    SPxErrorCode setViewWidthDegsFromMetres(void);
    SPxErrorCode setViewCentreLatLong(const SPxLatLong_t *centreLL);
    SPxErrorCode setViewCentrePixels(int xPixels,
				     int yPixels);
    SPxErrorCode setViewCentreMetres(double cXMetres,
				     double cYMetres);
    SPxErrorCode getViewCentreMetres(double *cXMetres,
				     double *cYMetres) const;
    SPxErrorCode setViewCentreMetresDisplay(double cXMetres,
					    double cYMetres);
    SPxErrorCode getViewCentreMetresDisplay(double *cXMetres,
					    double *cYMetres) const;
    SPxErrorCode zoomViewAtPixel(double factor,
				 int xPixels, int yPixels);
    SPxErrorCode zoomView(double factor);
    SPxErrorCode setPlatformLatLong(const SPxLatLong_t *latLong,
				    unsigned int platformIndex = 0,
				    int expired = FALSE);
    SPxErrorCode getPlatformLatLong(SPxLatLong_t *latLong,
				    unsigned int platformIndex = 0) const;
    SPxErrorCode setPlatformMagVarDegs(double degs,
				       unsigned int platformIndex = 0);
    SPxErrorCode setPlatformHeadingDegs(double degs,
					unsigned int platformIndex = 0);
    SPxErrorCode getPlatformHeadingDegs(double *degs,
					unsigned int platformIndex = 0) const;
    SPxErrorCode setPlatformSpeedMps(double mps,
				     unsigned int platformIndex = 0);
    SPxErrorCode setPlatformCourseDegs(double degs,
				       unsigned int platformIndex = 0);
    SPxErrorCode setPlatformSpeedOverWaterMps(double mps,
					      unsigned int platformIndex = 0);
    SPxErrorCode setPlatformCourseOverWaterDegs(double degs,
						unsigned int platformIndex = 0);
    SPxErrorCode getPlatformPositionMetres(double *xMetres,
					   double *yMetres,
					   unsigned int platformIndex = 0,
					   int adjustForWebMercator = FALSE) const;
    int isPlatformOnScreen(unsigned int platformIndex = 0) const;
    SPxErrorCode getRadarPositionMetres(double *xMetres,
					double *yMetres,
					unsigned int radarIndex = 0,
					unsigned int platformIndex = 0,
					int adjustForWebMercator = FALSE) const;
    SPxErrorCode setDeviceOffsetMetres(double xOffMetres,
				       double yOffMetres,
				       unsigned int deviceIndex = 0,
				       unsigned int platformIndex = 0);
    SPxErrorCode getDeviceLatLong(SPxLatLong_t *latLong,
				  unsigned int deviceIndex = 0,
				  unsigned int platformIndex = 0) const;
    SPxErrorCode getDevicePositionMetres(double *xMetres,
					 double *yMetres,
					 unsigned int deviceIndex = 0,
					 unsigned int platformIndex = 0,
					 int adjustForWebMercator = FALSE) const;
    SPxErrorCode metresToLatLong(double xMetres,
				 double yMetres,
				 SPxLatLong_t *latLong,
				 int hideConvErr = FALSE,
				 int normalise = TRUE) const;
    SPxErrorCode latLongToMetres(const SPxLatLong_t *latLong,
				 double *xMetres,
				 double *yMetres,
				 int hideConvErr = FALSE) const;
    SPxErrorCode metresToPixels(double xMetres,
				double yMetres,
				int *xPixels,
				int *yPixels) const;
    SPxErrorCode pixelsToMetres(int xPixels,
				int yPixels,
				double *xMetres,
				double *yMetres) const;
    SPxErrorCode latLongToPixels(const SPxLatLong_t *latLong,
				 int *xPixels,
				 int *yPixels,
				 int hideConvErr = FALSE) const;
    SPxErrorCode pixelsToLatLong(int xPixels,
				 int yPixels,
				 SPxLatLong_t *latLong,
				 int hideConvErr = FALSE,
				 int normalise = TRUE) const;
    SPxErrorCode webMercatorLatLongToPixels(const SPxLatLong_t *latLong,
					    double *xPixelsRtn,
					    double *yPixelsRtn,
					    int applyRotation=TRUE) const;
    SPxErrorCode webMercatorPixelsToLatLong(double xPixels,
					    double yPixels,
					    SPxLatLong_t *latLongRtn,
					    int applyRotation=TRUE) const;
    unsigned int getNumPlatforms(void) const;
    SPxErrorCode checkAndCreatePlatform(unsigned int platformIndex);
    SPxErrorCode addPlatforms(unsigned int size);
    static double normaliseAngle(double degs);
    static void rotateXY(double degs, double x, double y, 
			 double *xRtn, double *yRtn);
    static double latAdd(double latDegs, double valDegs);
    static double longAdd(double longDegs, double valDegs);
    static int normaliseLatLong(SPxLatLong_t *latLong);
    SPxErrorCode update(UINT32 forcedChangeFlags=0);
    SPxErrorCode adjustMovingPlatformView(void);
    SPxErrorCode updateChangeFlags(UINT32 *changeFlags);
    SPxErrorCode getRadarExtent(unsigned int platformIndex,
                                unsigned int radarIndex,
                                double rangeMetres,
                                double *latNDegsRtn,
                                double *latSDegsRtn,
                                double *lonEDegsRtn, 
                                double *lonWDegsRtn);
    static void deviceUpdateFn(unsigned int deviceIndex,
	const SPxNavDataDeviceInfo *deviceInfo, void *userArg);
    static void deviceExtentFn(unsigned int deviceIndex, 
	const SPxNavDataDeviceInfo *deviceInfo, void *userArg);
    /*
     * Static (i.e. per class, not per object) variables.
     */
    static FILE *m_logFile;		/* Destination for debug messages */

    /*
     * Private static functions.
     */
    
}; /* SPxViewControl */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_VIEW_CONTROL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
