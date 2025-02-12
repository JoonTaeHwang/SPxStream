/*********************************************************************
*
* (c) Copyright 2011, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxGraphicsRendererWin.h,v $
* ID: $Id: SPxGraphicsRendererWin.h,v 1.2 2012/12/12 17:16:05 rew Exp $
*
* Purpose:
*   Header for SPxGraphicsRendererWin class which supports the display
*   of overlay or underlay graphics in a Windows-based application.
*
* Revision Control:
*   12/12/12 v1.2    AGC	Add moving platform support.
*
* Previous Changes:
*   26/04/11 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_GRAPHICS_RENDERER_WIN_H
#define _SPX_GRAPHICS_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxGraphicsRenderer.h"
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
class SPxGraphicsRenderer;
class SPxRendererWin;
class SPxNavData;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxGraphicsRendererWin : public SPxGraphicsRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxGraphicsRendererWin(SPxNavData *navData=NULL);
    virtual ~SPxGraphicsRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxGraphicsRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_GRAPHICS_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
