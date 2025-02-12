/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTiledMapSrc object which may be used as
*	an alternative source of tiled maps.
*
*
* Revision Control:
*  03/07/20 v1.13   AGC	    Add GetObjName() to allow overriding dir.
*
* Previous Changes:
*  02/10/18 1.12    SP 	Replace SPxTiledMap with SPxTiledMapDatabase.
*  08/07/16 1.11    AGC	Add RenderKey().
*  15/06/16 1.10    AGC	Support custom out of bounds/placeholder tiles.
*			Support tile expiration.
*  02/06/14 1.9     AGC	Fix typo in GetIndicatorColour() name.
*  16/05/14 1.8    SP 	Support improvements to tiled map database
*			for custom and source object servers.
*  01/05/14 1.7    SP 	Add IsTileExpireAllowed() function.
*  15/01/14 1.6    AGC	Add GetSrcName() function.
*  27/09/13 1.5    AGC	Add IsZoomLevelSupported() function.
*  05/09/13 1.4    AGC	Add GetIndicatorColour() function.
*  03/04/13 1.3    AGC	Add contour source.
*  25/01/13 1.2    AGC	Add GetType() function.
*  09/11/12 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_TILED_MAP_SRC_H
#define _SPX_TILED_MAP_SRC_H

/*
 * Other headers required.
 */

/* For SPxTiledMapDatabase enums. */
#include "SPxLibUtils/SPxTiledMapDatabase.h"

#include "SPxLibUtils/SPxTiledMapRenderer.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*
 * Types
 */

enum SPxTiledMapSrcType
{
    SPX_TILED_MAP_SRC_TYPE_NONE = 0,
    SPX_TILED_MAP_SRC_TYPE_TERRAIN = 1,
    SPX_TILED_MAP_SRC_TYPE_CONTOUR = 2,
    SPX_TILED_MAP_SRC_TYPE_COVERAGE = 3
};


/* Define our class. */
class SPxTiledMapSrc : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxTiledMapSrc(void);
    virtual ~SPxTiledMapSrc(void);

    virtual SPxTiledMapSrcType GetType(void) const=0;
    virtual const char *GetSrcName(void) const=0;
    virtual const char *GetObjName(void) const { return SPxObj::GetConstClassName(); }
    virtual SPxTiledMapDatabase::TileImageFormat_t GetTileImageFormat(void) const=0;
    virtual int IsTileYInverted(void) const=0;
    virtual int IsZoomLevelSupported(int /*zoom*/) const { return TRUE; }
    virtual int IsTileExpired(SPxTiledMapDatabase * /*tiledMap*/,
			      int /*zoom*/,
			      int /*tileX*/, int /*tileY*/) const { return FALSE; }
    virtual SPxErrorCode GetTile(SPxTiledMapDatabase *tiledMap,
				 int zoom, int x, int y,
				 const char *filename)=0;
    virtual UINT32 GetIndicatorColour(void) const { return 0xFFFFFFFF; }
    virtual unsigned int GetOutOfBoundsTileSize(void) const { return 0; }
    virtual const unsigned char *GetOutOfBoundsTileData(void) const { return NULL; }
    virtual unsigned int GetPlaceholderTileSize(void) const { return 0; }
    virtual const unsigned char *GetPlaceholderTileData(void) const { return NULL; }
    virtual SPxErrorCode RenderKey(SPxTiledMapRenderer::Iface * /*renderer*/) { return SPX_NO_ERROR; }

}; /* SPxTiledMapSrc */

#endif /* _SPX_TILED_MAP_SRC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
