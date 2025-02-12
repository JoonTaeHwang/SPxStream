/*********************************************************************
*
* (c) Copyright 2011, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAISRendererGtk.h,v $
* ID: $Id: SPxAISRendererGtk.h,v 1.4 2011/04/13 07:26:56 rew Exp $
*
* Purpose:
*	Header for SPxAISRendererGtk class which supports the display
*	of AIS messages in a GTK+-based application.
*
* Revision Control:
*   12/04/11 v1.4    SP 	Remove unused Get/SetParameter() functions.
*
* Previous Changes:
*   06/04/11 1.3    SP 	Also derive from SPxRendererGtk.
*   04/03/11 1.2    SP 	Add setColour().
*   02/03/11 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_AIS_RENDERER_GTK_H
#define _SPX_AIS_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxAISRenderer.h"
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
class SPxAISRenderer;
class SPxRendererGtk;

/*
 * Define our class, derived from the base AIS renderer object
 * and also the GTK renderer to provide the rendering functions.
 */
class SPxAISRendererGtk :  public SPxAISRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAISRendererGtk(SPxAISDecoder *decoder);
    virtual ~SPxAISRendererGtk(void);

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

}; /* SPxAISRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_AIS_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
