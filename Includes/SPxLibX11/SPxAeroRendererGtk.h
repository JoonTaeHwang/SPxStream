/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAeroRendererGtk class.
*
*
* Revision Control:
*   13/10/20 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_AERO_RENDERER_GTK_H
#define _SPX_AERO_RENDERER_GTK_H

/*
 * Other headers required.
 */
/* We need the base classes. */
#include "SPxLibData/SPxAeroRenderer.h"
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
class SPxAeroRenderer;
class SPxRendererGtk;

/*
 * Define our class, derived from the base Aero renderer object
 * and also the GTK renderer to provide the rendering functions.
 */
class SPxAeroRendererGtk :  public SPxAeroRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAeroRendererGtk(SPxAeroDb *db);
    virtual ~SPxAeroRendererGtk(void);

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

}; /* SPxAeroRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_AERO_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
