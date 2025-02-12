/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRasterMapRendererWin class which supports the
*	rendering of raster images under Windows.
*
* Revision Control:
*   06/05/22 v1.5    SP 	Support transparency.
*
* Previous Changes:
*   18/05/15 1.4    AGC	Support rotation with multiple images.
*   20/06/14 1.3    SP 	Support contrast adjustment.
*   04/10/13 1.2    SP 	Support display rotation.
*   01/08/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_RASTER_MAP_RENDERER_WIN_H
#define _SPX_RASTER_MAP_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxRasterMapRenderer.h"
#include "SPxLibWin/SPxRendererWin.h"
#include "SPxLibWin/SPxBitmapWin.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxRasterMapRenderer;
class SPxRendererWin;
class SPxImage;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRasterMapRendererWin : public SPxRasterMapRenderer, public SPxRendererWin
{
public:
    /* Constructor and destructor. */
    SPxRasterMapRendererWin(void);
    virtual ~SPxRasterMapRendererWin(void);

    /* COnfiguration. */
    SPxErrorCode SetTransparencyEnabled(int isEnabled);
    int IsTransparencyEnabled(void) const;

private:
    /* Private variables. */
    int m_isTransparencyEnabled;	/* Is map image transparency enabled? */

    /* Private functions. */
    virtual SPxErrorCode renderBitmap(SPxImage *image,
	int x, int y, double scale, double brightness, double contrast,
	void **cachedImagePtr, double *rotationDegs, int changed);
    virtual void freeCachedBitmap(void *cachedBitmap);

}; /* SPxRasterMapRendererWin */

#endif /* _SPX_RASTER_MAP_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
