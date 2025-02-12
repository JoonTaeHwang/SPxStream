/*********************************************************************
*
* (c) Copyright 2011, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxADSBRendererGtk.h,v $
* ID: $Id: SPxADSBRendererGtk.h,v 1.1 2011/12/23 16:12:56 rew Exp $
*
* Purpose:
*	Header for SPxADSBRendererGtk class which supports the display
*	of ADSB messages in a GTK+-based application.
*
* Revision Control:
*   23/12/11 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_ADSB_RENDERER_GTK_H
#define _SPX_ADSB_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxADSBRenderer.h"
#include "SPxLibX11/SPxRendererGtk.h"


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
class SPxRendererGtk;

/*
 * Define our class, derived from the base ADSB renderer object
 * and also the GTK renderer to provide the rendering functions.
 */
class SPxADSBRendererGtk :  public SPxADSBRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxADSBRendererGtk(SPxADSBDecoder *decoder);
    virtual ~SPxADSBRendererGtk(void);

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

}; /* SPxADSBRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_ADSB_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
