/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxDFRendererGtk class which supports the display
*	of DF messages in a GTK+-based application.
*
* Revision Control:
*   22/02/18 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_DF_RENDERER_GTK_H
#define _SPX_DF_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxDFRenderer.h"
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
class SPxDFRenderer;
class SPxRendererGtk;

/*
 * Define our class, derived from the base DF renderer object
 * and also the GTK renderer to provide the rendering functions.
 */
class SPxDFRendererGtk :  public SPxDFRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxDFRendererGtk(SPxDFReceiver *receiver);
    virtual ~SPxDFRendererGtk(void);

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

}; /* SPxDFRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_DF_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
