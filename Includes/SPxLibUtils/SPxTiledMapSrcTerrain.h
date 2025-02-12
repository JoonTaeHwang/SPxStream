/*********************************************************************
*
* (c) Copyright 2012 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTiledMapSrcTerrain object which generates
*	Tiled Map tiles from terrain data.
*
*
* Revision Control:
*   02/10/18 v1.7    SP 	Replace SPxTiledMap with SPxTiledMapDatabase.
*
* Previous Changes:
*   08/07/16 1.6    AGC	Render key.
*   16/05/14 1.5    SP 	Support improvements to tiled map database
*			    for custom and source object servers.
*   15/01/14 1.4    AGC	Add GetSrcName() function.
*   27/09/13 1.3    AGC	Add IsZoomLevelSupported() function.
*   25/01/13 1.2    AGC	Add GetType() function.
*   09/11/12 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_TILED_MAP_SRC_TERRAIN_H
#define _SPX_TILED_MAP_SRC_TERRAIN_H

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
class SPxTiledMapRendererIface;

/* Define our class. */
class SPxTiledMapSrcTerrain : public SPxTiledMapSrc
{
public:
    /* Public functions. */
    explicit SPxTiledMapSrcTerrain(SPxTerrainDatabase *db);
    virtual ~SPxTiledMapSrcTerrain(void);

    virtual SPxTiledMapSrcType GetType(void) const { return SPX_TILED_MAP_SRC_TYPE_TERRAIN; }
    virtual const char *GetSrcName(void) const { return "Shuttle Radar Topography Mission Terrain"; }
    virtual SPxTiledMapDatabase::TileImageFormat_t GetTileImageFormat(void) const;
    virtual int IsTileYInverted(void) const;
    virtual int IsZoomLevelSupported(int zoom) const;
    virtual SPxErrorCode GetTile(SPxTiledMapDatabase *tiledMap,
	int zoom, int tileX, int tileY, const char *filename);
    virtual SPxErrorCode RenderKey(SPxTiledMapRenderer::Iface *renderer);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxTiledMapSrcTerrain */

#endif /* _SPX_TILED_MAP_SRC_TERRAIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
