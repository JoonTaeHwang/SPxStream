/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCompassRoseRendererWpf class which supports the display
*	of a compass rose around a circular PPI window area
*	in a WPF-based application.
*
* Revision Control:
*   01/11/19 v1.1    BTB	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_COMPASS_ROSE_RENDERER_WPF_H
#define _SPX_COMPASS_ROSE_RENDERER_WPF_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxCompassRoseRenderer.h"
#include "SPxLibWin/SPxRendererWpf.h"


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
class SPxCompassRoseRenderer;
class SPxRendererWpf;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxCompassRoseRendererWpf : public SPxCompassRoseRenderer, public SPxRendererWpf
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxCompassRoseRendererWpf(void);
    virtual ~SPxCompassRoseRendererWpf(void);

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

}; /* SPxCompassRoseRendererWpf */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_COMPASS_ROSE_RENDERER_WPF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
