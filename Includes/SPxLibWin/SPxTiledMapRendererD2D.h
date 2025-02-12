/*********************************************************************
*
* (c) Copyright 2013 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTiledMapRendererD2D class which supports the display
*	of a tiled map in a Direct2D Windows application.
*
* Revision Control:
*   19/03/21 v1.8    AGC	Fix N/C #847 - display stops working when D3D device reset.
*
* Previous Changes:
*   25/07/19 1.7    AGC	Support tile caching.
*   01/07/19 1.6    AGC	Support transparency.
*   02/10/18 1.5    SP 	Support configurable tile size.
*   11/12/14 1.4    AGC	SetDestination() not longer requires factory.
*			Remove brightness/contrast adjustments.
*   02/06/14 1.3    SP 	Add isChanged to CheckBitmaps().
*   04/10/13 1.2    SP 	Support changes to the base class.
*   27/09/13 1.1    AGC	Initial version.
*
**********************************************************************/

#ifndef _SPX_TILED_MAP_RENDERER_D2D_H
#define _SPX_TILED_MAP_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxTiledMapRenderer.h"
#include "SPxLibWin/SPxRendererD2D.h"
#include "SPxLibWin/SPxBitmapWin.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/


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

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxTiledMapRenderer;
class SPxTerrainDatabase;
class SPxRendererD2D;
struct ID2D1Bitmap;
struct ID2D1BitmapRenderTarget;

/*
 * Define our class, derived from the base graphics renderer object
 * and also the WIN renderer to provide the rendering functions.
 */
class SPxTiledMapRendererD2D :  public SPxTiledMapRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTiledMapRendererD2D(SPxTiledMapDatabase *db, SPxTerrainDatabase *tdb=NULL);
    virtual ~SPxTiledMapRendererD2D(void);

    virtual SPxErrorCode SetDestination(ID2D1RenderTarget *target, int clearResources=TRUE);
    virtual SPxErrorCode SetDestination(ID2D1DeviceContext *target, int clearResources=TRUE);

    virtual void SetUseTransparency(int useTransparency);
    SPxErrorCode SetMaxCacheSize(unsigned int numItems);

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */

    SPxErrorCode CheckBitmaps(unsigned int mapWidthPixels,
                              unsigned int mapHeightPixels,
                              int *isChanged);

    SPxErrorCode ClearMapBitmap(unsigned int mapWidthPixels,
                                unsigned int mapHeightPixels);

    SPxErrorCode RenderTileToBitmap(const char *filename,
                                    unsigned tileSizePixels,
                                    int tileXPixels,
                                    int tileYPixels,
                                    int destXPixels,
                                    int destYPixels,
                                    double scale);

    SPxErrorCode RotateMap(unsigned int mapWidthPixels,
                           unsigned int mapHeightPixels);

    SPxErrorCode RenderMapToDestination(unsigned int mapWidthPixels,
                                        unsigned int mapHeightPixels,
                                        int mapViewCentreXPixels,
                                        int mapViewCentreYPixels,
                                        double scale);

}; /* SPxTiledMapRendererD2D */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_TILED_MAP_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
