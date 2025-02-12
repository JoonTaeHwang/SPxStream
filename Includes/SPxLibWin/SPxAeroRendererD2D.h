/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAeroRendererWin class which supports the display
*	of SPxAero items in a Direct2D-based application.
*
* Revision Control:
*   13/10/20 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_AERO_RENDERER_D2D_H
#define _SPX_AERO_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibData/SPxAeroRenderer.h"
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
class SPxAeroDb;
class SPxAeroRenderer;
class SPxRendererD2D;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxAeroRendererD2D : public SPxAeroRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAeroRendererD2D(SPxAeroDb *db);
    virtual ~SPxAeroRendererD2D(void);

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

}; /* SPxAeroRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_AERO_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
