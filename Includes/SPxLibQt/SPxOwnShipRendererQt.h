/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxOwnShipRendererQt class which supports the display
*   of an own ship marker in a Qt-based application.
*
* Revision Control:
*   08/04/15 v1.1    JP 	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_OWN_SHIP_RENDERER_QT_H
#define _SPX_OWN_SHIP_RENDERER_QT_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxOwnShipRenderer.h"
#include "SPxLibQt/SPxRendererQt.h"


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
class SPxRendererQt;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxOwnShipRendererQt : public SPxOwnShipRenderer, public SPxRendererQt
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxOwnShipRendererQt(SPxNavData *navData);
    virtual ~SPxOwnShipRendererQt(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxOwnShipRendererQt */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_OWN_SHIP_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
