/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxOwnShipRendererWin.h,v $
* ID: $Id: SPxOwnShipRendererWin.h,v 1.1 2012/02/08 17:40:30 rew Exp $
*
* Purpose:
*   Header for SPxOwnShipRendererWin class which supports the display
*   of an own ship marker in a Windows-based application.
*
* Revision Control:
*   08/02/12 v1.1    SP 	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_OWN_SHIP_RENDERER_WIN_H
#define _SPX_OWN_SHIP_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxOwnShipRenderer.h"
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
class SPxOwnShipRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxOwnShipRendererWin : public SPxOwnShipRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxOwnShipRendererWin(SPxNavData *navData);
    virtual ~SPxOwnShipRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxOwnShipRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_OWN_SHIP_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
