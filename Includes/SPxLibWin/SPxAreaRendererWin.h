/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAreaRendererWin.h,v $
* ID: $Id: SPxAreaRendererWin.h,v 1.1 2012/02/08 17:39:55 rew Exp $
*
* Purpose:
*   Header for SPxAreaRendererWin class which supports the display
*   of active areas in a Windows-based application.
*
* Revision Control:
*   08/02/12 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_AREA_RENDERER_WIN_H
#define _SPX_AREA_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxAreaRenderer.h"
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
class SPxAreaRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxAreaRendererWin : public SPxAreaRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAreaRendererWin(void);
    virtual ~SPxAreaRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxAreaRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_AREA_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
