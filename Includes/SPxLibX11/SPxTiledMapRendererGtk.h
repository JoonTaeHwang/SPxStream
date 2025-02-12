/*********************************************************************
*
* (c) Copyright 2012 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTiledMapRendererGtk class which supports the display
*	of a tiled map in a GTK+-based application.
*
* Revision Control:
*   02/10/18 v1.9    SP 	Support configurable tile size.
*
* Previous Changes:
*   28/07/16 1.8    AGC	Brightness functions now virtual.
*   02/06/14 1.7    SP 	Add isModified to CheckBitmaps().
*                       Add SetBrightness().
*   04/10/13 1.6    SP 	Support changes to the base class.
*   05/09/13 1.5    AGC	Optional display of terrain download indicator.
*   21/08/12 1.4    SP 	Move all common code to base class.
*   02/08/11 1.3    SP 	Add map tile interpolation.
*   18/07/11 1.2    SP 	Remove Render() and add RenderMap().
*   06/06/11 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_TILED_MAP_RENDERER_GTK_H
#define _SPX_TILED_MAP_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxTiledMapRenderer.h"
#include "SPxLibX11/SPxRendererGtk.h"


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
class SPxRendererGtk;

/*
 * Define our class, derived from the base graphics renderer object
 * and also the GTK renderer to provide the rendering functions.
 */
class SPxTiledMapRendererGtk :  public SPxTiledMapRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTiledMapRendererGtk(SPxTiledMapDatabase *db, SPxTerrainDatabase *tdb=NULL);
    virtual ~SPxTiledMapRendererGtk(void);

    virtual SPxErrorCode SetBrightness(int brightness);
    virtual int GetBrightness(void) const { return m_brightness; }

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    SPxErrorCode RenderMap(void);

private:
    /*
     * Private variables.
     */

    /* Pixbuf used for holding map tiles. */
    struct _GdkPixbuf *m_mapPixbuf;

    /* Pixbuf used for map scaling. */
    struct _GdkPixbuf *m_scaledMapPixbuf;

    /* Pixbuf used for tile scaling. */
    struct _GdkPixbuf *m_scaledTilePixbuf;

    /* Map rrightness in the range -100 to 100. */
    int m_brightness;

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
        
}; /* SPxTiledMapRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_TILED_MAP_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
