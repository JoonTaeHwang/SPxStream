/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for SPxTrackRendererQt class which supports the display
*	of radar tracks in a Qt-based application.
*
* Revision Control:
*   10/04/15 v1.1    SP  Initial Version.
*
* Previous Changes:
*
**********************************************************************/

#ifndef _SPX_TRACK_RENDERER_QT_H
#define _SPX_TRACK_RENDERER_QT_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxTrackRenderer.h"
#include "SPxLibQt/SPxRendererQt.h"


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
class SPxRendererQt;

/*
 * Define our class, derived from the base track renderer object
 * and also the GTK renderer to provide the rendering functions.
 */
class SPxTrackRendererQt :  public SPxTrackRenderer, public SPxRendererQt
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTrackRendererQt(SPxTrackDatabase *database,
			SPxTrackRenderConfig renderConfig = SPX_TRACK_RENDER_CONFIG_SPX);
    virtual ~SPxTrackRendererQt(void);

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

}; /* SPxTrackRendererQt */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TRACK_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
