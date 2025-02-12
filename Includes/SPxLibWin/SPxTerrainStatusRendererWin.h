/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxTerrainStatusRendererWin class which supports 
*   the display of loaded/available terrain areas for Windows GDI.
*
* Revision Control:
*   06/06/14 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef SPX_TERRAIN_STATUS_RENDERER_WIN_
#define SPX_TERRAIN_STATUS_RENDERER_WIN_

/* We need the base classes. */
#include "SPxLibUtils/SPxTerrainStatusRenderer.h"
#include "SPxLibWin/SPxRendererWin.h"

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxTerrainStatusRenderer;
class SPxRendererWin;
class SPxTerrainDatabase;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxTerrainStatusRendererWin : public SPxTerrainStatusRenderer, public SPxRendererWin
{
public:
    /* Constructor and destructor. */
    SPxTerrainStatusRendererWin(SPxTerrainDatabase *terrainDB);
    virtual ~SPxTerrainStatusRendererWin(void);

}; /* SPxTerrainStatusRendererD2D */

#endif /* SPX_TERRAIN_STATUS_RENDERER_WIN_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
