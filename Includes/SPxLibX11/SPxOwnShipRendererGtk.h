/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxOwnShipRendererGtk.h,v $
* ID: $Id: SPxOwnShipRendererGtk.h,v 1.1 2012/06/22 08:01:55 rew Exp $
*
* Purpose:
*   Header for SPxOwnShipRendererWin class which supports the display
*   of an own ship marker in a GTK+-based application.
*
* Revision Control:
*   21/06/12 v1.1    SP 	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_OWN_SHIP_RENDERER_GTK_H
#define _SPX_OWN_SHIP_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxOwnShipRenderer.h"
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
class SPxOwnShipRenderer;
class SPxRendererGtk;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxOwnShipRendererGtk : public SPxOwnShipRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxOwnShipRendererGtk(SPxNavData *navData);
    virtual ~SPxOwnShipRendererGtk(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxOwnShipRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_OWN_SHIP_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
