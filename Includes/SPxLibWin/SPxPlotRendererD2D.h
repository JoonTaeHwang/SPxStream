/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotRendererD2D class which supports the display
*	of plots in a Direct2D-based application.
*
* Revision Control:
*   30/11/18 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_PLOT_RENDERER_D2D_H
#define _SPX_PLOT_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxPlotRenderer.h"
#include "SPxLibWin/SPxRendererD2D.h"


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
class SPxRendererD2D;
class SPxTargetDB;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxPlotRendererD2D : public SPxPlotRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxPlotRendererD2D(SPxTargetDB *plotDatabase,
		       unsigned int maxTrailLenSecs=3600);
    virtual ~SPxPlotRendererD2D(void);

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

}; /* SPxPlotRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_PLOT_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
