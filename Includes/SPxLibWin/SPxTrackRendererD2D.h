/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxTrackRendererD2D.h,v $
* ID: $Id: SPxTrackRendererD2D.h,v 1.1 2013/09/27 13:59:31 rew Exp $
*
* Purpose:
*   Header for SPxTrackRendererD2D class which supports the display
*   of radar tracks in a Windows-based application.
*
* Revision Control:
*   27/09/13 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_TRACK_RENDERER_D2D_H
#define _SPX_TRACK_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxTrackRenderer.h"
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
class SPxTrackRenderer;
class SPxRendererD2D;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxTrackRendererD2D : public SPxTrackRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTrackRendererD2D(SPxTrackDatabase *database,
			SPxTrackRenderConfig renderConfig = SPX_TRACK_RENDER_CONFIG_SPX);
    virtual ~SPxTrackRendererD2D(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxTrackRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_Track_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
