/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxTerrainStatusRendererD2D class which supports 
*   the display of loaded/available terrain areas for Windows D2D.
*
* Revision Control:
*   06/06/14 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef SPX_TERRAIN_STATUS_RENDERER_D2D_
#define SPX_TERRAIN_STATUS_RENDERER_D2D_

/* We need the base classes. */
#include "SPxLibUtils/SPxTerrainStatusRenderer.h"
#include "SPxLibWin/SPxRendererD2D.h"

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxTerrainStatusRenderer;
class SPxRendererD2D;
class SPxTerrainDatabase;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxTerrainStatusRendererD2D : public SPxTerrainStatusRenderer, public SPxRendererD2D
{
public:
    /* Constructor and destructor. */
    SPxTerrainStatusRendererD2D(SPxTerrainDatabase *terrainDB);
    virtual ~SPxTerrainStatusRendererD2D(void);

}; /* SPxTerrainStatusRendererD2D */

#endif /* SPX_TERRAIN_STATUS_RENDERER_D2D_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
