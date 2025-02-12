/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRasterMapRendererD2D class which supports the
*	rendering of raster images under Direct2D.
*
* Revision Control:
*   30/11/18 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_RASTER_MAP_RENDERER_D2D_H
#define _SPX_RASTER_MAP_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxRasterMapRenderer.h"
#include "SPxLibWin/SPxRendererD2D.h"
#include "SPxLibWin/SPxBitmapWin.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxRasterMapRenderer;
class SPxRendererD2D;
class SPxImage;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRasterMapRendererD2D : public SPxRasterMapRenderer, public SPxRendererD2D
{
public:
    /* Constructor and destructor. */
    SPxRasterMapRendererD2D(void);
    virtual ~SPxRasterMapRendererD2D(void);

private:
    /* Private variables. */

    /* Private functions. */
    virtual SPxErrorCode renderBitmap(SPxImage *image,
	int x, int y, double scale, double brightness, double contrast,
	void **cachedImagePtr, double *rotationDegs, int changed);
    virtual void freeCachedBitmap(void * /*cachedBitmap*/) {}

}; /* SPxRasterMapRendererD2D */

#endif /* _SPX_RASTER_MAP_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
