/*********************************************************************
*
* (c) Copyright 2010, 2012, Cambridge Pixel Ltd.
*
* File:  $RCSfile: SPxWorldMapFeatureSet.h,v $
* ID: $Id: SPxWorldMapFeatureSet.h,v 1.4 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*	Header file for SPxWorldMapFeatureSet helper.
*
*
* Revision Control:
*   04/10/13 v1.4    AGC	Simplify headers.
*
* Previous Changes:
*   26/04/12 1.3    REW	Add m_errorDuringLoading.
*   16/11/10 1.2    REW	Add m_oldFeatures.
*   05/11/10 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_WORLDMAPFEATURESET_H
#define _SPX_WORLDMAPFEATURESET_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types. */
#include "SPxLibUtils/SPxObj.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Define the resolutions. Note that these are used as indices into an
 * array in the parent SPxWorldMap object, so take care if adding/changing.
 */
typedef enum {
    SPX_WORLD_MAP_RES_HIGH = 0,		/* High resolution */
    SPX_WORLD_MAP_RES_MED = 1,		/* Medium resolution */
    SPX_WORLD_MAP_RES_LOW = 2,		/* Low resolution */
    SPX_WORLD_MAP_RES_AUTOVIEW = 3	/* Auto based on view - must be last */
} SPxWorldMapRes_t;

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward-declare the helper class and other classes we need. */
class SPxWorldMap;
class SPxWorldMapFeature;

/*
 * Define the internal class used for keeping multiple feature sets.
 */
class SPxWorldMapFeatureSet :public SPxObj
{
    /* SPxWorldMap needs to be able to access internals directly. */
    friend class SPxWorldMap;

public:
    /*
     * Public functions.
     */
    /* Calculation helpers. */
    SPxErrorCode CheckMetresFromRefValid(void);
    SPxErrorCode CheckMetresFromRadarValid(void);
    SPxErrorCode CheckPixelsForViewValid(void);

protected:
    /* Protected functions, used by SPxWorldMap. */
    SPxWorldMapFeatureSet(void);
    virtual ~SPxWorldMapFeatureSet(void);

    /* Tidy up functions. */
    void deleteFeatureList(SPxWorldMapFeature *listHead);


    /*
     * Protected fields.
     */
    /* Main feature list. */
    SPxWorldMapFeature *m_featureList;	/* Active list of features */

    /* Old feature list pending deletion. */
    SPxWorldMapFeature *m_oldFeatures;	/* Waiting for deletion */

    /* Pre-caching feature list. */
    SPxWorldMapFeature *m_preCacheFirstFeature;	/* Start of pre-cache list */
    SPxWorldMapFeature *m_preCacheLastFeature;	/* End of pre-cache list */
    unsigned int m_preCachedFeatureCount;	/* Number of features cached */
    unsigned int m_preCacheTotalFeatureCount;	/* Total number of features */

    /* Useful info. */
    SPxWorldMap *m_parentMap;		/* Parent map */
    SPxWorldMapRes_t m_resolution;	/* Resolution */

    /* Filenames. */
    char *m_filenameMap;		/* Name of main map file */
    char *m_filenameRivers;		/* Name of rivers file */
    char *m_filenameBorders;		/* Name of borders file */

    /* Flags */
    int m_metresFromRefValid;		/* Are these coordinates valid? */
    int m_metresFromRadarValid;		/* Are these coordinates valid? */
    int m_pixelsValid;			/* Are these coordinates valid? */
    int m_errorDuringLoading;		/* Was there an error? */
}; /* SPxWorldMapFeatureSet */


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

#endif /* _SPX_WORLDMAPFEATURESET_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
