/*********************************************************************
*
* (c) Copyright 2010 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxWorldMapFeature functionality.
*
*
* Revision Control:
*   12/02/20 v1.10   AGC	Simplify and improve last change.
*
* Previous Changes:
*   10/02/20 1.9    AGC	Fix issues caused by lat/long and pixel
*			bounding box intersection.
*   04/12/19 1.8    AGC	Improve calculation of pixel points.
*   15/03/17 1.7    AGC	Skip pixel points where appropriate.
*   04/10/13 1.6    AGC	Simplify headers.
*   05/11/10 1.5    REW	Rework for multiple resolutions.
*   28/10/10 1.4    REW	Add bounding box to constructor.
*   27/10/10 1.3    REW	Add colour support and low-res non-UTM mapping.
*   14/10/10 1.2    REW	Add clipForCachedArea()
*   11/10/10 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_WORLDMAPFEATURE_H
#define _SPX_WORLDMAPFEATURE_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Types of geographical features. */
typedef enum {
    SPX_WORLD_MAP_FEATURE_TYPE_UNKNOWN = 0,	/* Not set */
    SPX_WORLD_MAP_FEATURE_TYPE_LAND = 1,	/* Land */
    SPX_WORLD_MAP_FEATURE_TYPE_LAKE = 2,	/* Lake */
    SPX_WORLD_MAP_FEATURE_TYPE_ISLAND = 3,	/* Island in lake */
    SPX_WORLD_MAP_FEATURE_TYPE_POND = 4,	/* Pond in island */
    SPX_WORLD_MAP_FEATURE_TYPE_RIVER = 5,	/* River */
    SPX_WORLD_MAP_FEATURE_TYPE_BORDER = 6	/* Border */
} SPxWorldMapFeatureType_t;

/* Coordinate pairs. This must be scalar, so that "=" works for copying.  */
typedef struct SPxWorldMapLatLong_tag {
    double latDegs;
    double longDegs;
} SPxWorldMapLatLong_t;

typedef struct SPxWorldMapXY_tag {
    REAL32 x;
    REAL32 y;
} SPxWorldMapXY_t;


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declare the world map etc. */
class SPxWorldMap;
class SPxWorldMapFeatureSet;

/*
 * Define the class used to store individual geographical features.
 */
class SPxWorldMapFeature :public SPxObj
{
    /* SPxWorldMap needs to be able to access some of the internals directly,
     * specifically the constructor/destructor. Same for the feature set.
     */
    friend class SPxWorldMap;
    friend class SPxWorldMapFeatureSet;

public:
    /*
     * Public functions.
     */
    /* NB: The constructor/destructor are not public, because these
     * objects can only be created/destroyed by the SPxWorldMap itself.
     */

    /* Access to parent map. */
    SPxWorldMap *GetParentMap(void)	{ return(m_parentMap); }

    /* Access to coordinate arrays. */
    unsigned int GetNumPoints(void) const { return(m_numPoints); }
    const SPxWorldMapXY_t *GetMetresFromRef(void);
    const SPxWorldMapXY_t *GetMetresFromRadar(void);

    unsigned int GetNumPixelPoints(void) const;
    const SPxWorldMapXY_t *GetPixelsForView(void);

    /* Access to types etc. */
    SPxWorldMapFeatureType_t GetType(void) const { return(m_type); }
    int GetCrossesGreenwich(void) const		{ return(m_crossesGreenwich); }

    /* Access to bounding box. */
    double GetBboxLatSouth() const 	{ return(m_bboxSouthLat); }
    double GetBboxLatNorth() const	{ return(m_bboxNorthLat); }
    double GetBboxLongWest() const	{ return(m_bboxWestLong); }
    double GetBboxLongEast() const	{ return(m_bboxEastLong); }

    /* Suggested colours. */
    UINT32 GetColourFill(void) const;
    UINT32 GetColourLine(void) const;

protected:
    /*
     * Protected functions, likely to be used by SPxWorldMap.
     */
    /* Construction/destruction only done by friend SPxWorldMap class. */
    SPxWorldMapFeature(SPxWorldMap *parentMap, SPxWorldMapFeatureSet *set,
			unsigned int maxPoints,
			REAL32 southLatDegs, REAL32 northLatDegs,
			REAL32 westLongDegs, REAL32 eastLongDegs);
    virtual ~SPxWorldMapFeature(void);

    /* Setup information. */
    void setType(SPxWorldMapFeatureType_t type)	{ m_type = type; }
    void setCrossesGreenwich(int flag)		{ m_crossesGreenwich = flag; }

    /* Clipping. */
    SPxErrorCode clipForCachedArea(double southLat, double northLat,
					double westLong, double eastLong);

    /* Access to coordinate arrays. */
    SPxWorldMapLatLong_t *getLatLongs(void) { return(m_latLongArray); }

    /* Calculation functions. */
    SPxErrorCode calculateMetresFromRef(double latDegs, double longDegs,
					int useUTMcalcs = TRUE);
    SPxErrorCode calculateMetresFromRadar(double radarXMetres,
					double radarYMetres);
    SPxErrorCode calculatePixelsForView(REAL32 xMetres, REAL32 yMetres,
					unsigned int widthPixels,
					unsigned int heightPixels,
					REAL32 pixelsPerMetre);

    /* Expensive look-up functions. */
    int isLatLongInsideFeature(double latDegs, double longDegs);
    double getSquaredMetresToEdge(double xMetres, double yMetres);

    /*
     * Protected variables, likely to be used by SPxWorldMap.
     */
    /* Linked list pointers. */
    SPxWorldMapFeature *m_next;


private:
    /*
     * Private functions.
     */
    UINT8 comparePointToClipArea(const SPxWorldMapLatLong_t *pt);
    UINT8 comparePointToWinArea(const SPxWorldMapXY_t *pt,
                                REAL32 width, REAL32 height);

    /*
     * Private variables.
     */
    /* Parent map and feature-set. */
    SPxWorldMap *m_parentMap;			/* Parent map */
    SPxWorldMapFeatureSet *m_parentSet;		/* Parent feature set */

    /* Control information. */
    SPxWorldMapFeatureType_t m_type;		/* Type of feature */
    int m_crossesGreenwich;			/* Does it straddle 0degs? */

    /* Points. */
    unsigned int m_maxPoints;			/* Maximum points at startup */
    unsigned int m_numPoints;			/* Actual number of points */
    SPxWorldMapLatLong_t *m_latLongArray;	/* Array of lat/longs */

    /* Convenience arrays of coordinates. */
    SPxWorldMapXY_t *m_xyMetresFromRef;		/* Array of X/Y metres */
    SPxWorldMapXY_t *m_xyMetresFromRadar;	/* Array of X/Y metres */

    unsigned int m_numPixelPoints;		/* Number of pixel points */
    SPxWorldMapXY_t *m_xyPixelsForView;		/* Array of X/Y pixels */
    unsigned int m_capPixelPoints;		/* Capacity of X/Y pixels array */

    /* Clip region. */
    double m_clipSouthLat;			/* Clip limit */
    double m_clipNorthLat;			/* Clip limit */
    double m_clipWestLong;			/* Clip limit */
    double m_clipEastLong;			/* Clip limit */

    /* Bounding box. */
    double m_bboxSouthLat;			/* Area limit */
    double m_bboxNorthLat;			/* Area limit */
    double m_bboxWestLong;			/* Area limit */
    double m_bboxEastLong;			/* Area limit */
}; /* SPxWorldMapFeature */


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

#endif /* _SPX_WORLDMAPFEATURE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
