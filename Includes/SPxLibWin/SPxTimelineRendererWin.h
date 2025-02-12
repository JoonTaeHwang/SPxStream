/*********************************************************************
 *
 * (c) Copyright 2014, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxTimelineRendererWin.h,v $
 * ID: $Id: SPxTimelineRendererWin.h,v 1.1 2014/11/21 14:37:06 rew Exp $
 *
 * Purpose:
 *   Header for SPxTimelineRendererWin class which supports the display
 *   of a record/replay timeline in a Windows-based application.
 *
 * Revision Control:
 *   21/11/14 v1.1    SP     Initial version.
 *
 * Previous Changes:
 *
 **********************************************************************/

#ifndef _SPX_TIMELINE_RENDERER_WIN_H
#define _SPX_TIMELINE_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxTimelineRenderer.h"
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
class SPxTimelineRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxTimelineRendererWin : public SPxTimelineRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTimelineRendererWin(SPxTimelineDatabase *database);
    virtual ~SPxTimelineRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxTimelineRendererWin */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* SPX_TIMELINE_RENDERER_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
