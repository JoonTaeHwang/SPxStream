/*********************************************************************
*
* (c) Copyright 2013 - 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxExtRendererD2D.h,v $
* ID: $Id: SPxExtRendererD2D.h,v 1.2 2014/10/14 14:58:57 rew Exp $
*
* Purpose:
*   Header for SPxExtRendererD2D class which supports the display
*   of external graphics in a Direct2D Windows-based application.
*
* Revision Control:
*   14/10/14 v1.2    AGC	Make constructor explicit.
*
* Previous Changes:
*   07/10/13 1.1    AGC	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_EXT_RENDERER_D2D_H
#define _SPX_EXT_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxExtRenderer.h"
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
class SPxExtRenderer;
class SPxRendererD2D;
class SPxRendererObj;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxExtRendererD2D : public SPxExtRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxExtRendererD2D(SPxRendererObj *obj);
    virtual ~SPxExtRendererD2D(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxExtRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_EXT_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
