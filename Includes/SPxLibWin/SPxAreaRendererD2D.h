/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAreaRendererD2D.h,v $
* ID: $Id: SPxAreaRendererD2D.h,v 1.1 2014/06/06 14:15:36 rew Exp $
*
* Purpose:
*   Header for SPxAreaRendererD2D class which supports the display
*   of overlay or underlay graphics in a Windows-based application.
*
* Revision Control:
*   06/06/14 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef SPX_AREA_RENDERER_D2D_H
#define SPX_AREA_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxAreaRenderer.h"
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
class SPxAreaRenderer;
class SPxRendererD2D;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxAreaRendererD2D : public SPxAreaRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAreaRendererD2D(void);
    virtual ~SPxAreaRendererD2D(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxAreaRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_AREA_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
