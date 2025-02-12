/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRasterMapRendererGtk.h,v $
* ID: $Id: SPxRasterMapRendererGtk.h,v 1.2 2015/05/18 13:52:30 rew Exp $
*
* Purpose:
*       Header for SPxRasterMapRendererGtk class which
*       supports the rendering of raster images using Gtk.
*
* Revision Control:
*   18/05/15 v1.2    AGC	Update to match base class interface changes.
*
* Previous Changes:
*   11/05/15 1.1    JP  Initial Version.
**********************************************************************/

#ifndef _SPX_RASTER_MAP_RENDERER_GTK_H
#define _SPX_RASTER_MAP_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxRasterMapRenderer.h"
#include "SPxLibX11/SPxRendererGtk.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxRasterMapRenderer;
class SPxRendererGtk;
typedef struct _GdkPixbuf GdkPixbuf;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRasterMapRendererGtk : public SPxRasterMapRenderer, public SPxRendererGtk
{
public:
    /* Constructor and destructor. */
    SPxRasterMapRendererGtk(void);
    virtual ~SPxRasterMapRendererGtk(void);

private:
    /* Private functions. */
    virtual SPxErrorCode renderBitmap(SPxImage *image,
	int x, int y, double scale, double brightness, double contrast,
	void **rotatedImagePtr, double *rotationDegs, int changed);
    virtual void freeCachedBitmap(void *rotatedBitmap);

}; /* SPxRasterMapRendererGtk */

#endif /* _SPX_RASTER_MAP_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
