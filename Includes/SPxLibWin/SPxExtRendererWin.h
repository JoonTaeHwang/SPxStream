/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxExtRendererWin.h,v $
* ID: $Id: SPxExtRendererWin.h,v 1.1 2013/10/07 13:48:40 rew Exp $
*
* Purpose:
*   Header for SPxExtRendererWin class which supports the display
*   of external graphics in a Windows-based application.
*
* Revision Control:
*   07/10/13 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_EXT_RENDERER_WIN_H
#define _SPX_EXT_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxExtRenderer.h"
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
class SPxExtRenderer;
class SPxRendererWin;
class SPxRendererObj;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxExtRendererWin : public SPxExtRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxExtRendererWin(SPxRendererObj *obj);
    virtual ~SPxExtRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxExtRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_EXT_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
