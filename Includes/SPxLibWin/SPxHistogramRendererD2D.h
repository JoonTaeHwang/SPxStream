/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxHistogramRendererD2D class which supports the
*	display of a histogram area in a Direct2D-based application.
*
* Revision Control:
*   30/11/18 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_HISTOGRAM_RENDERER_D2D_H
#define _SPX_HISTOGRAM_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxHistogramRenderer.h"
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
class SPxHistogramRenderer;
class SPxRendererD2D;
class SPxRunProcess;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxHistogramRendererD2D : public SPxHistogramRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxHistogramRendererD2D(SPxRunProcess *histRunProcess);
    virtual ~SPxHistogramRendererD2D(void);

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

}; /* SPxHistogramRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_HISTOGRAM_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
