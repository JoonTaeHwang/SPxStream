/*********************************************************************
*
* (c) Copyright 2010 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxWorldMap functionality.
*
*
* Revision Control:
*   24/04/18 v1.23   AGC	Add optional navData arg to ConvRadarMetresToLL().
*
* Previous Changes:
*   15/03/17 1.22   AGC	Use const for GetColourFill/Line() args.
*   07/10/16 1.21   AGC	Use atomic variable for pre cache in background flag.
*   29/09/16 1.20   REW	Add CheckMapFiles().
*   02/09/16 1.19   AGC	Use atomic variable for pre cache in progress flag.
*   16/05/14 1.18   SP 	Allow object to disabled.
*   21/10/13 1.17   SP 	Add SPxNavData arg to SPxGetRadarLatLong().
*   04/10/13 1.16   AGC	Simplify headers.
*   15/07/13 1.15   AGC	Replace GetCacheLat/Long...() functions
*			with GetCacheArea() function.
*			Add IsLLInCacheArea() function.
*   06/06/13 1.14   AGC	Add GetAutoResolution() function.
*			Add GetCacheLat/Long...() functions.
*			Add GetRefLat/Long...() functions.
*   12/02/13 1.13   AGC	Fix #228: Support -1 as radar index for 
*			ConvRadarMetresToLL().
*   11/01/13 1.12   AGC	Add optional radar index to ConvRadarMetresToLL().
*   21/12/12 1.11   AGC	Add SPxGetPlatformLatLong().
*   13/07/12 1.10   AGC	Add radar index to SPxGetRadarLatLong().
*			Add lat/long callback support.
*   23/04/12 1.9    REW	Support display rotations.
*   08/02/12 1.8    AGC	Add SPxGetRadarLatLong() function.
*   08/02/11 1.7    REW	Support generic rotation.
*   04/02/11 1.6    SP 	Add SetRadarRadius() and GetRadarRadius().
*   19/11/10 1.5    REW	Support magnetic variation.
*   05/11/10 1.4    REW	Rework for multiple resolutions and moving platforms.
*   28/10/10 1.3    REW	Add land/sea and coastal proximity support.
*   27/10/10 1.2    REW	Add LoadActiveRegion(), colours and low-res map
*   11/10/10 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_WORLDMAP_H
#define _SPX_WORLDMAP_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types. */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/* We need the world map feature support. */
#include "SPxLibUtils/SPxWorldMapFeatureSet.h"
#include "SPxLibUtils/SPxWorldMapFeature.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Debug flags. */
#define	SPX_WORLD_MAP_DEBUG_RADAR	0x00000001	/* Radar updates */
#define	SPX_WORLD_MAP_DEBUG_VIEW	0x00000002	/* View updates */
#define	SPX_WORLD_MAP_DEBUG_ITERATE	0x00000010	/* Iterations */
#define	SPX_WORLD_MAP_DEBUG_REGION	0x00000100	/* LoadRegion */
#define	SPX_WORLD_MAP_DEBUG_IS_LAND	0x00000200	/* IsOnLand */
#define	SPX_WORLD_MAP_DEBUG_DISTANCE	0x00000400	/* GetDistanceToCoast*/
#define	SPX_WORLD_MAP_DEBUG_PRECACHE	0x00010000	/* Pre-caching */
#define	SPX_WORLD_MAP_DEBUG_FILES	0x00020000	/* File access */
#define	SPX_WORLD_MAP_DEBUG_REFMETRES	0x01000000	/* RefMetres */
#define	SPX_WORLD_MAP_DEBUG_RADARMETRES	0x02000000	/* RadarMetres */
#define	SPX_WORLD_MAP_DEBUG_VIEWPIXELS	0x04000000	/* PixelsForView */
#define	SPX_WORLD_MAP_DEBUG_CHECKS	0x08000000	/* Above three checks*/
#define	SPX_WORLD_MAP_DEBUG_VERBOSE	0x80000000	/* Verbose */


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward-declare the helper class and other classes we need. */
class SPxWorldMap;
class SPxWorldMapFeatureSet;
class SPxWorldMapFeature;
class SPxActiveRegion;
class SPxNavData;
class SPxThread;

/* Define the type of iterator functions. */
typedef void (*SPxWorldMapIteratorFn_t)(SPxWorldMapFeature *feature,
					void *userArg);

/* Define the type of lat/long callback functions. */
typedef void (*SPxWorldMapLatLongFn_t)(double latDegs, double longDegs,
				       void *userArg);

/*
 * Define the main class for the map as a whole.
 */
