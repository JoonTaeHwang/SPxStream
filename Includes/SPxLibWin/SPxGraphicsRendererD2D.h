/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxGraphicsRendererD2D.h,v $
* ID: $Id: SPxGraphicsRendererD2D.h,v 1.1 2013/09/27 13:59:31 rew Exp $
*
* Purpose:
*   Header for SPxGraphicsRendererD2D class which supports the display
*   of overlay or underlay graphics in a Windows-based application.
*
* Revision Control:
*   27/09/13 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef SPX_GRAPHICS_RENDERER_D2D_H
#define SPX_GRAPHICS_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxGraphicsRenderer.h"
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
class SPxGraphicsRenderer;
class SPxRendererD2D;
class SPxNavData;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxGraphicsRendererD2D : public SPxGraphicsRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxGraphicsRendererD2D(SPxNavData *navData=NULL);
    virtual ~SPxGraphicsRendererD2D(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxGraphicsRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_GRAPHICS_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
