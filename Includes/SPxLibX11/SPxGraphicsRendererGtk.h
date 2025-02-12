/*********************************************************************
*
* (c) Copyright 2011, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxGraphicsRendererGtk.h,v $
* ID: $Id: SPxGraphicsRendererGtk.h,v 1.2 2012/12/12 17:17:10 rew Exp $
*
* Purpose:
*	Header for SPxGraphicsRendererGtk class which supports the display
*	of overlay or underlay in a GTK+-based application.
*
* Revision Control:
*   12/12/12 v1.2    AGC	Support moving platform.
*
* Previous Changes:
*   26/04/11 1.1    SP 	Initial Version.
*
**********************************************************************/

#ifndef _SPX_GRAPHICS_RENDERER_GTK_H
#define _SPX_GRAPHICS_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxGraphicsRenderer.h"
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
class SPxGraphicsRenderer;
class SPxRendererGtk;
class SPxNavData;

/*
 * Define our class, derived from the base graphics renderer object
 * and also the GTK renderer to provide the rendering functions.
 */
class SPxGraphicsRendererGtk :  public SPxGraphicsRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxGraphicsRendererGtk(SPxNavData *navData=NULL);
    virtual ~SPxGraphicsRendererGtk(void);

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

    /*
     * Private functions.
     */

}; /* SPxGraphicsRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_GRAPHICS_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