class SPxWorldMap :public SPxObj
{
    /* The feature set needs to access some of our internals. */
    friend class SPxWorldMapFeatureSet;

public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxWorldMap(int isActive=TRUE);
    virtual ~SPxWorldMap(void);

    /* Filename control. */
    SPxErrorCode SetFilenameMap(const char *name, SPxWorldMapRes_t res);
    SPxErrorCode SetFilenameRivers(const char *name, SPxWorldMapRes_t res);
    SPxErrorCode SetFilenameBorders(const char *name, SPxWorldMapRes_t res);
    const char *GetFilenameMap(SPxWorldMapRes_t res);
    const char *GetFilenameRivers(SPxWorldMapRes_t res);
    const char *GetFilenameBorders(SPxWorldMapRes_t res);

    /* Utility function to see if all the files can be found. */
    SPxErrorCode CheckMapFiles(void);

    /* Control of area around radar. */
    SPxErrorCode SetNavDataLink(SPxNavData *navData);
    SPxErrorCode SetRadarLatLong(double latDegs, double longDegs);
    double GetRadarLatDegs(void)		{ return(m_radarLatDegs); }
    double GetRadarLongDegs(void)		{ return(m_radarLongDegs); }
    SPxErrorCode SetRadarRadius(double radiusMetres);
    double GetRadarRadius(void)                  { return(m_radarRadius); }
    SPxErrorCode SetLatLongCallback(SPxWorldMapLatLongFn_t fn, void *userArg);

    /* Magnetic variation at the radar position. */
    SPxErrorCode SetRadarMagneticVariation(double degs);
    double GetRadarMagneticVariationDegs(void)	{ return(m_radarMagVarDegs); }
    double GetRadarMagneticVariationSin(void)	{ return(m_radarMagVarSin); }
    double GetRadarMagneticVariationCos(void)	{ return(m_radarMagVarCos); }
    SPxErrorCode SetUseMagVar(int use);
    int GetUseMagVar(void)			{ return(m_useMagVar); }

    /* User-selectable Rotation. Applied to both processing and display. */
    SPxErrorCode SetRotationDegs(double degs);
    double GetRotationDegs(void)		{ return(m_rotationDegs); }
    double GetRotationSin(void)			{ return(m_rotationSin); }
    double GetRotationCos(void)			{ return(m_rotationCos); }

    /* Display Rotation. Does not affect processing. */
    SPxErrorCode SetDisplayRotationDegs(double degs);
    double GetDisplayRotationDegs(void)	{ return(m_displayRotationDegs); }
    double GetDisplayRotationSin(void)	{ return(m_displayRotationSin); }
    double GetDisplayRotationCos(void)	{ return(m_displayRotationCos); }

    /* Total pre-display rotation, composed of magnetic variation (if enabled)
     * and user-selectable rotation, but not display rotation.
     */
    double GetTotalPreDisplayRotationDegs(void)
				{ return(m_totalPreDisplayRotationDegs);}
    double GetTotalPreDisplayRotationSin(void)
				{ return(m_totalPreDisplayRotationSin); }
    double GetTotalPreDisplayRotationCos(void)
				{ return(m_totalPreDisplayRotationCos); }

    /* Total rotation, composed of magnetic variation (if enabled),
     * user-selectable rotation and display rotation.
     */
    double GetTotalRotationDegs(void)		{ return(m_totalRotationDegs);}
    double GetTotalRotationSin(void)		{ return(m_totalRotationSin); }
    double GetTotalRotationCos(void)		{ return(m_totalRotationCos); }

    /* View control. */
    SPxErrorCode SetViewCentreMetres(double xMetresFromRadar,
					double yMetresFromRadar);
    SPxErrorCode SetViewSizePixels(unsigned int widthPixels,
					unsigned int heightPixels);
    SPxErrorCode SetViewPixelsPerMetre(double pixelsPerMetre);
    double GetViewCentreXMetres(void)	{ return(m_viewXMetresFromRadar); }
    double GetViewCentreYMetres(void)	{ return(m_viewYMetresFromRadar); }
    unsigned int GetViewWidthPixels(void) { return(m_viewWidthPixels); }
    unsigned int GetViewHeightPixels(void) { return(m_viewHeightPixels); }
    double GetViewPixelsPerMetre(void)	{ return(m_viewPixelsPerMetre); }

    /* Iterators. */
    SPxErrorCode IterateFeatures(SPxWorldMapIteratorFn_t fn, void *userArg,
				SPxWorldMapRes_t res = SPX_WORLD_MAP_RES_HIGH);

