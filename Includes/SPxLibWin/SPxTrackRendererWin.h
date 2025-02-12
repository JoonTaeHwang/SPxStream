/*********************************************************************
*
* (c) Copyright 2011, 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxTrackRendererWin.h,v $
* ID: $Id: SPxTrackRendererWin.h,v 1.2 2012/02/08 17:41:47 rew Exp $
*
* Purpose:
*   Header for SPxTrackRendererWin class which supports the display
*   of radar tracks in a Windows-based application.
*
* Revision Control:
*   08/02/12 v1.2    AGC	Add optional render config option.
*
* Previous Changes:
*   12/04/11 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_TRACK_RENDERER_WIN_H
#define _SPX_TRACK_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxTrackRenderer.h"
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
class SPxTrackRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxTrackRendererWin : public SPxTrackRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTrackRendererWin(SPxTrackDatabase *database,
			SPxTrackRenderConfig renderConfig = SPX_TRACK_RENDER_CONFIG_SPX);
    virtual ~SPxTrackRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxTrackRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_Track_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
