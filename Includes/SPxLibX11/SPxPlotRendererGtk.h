/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPlotRendererGtk.h,v $
* ID: $Id: SPxPlotRendererGtk.h,v 1.1 2015/11/13 12:01:03 rew Exp $
*
* Purpose:
*	Header for SPxPlotRendererGtk class which supports the display
*	of plots in a GTK-based application.
*
* Revision Control:
*   13/11/15 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_PLOT_RENDERER_GTK_H
#define _SPX_PLOT_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxPlotRenderer.h"
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
class SPxPlotRenderer;
class SPxRendererGtk;
class SPxTargetDB;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxPlotRendererGtk : public SPxPlotRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxPlotRendererGtk(SPxTargetDB *plotDatabase);
    virtual ~SPxPlotRendererGtk(void);

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

}; /* SPxPlotRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_PLOT_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