    /* Other functions to use the area. */
    SPxErrorCode LoadActiveRegion(SPxActiveRegion *region,
				SPxWorldMapRes_t res = SPX_WORLD_MAP_RES_HIGH);

    /* Coastal proximity functions. */
    int IsOnLand(const struct SPxWorldMapLatLong_tag *latLong,
				double *distanceMetresPtr=NULL,
				SPxWorldMapRes_t res = SPX_WORLD_MAP_RES_HIGH);
    double GetDistanceToCoast(const struct SPxWorldMapLatLong_tag *latLong,
				SPxWorldMapFeature *feature=NULL,
				SPxWorldMapRes_t res = SPX_WORLD_MAP_RES_HIGH);
    /* Consider adding IsDistanceToCoastLessThanX() */

    /* Coordinate conversion helpers. */
    SPxErrorCode ConvLLToRadarMetres(
				const struct SPxWorldMapLatLong_tag *latLong,
				double *xMetresPtr, double *yMetresPtr);
    SPxErrorCode ConvRadarMetresToLL(double xMetres, double yMetres,
				struct SPxWorldMapLatLong_tag *latLongPtr,
				int radarIndex=0, SPxNavData *navData=NULL);

    /* Colour management. */
    UINT32 GetColourFill(const SPxWorldMapFeature *feature) const;
    UINT32 GetColourLine(const SPxWorldMapFeature *feature) const;

    /* Auto-resolution. */
    SPxWorldMapRes_t GetAutoResolution(void);
    SPxErrorCode GetCacheArea(double *nwX, double *nwY,
			      double *neX, double *neY,
			      double *seX, double *seY,
			      double *swX, double *swY,
			      int applyRotation=TRUE);
    int IsLLInCacheArea(double latDegs, double longDegs);

    /* Debug. */
    void SetDebug(UINT32 debug)		{ m_debug = debug; }
    UINT32 GetDebug(void)		{ return(m_debug); }

    /* Generic parameter control. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /*
     * Static functions for class.
     */

protected:
    /* Protected functions, which we expect to be used by the friend class
     * (noting that being protected rather than private doesn't give them
     * that right, but is a way of us trying to indicate which of the
     * internal features will be accessed by friends and which won't).
     */
    void getRefLatLong(double *latDegsPtr, double *longDegsPtr)
    {
	if( latDegsPtr != NULL )	{ *latDegsPtr = m_refLatDegs; }
	if( longDegsPtr != NULL )	{ *longDegsPtr = m_refLongDegs; }
    }
    void getRadarXYFromRef(double *xMetresPtr, double *yMetresPtr)
    {
	if( xMetresPtr != NULL )	{ *xMetresPtr = m_radarXMetresFromRef;}
	if( yMetresPtr != NULL )	{ *yMetresPtr = m_radarYMetresFromRef;}
    }

private:
    /*
     * Private functions.
     */
    SPxWorldMapFeatureSet *getFeatureSet(SPxWorldMapRes_t res);

    /* Rotation assistance. */
    SPxErrorCode updateTotalRotation(void);

    /* Nav data callback. */
    SPxErrorCode navDataCallback(const char *nmea);

    /* Cache-related functions. */
    SPxErrorCode checkCachedArea(void);
    SPxErrorCode startPreCache(double latDegs, double longDegs,
					double radiusMetres);
    void *preCacheThreadFn(SPxThread *thread);
    SPxErrorCode readAndPreCacheArea(double latDegs, double longDegs,
					double radiusMetres);
    SPxErrorCode readAndPreCacheAreaFromFile(SPxWorldMapFeatureSet *featureSet,
					char *filename,
					SPxWorldMapFeatureType_t type,
					double clipLatSouth,
					double clipLatNorth,
					double clipLongWest,
					double clipLongEast);

    /*
     * Private variables.
     */
    SPxCriticalSection m_mutex;			/* Safety mutex */
    UINT32 m_debug;				/* Debug flags */
    FILE *m_logFile;				/* Log file */

    /* Varying resolution feature sets. */
    SPxWorldMapFeatureSet m_featureSetHigh;	/* High res features */
    SPxWorldMapFeatureSet m_featureSetMed;	/* Medium res features */
    SPxWorldMapFeatureSet m_featureSetLow;	/* Low res features */

    /* Array of pointers to the above sets for convenient looping. */
    SPxWorldMapFeatureSet *m_featureSets[3];
    unsigned int m_numFeatureSets;	/* Length of array */

    /* Reference point. */
    double m_refLatDegs;		/* Latitude of reference point */
    double m_refLongDegs;		/* Longitude of reference point */

