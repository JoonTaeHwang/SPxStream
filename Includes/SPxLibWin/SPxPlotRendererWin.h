/*********************************************************************
*
* (c) Copyright 2015, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotRendererWin class which supports the display
*	of plots in a Windows-based application.
*
* Revision Control:
*   30/11/18 v1.2    AGC	Support specifying max trail length.
*
* Previous Changes:
*   13/11/15 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_PLOT_RENDERER_WIN_H
#define _SPX_PLOT_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxPlotRenderer.h"
#include "SPxLibWin/SPxRendererWin.h"


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
class SPxRendererWin;
class SPxTargetDB;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxPlotRendererWin : public SPxPlotRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxPlotRendererWin(SPxTargetDB *plotDatabase,
		       unsigned int maxTrailLenSecs = 3600);
    virtual ~SPxPlotRendererWin(void);

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

}; /* SPxPlotRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_PLOT_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
