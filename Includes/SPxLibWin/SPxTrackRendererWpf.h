/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTrackRendererWpf class which supports the display
*	of radar tracks in a WPF-based application.
*
* Revision Control:
*   24/09/19 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_TRACK_RENDERER_WPF_H
#define _SPX_TRACK_RENDERER_WPF_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxTrackRenderer.h"
#include "SPxLibWin/SPxRendererWpf.h"


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
class SPxRendererWpf;
class SPxTrackDatabase;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxTrackRendererWpf : public SPxTrackRenderer, public SPxRendererWpf
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxTrackRendererWpf(SPxTrackDatabase *database);
    virtual ~SPxTrackRendererWpf(void);

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

}; /* SPxTrackRendererWpf */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TRACK_RENDERER_WPF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
