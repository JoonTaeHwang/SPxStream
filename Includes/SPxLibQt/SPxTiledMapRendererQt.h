/*********************************************************************
*
* (c) Copyright 2015 - 2018 Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for a SPxTiledMapRendererQt class.
*
* Revision Control:
*   03/10/18 v1.2   SP      Support configurable tile size.
*
* Previous Changes:
*   07/01/15 1.1   JP      Initial version.
*
**********************************************************************/
#ifndef _SPX_TILED_MAP_RENDERER_QT_H
#define _SPX_TILED_MAP_RENDERER_QT_H

/* We need the base classes. */
#include "SPxLibUtils/SPxTiledMapRenderer.h"
#include "SPxLibQt/SPxRendererQt.h"

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
class SPxRendererQt;

/*
 * Define our class, derived from the base graphics renderer object
 * and also the GTK renderer to provide the rendering functions.
 */
class SPxTiledMapRendererQt :  public SPxTiledMapRenderer, public SPxRendererQt
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTiledMapRendererQt(SPxTiledMapDatabase *db, SPxTerrainDatabase *tdb=NULL);
    virtual ~SPxTiledMapRendererQt(void);

    SPxErrorCode SetBrightness(int brightness);

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

	QPixmap *m_tiledMapPixmap;

    /* Bitmap containing an image rotated to match the display rotation. */
    QPixmap *m_rotatedTiledMapPixmap;

    /* Brightness adjustment. */
    int m_brightness;

    /* Flag to indicate which bitmap to use for scaling. */
    int m_isSrcRotatedBitmap;

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

#endif /* SPX_TILED_MAP_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
