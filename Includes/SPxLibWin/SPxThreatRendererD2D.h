/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxThreatRenderer base class which supports the display
*   of threats in a Direct2D application.
*
* Revision Control:
*    19/06/19 v1.1   AGC	Initial version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_THREAT_RENDERER_D2D_H
#define _SPX_THREAT_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxThreatRenderer.h"
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
class SPxThreatRenderer;
class SPxRendererD2D;

/*
 * Define our class.
 */
class SPxThreatRendererD2D : public SPxThreatRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxThreatRendererD2D(SPxThreatDatabase *database);
    virtual ~SPxThreatRendererD2D(void);

protected:
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

    /*
     * Private static functions.
     */

}; /* SPxThreatRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_THREAT_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
