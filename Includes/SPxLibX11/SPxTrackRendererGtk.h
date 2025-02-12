/*********************************************************************
*
* (c) Copyright 2011, 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxTrackRendererGtk.h,v $
* ID: $Id: SPxTrackRendererGtk.h,v 1.2 2012/02/08 16:18:22 rew Exp $
*
* Purpose:
*	Header for SPxTrackRendererGtk class which supports the display
*	of radar tracks in a GTK+-based application.
*
* Revision Control:
*   08/02/12 v1.2    AGC	Add optional render config option.
*
* Previous Changes:
*   12/04/11 1.1    SP 	Initial Version.
*
**********************************************************************/

#ifndef _SPX_TRACK_RENDERER_GTK_H
#define _SPX_TRACK_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxTrackRenderer.h"
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
class SPxTrackRenderer;
class SPxRendererGtk;

/*
 * Define our class, derived from the base track renderer object
 * and also the GTK renderer to provide the rendering functions.
 */
class SPxTrackRendererGtk :  public SPxTrackRenderer, public SPxRendererGtk
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTrackRendererGtk(SPxTrackDatabase *database,
			SPxTrackRenderConfig renderConfig = SPX_TRACK_RENDER_CONFIG_SPX);
    virtual ~SPxTrackRendererGtk(void);

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

}; /* SPxTrackRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_TRACK_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
