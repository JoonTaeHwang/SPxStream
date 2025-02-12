/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxExtRendererCairo class which supports rendering by
*	an external object in a Linux application using Cairo.
*
* Revision Control:
*   06/08/21 v1.1    SP  	Initial Version.
*
* Previous Changes:
*
**********************************************************************/

#ifndef _SPX_EXT_RENDERER_CAIRO_H
#define _SPX_EXT_RENDERER_CAIRO_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxExtRenderer.h"
#include "SPxLibX11/SPxRendererCairo.h"

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxExtRenderer;
class SPxRendererCairo;
class SPxRendererObj;
struct _cairo;

class SPxExtRendererCairo :  public SPxExtRenderer, public SPxRendererCairo
{
public:
    /* Constructor and destructor. */
    SPxExtRendererCairo(SPxRendererObj *obj);
    virtual ~SPxExtRendererCairo(void);

}; /* SPxExtRendererCairo */

#endif /* SPX_EXTRENDERER_CAIRO_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
