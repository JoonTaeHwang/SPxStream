/*********************************************************************
*
* (c) Copyright 2011, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxADSBRendererWin.h,v $
* ID: $Id: SPxADSBRendererWin.h,v 1.1 2011/12/23 16:12:08 rew Exp $
*
* Purpose:
*	Header for SPxADSBRendererWin class which supports the display
*	of ADS-B messages in a Windows-based application.
*
* Revision Control:
*   23/12/11 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_ADSB_RENDERER_WIN_H
#define _SPX_ADSB_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxADSBRenderer.h"
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
class SPxADSBRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxADSBRendererWin : public SPxADSBRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxADSBRendererWin(SPxADSBDecoder *decoder);
    virtual ~SPxADSBRendererWin(void);

protected:
    /*
     * Protected variables.
     */

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

}; /* SPxADSBRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_ADSB_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
