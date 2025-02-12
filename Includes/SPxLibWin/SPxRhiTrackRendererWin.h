/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for renderer base class which supports the display 
 *   of plots and tracks in a range/altitude window using GDI+.
 *
 * Revision Control:
 *   13/11/20 v1.4   SP 	Rename class.
 *
 * Previous Changes:
 *   27/10/20 1.3   SP 	Split graphics out into new renderer.
 *   23/10/20 1.2   SP 	Correct description.
 *   22/10/20 1.1   SP 	Initial version.
 *
 **********************************************************************/

#ifndef _SPX_RHI_TRACK_RENDERER_WIN_H
#define _SPX_RHI_TRACK_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxRhiTrackRenderer.h"
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
class SPxRhiTrackRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRhiTrackRendererWin : public SPxRhiTrackRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRhiTrackRendererWin(SPxRhi *view, SPxTrackDatabase *trackDB);
    virtual ~SPxRhiTrackRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxRhiTrackRendererWin */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_RHI_TRACK_RENDERER_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