    /* Radar information. */
    double m_radarRadius;		/* Radius in metres */
    double m_radarLatDegs;		/* Latitude of radar, +ve north */
    double m_radarLongDegs;		/* Longitude of radar, +ve east */
    double m_radarMagVarDegs;		/* Magnetic variation at radar */
    double m_radarMagVarSin;		/* Sine of angle */
    double m_radarMagVarCos;		/* Cosine of angle */
    int m_useMagVar;			/* Flag to say use MagVarDegs */
    double m_radarXMetresFromRef;	/* Distance of radar from reference */
    double m_radarYMetresFromRef;	/* Distance of radar from reference */

    /* Lat/long callback info. */
    SPxWorldMapLatLongFn_t m_latLongFn;	/* Lat/long callback function. */
    void *m_latLongFnArg;		/* User argument for lat/long callback. */

    /* Rotations. */
    double m_rotationDegs;		/* User rotation in degrees */
    double m_rotationSin;		/* Sine of angle */
    double m_rotationCos;		/* Cosine of angle */
    double m_displayRotationDegs;	/* Display rotation in degrees */
    double m_displayRotationSin;	/* Sine of angle */
    double m_displayRotationCos;	/* Cosine of angle */
    double m_totalPreDisplayRotationDegs; /* Total rotation excluding display*/
    double m_totalPreDisplayRotationSin; /* Sine of angle */
    double m_totalPreDisplayRotationCos; /* Cosine of angle */
    double m_totalRotationDegs;		/* Total rotation */
    double m_totalRotationSin;		/* Sine of angle */
    double m_totalRotationCos;		/* Cosine of angle */

    /* Navigation data linkage. */
    SPxNavData *m_navData;		/* Pointer to nav data object */

    /* View information. */
    double m_viewXMetresFromRadar;	/* X offset to centre of window */
    double m_viewYMetresFromRadar;	/* Y offset to centre of window */
    unsigned int m_viewWidthPixels;	/* Width of window */
    unsigned int m_viewHeightPixels;	/* Height of window */
    double m_viewPixelsPerMetre;	/* Scaling factor */

    /* Cached features. */
    double m_cachedRadius;		/* Metres from reference point */
    double m_cacheLatSouth;		/* Southern latitude */
    double m_cacheLatNorth;		/* Northern latitude */
    double m_cacheLongWest;		/* Western longitude */
    double m_cacheLongEast;		/* Eastern longitude */

    /* Control over pre-caching. */
    SPxAtomic<int> m_preCacheInProgress;/* Is one in progress? */
    SPxThread *m_preCacheThread;	/* Background thread */
    SPxAtomic<int> m_preCacheInBackground; /* Run in background or not? */
    double m_preCacheLatDegs;		/* New reference point */
    double m_preCacheLongDegs;		/* New reference point */
    double m_preCacheRadius;		/* Metres */
    double m_preCacheLatSouth;		/* Southern latitude */
    double m_preCacheLatNorth;		/* Northern latitude */
    double m_preCacheLongWest;		/* Western longitude */
    double m_preCacheLongEast;		/* Eastern longitude */

    /* Colours. */
    UINT32 m_colourFillLand;
    UINT32 m_colourFillLake;
    UINT32 m_colourFillIsland;
    UINT32 m_colourFillPond;
    UINT32 m_colourLineLand;
    UINT32 m_colourLineLake;
    UINT32 m_colourLineIsland;
    UINT32 m_colourLinePond;
    UINT32 m_colourLineBorder;
    UINT32 m_colourLineRiver;

    /*
     * Static variables for class.
     */

    /*
     * Static private functions for class.
     */
    static void iteratorLoadActiveRegion(SPxWorldMapFeature *feature,
						void *region);
    static void iteratorIsOnLand(SPxWorldMapFeature *feature,
						void *arg);
    static void iteratorDistanceToCoast(SPxWorldMapFeature *feature,
						void *arg);
    static void *staticPreCacheThreadFn(SPxThread *thread);
    static int staticNavDataCallback(void *invokingObj, void *userObj,
						void *arg);
}; /* SPxWorldMap */


/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Get the radar/platform lat/long from a specified nav data object, 
 * the global navdata or a world map object. 
 */
SPxErrorCode SPxGetPlatformLatLong(SPxWorldMap *worldMap,
				   double *platLatDegs,
				   double *platLongDegs,
                                   SPxNavData *navData=NULL);
SPxErrorCode SPxGetRadarLatLong(SPxWorldMap *worldMap,
				double *radarLatDegs,
				double *radarLongDegs,
				unsigned int radarIndex=0,
                                SPxNavData *navData=NULL);

#endif /* _SPX_WORLDMAP_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
