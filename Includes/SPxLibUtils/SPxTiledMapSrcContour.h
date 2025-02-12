/*********************************************************************
*
* (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTiledMapSrcContour object which generates
*	Tiled Map tiles with contour lines from terrain data.
*
*
* Revision Control:
*   02/10/18 v1.7    SP 	Replace SPxTiledMap with SPxTiledMapDatabase.
*
* Previous Changes:
*   02/06/14 1.6    AGC	Fix typo in GetIndicatorColour() name.
*   16/05/14 1.5    SP 	Support improvements to tiled map database
*			for custom and source object servers.
*   15/01/14 1.4    AGC	Add GetSrcName() function.
*   27/09/13 1.3    AGC	Add IsZoomLevelSupported() function.
*   05/09/13 1.2    AGC	Add GetIndicatorColour() function.
*   03/04/13 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_TILED_MAP_SRC_CONTOUR_H
#define _SPX_TILED_MAP_SRC_CONTOUR_H

/*
 * Other headers required.
 */

/* For SPxTiledMapDatabase enums. */
#include "SPxLibUtils/SPxTiledMapDatabase.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxTiledMapSrc.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxTerrainDatabase;
struct SPxTiledMapSrcContourPriv;

/* Define our class. */
class SPxTiledMapSrcContour : public SPxTiledMapSrc
{
public:
    /* Public functions. */
    explicit SPxTiledMapSrcContour(SPxTerrainDatabase *db);
    virtual ~SPxTiledMapSrcContour(void);

    virtual SPxTiledMapSrcType GetType(void) const { return SPX_TILED_MAP_SRC_TYPE_CONTOUR; }
    virtual const char *GetSrcName(void) const { return "Shuttle Radar Topography Mission Contours"; }
    virtual SPxTiledMapDatabase::TileImageFormat_t GetTileImageFormat(void) const;
    virtual int IsTileYInverted(void) const;
    virtual int IsZoomLevelSupported(int zoom) const;
    virtual SPxErrorCode GetTile(SPxTiledMapDatabase *tiledMap,
	int zoom, int tileX, int tileY, const char *filename);
    virtual UINT32 GetIndicatorColour(void) const { return 0xFF000000; }

private:
    /* Private variables. */
    SPxAutoPtr<SPxTiledMapSrcContourPriv> m_p;

}; /* SPxTiledMapSrcContour */

#endif /* _SPX_TILED_MAP_SRC_CONTOUR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
