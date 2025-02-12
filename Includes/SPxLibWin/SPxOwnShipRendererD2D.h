/*********************************************************************
*
* (c) Copyright 2013, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxOwnShipRendererD2D class which supports the display
*   of an own ship marker in a Windows-based application.
*
* Revision Control:
*   30/11/18 v1.2    AGC	Support omitting nav data object.
*
* Previous Changes:
*   27/09/13 1.1    AGC	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_OWN_SHIP_RENDERER_D2D_H
#define _SPX_OWN_SHIP_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxOwnShipRenderer.h"
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
class SPxOwnShipRenderer;
class SPxRendererD2D;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxOwnShipRendererD2D : public SPxOwnShipRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxOwnShipRendererD2D(SPxNavData *navData=NULL);
    virtual ~SPxOwnShipRendererD2D(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxOwnShipRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_OWN_SHIP_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
