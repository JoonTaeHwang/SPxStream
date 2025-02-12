/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRasterMapRendererQt.h,v $
* ID: $Id: SPxRasterMapRendererQt.h,v 1.2 2015/05/18 13:50:27 rew Exp $
*
* Purpose:
*       Header for SPxRasterMapRendererQt class which
*       supports the rendering of raster images using Qt.
*
* Revision Control:
*   18/05/15 v1.2    AGC	Support rotation with multiple images.
*
* Previous Changes:
*   22/04/15 1.1    JP 	Initial version.
**********************************************************************/

#ifndef _SPX_RASTER_MAP_RENDERER_QT_H
#define _SPX_RASTER_MAP_RENDERER_QT_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxRasterMapRenderer.h"
#include "SPxLibQt/SPxRendererQt.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxRasterMapRenderer;
class SPxRendererQt;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRasterMapRendererQt : public SPxRasterMapRenderer, public SPxRendererQt
{
public:
    /* Constructor and destructor. */
    SPxRasterMapRendererQt(void);
    virtual ~SPxRasterMapRendererQt(void);

private:
    /* Private functions. */
    virtual SPxErrorCode renderBitmap(SPxImage *image,
	int x, int y, double scale, double brightness, double contrast,
	void **cachedImagePtr, double *rotationDegs, int changed);
    virtual void freeCachedBitmap(void *cachedBitmap);

}; /* SPxRasterMapRendererQt */

#endif /* _SPX_RASTER_MAP_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
