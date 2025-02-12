/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxThreatRendererWin.h,v $
* ID: $Id: SPxThreatRendererWin.h,v 1.1 2013/08/14 15:14:39 rew Exp $
*
* Purpose:
*   Header for SPxThreatRenderer base class which supports the display
*   of threats in a Windows application.
*
* Revision Control:
*   06/08/13 v1.1   SP  	Initial version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_THREAT_RENDERER_WIN_H
#define _SPX_THREAT_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxThreatRenderer.h"
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
class SPxThreatRenderer;
class SPxRendererWin;

/*
 * Define our class.
 */
class SPxThreatRendererWin : public SPxThreatRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxThreatRendererWin(SPxThreatDatabase *database);
    virtual ~SPxThreatRendererWin(void);

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

}; /* SPxThreatRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_THREAT_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
