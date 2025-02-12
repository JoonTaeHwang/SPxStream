/*********************************************************************
*
* (c) Copyright 2012 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTiledMapRendererWin class which supports the display
*	of a tiled map in a Windows application.
*
* Revision Control:
*   23/05/19 v1.13   SP 	Fix N/C #695 - rotated semi-transparent maps not rendered.
*
* Previous Changes:
*   02/10/18 1.12   SP 	Support configurable tile size.
*   02/02/18 1.11   AGC	Support transparency.
*   28/07/16 1.10   AGC	Brightness/contrast functions now virtual.
*   02/06/14 1.9    SP 	Add isChanged to CheckBitmaps().
*   06/12/13 1.8    AGC	Add SetBrightness/SetContrast functions.
*   04/10/13 1.7    AGC	Simplify headers.
*   04/10/13 1.6    SP 	Support display rotation.
*   05/09/13 1.5    AGC	Optional display of terrain download indicator.
*   21/08/12 1.4    SP 	Move all common code to base class.
*   18/07/11 1.3    SP 	Remove Render() and add RenderMap().
*   13/07/11 1.2    SP 	Add RenderTilesToBitmap().
*   21/06/11 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_TILED_MAP_RENDERER_WIN_H
#define _SPX_TILED_MAP_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxTiledMapRenderer.h"
#include "SPxLibWin/SPxRendererWin.h"


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
class SPxRendererWin;
class SPxBitmapWin;

namespace Gdiplus
{
    class Graphics;
};

/*
 * Define our class, derived from the base graphics renderer object
 * and also the WIN renderer to provide the rendering functions.
 */
class SPxTiledMapRendererWin :  public SPxTiledMapRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTiledMapRendererWin(SPxTiledMapDatabase *db, 
                           SPxTerrainDatabase *tdb=NULL);
    virtual ~SPxTiledMapRendererWin(void);

    virtual SPxErrorCode SetBrightness(int brightness);
    virtual int GetBrightness(void) const { return m_brightness; }
    virtual SPxErrorCode SetContrast(int contrast);
    virtual int GetContrast(void) const { return m_contrast; }
    virtual void SetUseTransparency(int useTransparency);

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

    /* Bitmap and GDI+ used for holding map tiles. */
    SPxBitmapWin *m_mapBitmap;
    Gdiplus::Graphics *m_mapGfx;

    /* Bitmap containing an image rotated to match the display rotation. */
    SPxBitmapWin *m_rotatedMapBitmap;

    /* Translation applied to rotated image to centralise it
     * in the rotated bitmap.
     */
    int m_mapTranslationXPixels;
    int m_mapTranslationYPixels;

    /* Flag to indicate which bitmap to use for scaling. */
    int m_isSrcRotatedBitmap;

    int m_brightness;	/* Brightness adjustment. */
    int m_contrast;	/* Contrast adjustment. */

    int m_useTransparency; /* Use transparency from tiles? */

    /*
     * Private functions.
     */

    SPxErrorCode CheckBitmaps(unsigned int mapWidthPixels,
                              unsigned int mapHeightPixels,
                              int *isChanged);

    SPxErrorCode ClearMapBitmap(unsigned int mapWidthPixels,
                                unsigned int mapHeightPixels);

    SPxErrorCode RenderTileToBitmap(const char *filename,
                                    unsigned int tileSizePixels,
                                    int tileXPixels,
                                    int tileYPixels,
                                    int destXPixels,
                                    int destYPixels,
                                    double scale);
    
    SPxErrorCode RotateMap(unsigned int mapWidthPixels,
                           unsigned int mapHeightPixels);
    
    SPxErrorCode RenderMapToDestination(unsigned int mapWidthPixels,
                                        unsigned int mapHeightPixels,
                                        int offsetXPixels,
                                        int offsetYPixels,
                                        double scale);

}; /* SPxTiledMapRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_TILED_MAP_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
