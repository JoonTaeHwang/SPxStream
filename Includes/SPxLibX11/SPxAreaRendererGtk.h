/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAreaRendererGtk.h,v $
* ID: $Id: SPxAreaRendererGtk.h,v 1.1 2012/02/08 16:00:15 rew Exp $
*
* Purpose:
*   Header for SPxAreaRendererGtk class which supports the display
*   of active areas in a Gtk-based application.
*
* Revision Control:
*   08/02/12 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_AREA_RENDERER_GTK_H
#define _SPX_AREA_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxAreaRenderer.h"
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
class SPxAreaRenderer;
class SPxRendererGtk;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxAreaRendererGtk : public SPxAreaRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAreaRendererGtk(void);
    virtual ~SPxAreaRendererGtk(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxAreaRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_AREA_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
