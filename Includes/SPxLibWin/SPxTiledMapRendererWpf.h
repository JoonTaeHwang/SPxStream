/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTiledMapRendererWpf class which supports the display
*	of map tiles in a WPF-based application.
*
* Revision Control:
*   27/01/20 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef SPX_TILED_MAP_RENDERER_WPF_H
#define SPX_TILED_MAP_RENDERER_WPF_H

/*
 * Other headers required.
 */

 /* We need the base classes. */
#include "SPxLibUtils/SPxTiledMapRenderer.h"
#include "SPxLibWin/SPxRendererWpf.h"


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
class SPxRendererWpf;
class SPxTiledMapDatabase;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxTiledMapRendererWpf : public SPxTiledMapRenderer, public SPxRendererWpf
{
public:
    typedef SPxErrorCode(*ClearMapBitmapFn)(unsigned int mapWidthPixels,
                                            unsigned int mapHeightPixels);
    typedef SPxErrorCode(*RenderTileToBitmapFn)(const char *filename,
                                                unsigned int tileSizePixels,
                                                int srcXPixels,
                                                int srcYPixels,
                                                int destXPixels,
                                                int destYPixels,
                                                double scale);
    typedef SPxErrorCode(*RotateMapFn)(unsigned int mapWidthPixels,
                                       unsigned int mapHeightPixels);
    typedef SPxErrorCode(*RenderMapToDestinationFn)(unsigned int mapWidthPixels,
                                                    unsigned int mapHeightPixels,
                                                    int mapViewCentreXPixels,
                                                    int mapViewCentreYPixels,
                                                    double scale);

    /* Constructor and destructor. */
    explicit SPxTiledMapRendererWpf(SPxTiledMapDatabase *tiledMapDB);
    virtual ~SPxTiledMapRendererWpf(void);

    void InstallClearMapBitmapFn(ClearMapBitmapFn clearMapBitmapFn);
    void InstallRenderTileToBitmapFn(RenderTileToBitmapFn renderTileToBitmapFn);
    void InstallRotateMapFn(RotateMapFn rotateMapFn);
    void InstallRenderMapToDestinationFn(RenderMapToDestinationFn renderMapToDestinationFn);

private:
    virtual SPxErrorCode CheckBitmaps(unsigned int mapWidthPixels,
                                      unsigned int mapHeightPixels,
                                      int *isChanged);

    virtual SPxErrorCode ClearMapBitmap(unsigned int mapWidthPixels,
                                        unsigned int mapHeightPixels);

    virtual SPxErrorCode RenderTileToBitmap(const char *filename,
                                            unsigned int tileSizePixels,
                                            int srcXPixels,
                                            int srcYPixels,
                                            int destXPixels,
                                            int destYPixels,
                                            double scale);

    virtual SPxErrorCode RotateMap(unsigned int mapWidthPixels,
                                   unsigned int mapHeightPixels);

    virtual SPxErrorCode RenderMapToDestination(unsigned int mapWidthPixels,
                                                unsigned int mapHeightPixels,
                                                int mapViewCentreXPixels,
                                                int mapViewCentreYPixels,
                                                double scale);

    ClearMapBitmapFn m_clearMapBitmapFn;
    RenderTileToBitmapFn m_renderTileToBitmapFn;
    RotateMapFn m_rotateMapFn;
    RenderMapToDestinationFn m_renderMapToDestinationFn;

}; /* SPxTiledMapRendererWpf */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TILED_MAP_RENDERER_WPF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
