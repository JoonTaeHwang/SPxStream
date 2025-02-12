/*********************************************************************
 *
 * (c) Copyright 2015, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxRecordRendererWin.h,v $
 * ID: $Id: SPxRecordRendererWin.h,v 1.2 2015/02/16 15:21:16 rew Exp $
 *
 * Purpose:
 *   Header for SPxRecordRendererWin class which supports the display
 *   of stamp and channel records in a Windows-based application.
 *
 * Revision Control:
 *   16/02/15 v1.2    SP  Change constructor args.
 *
 * Previous Changes:
 *   07/01/15 1.1    SP  Initial version.
 *
 **********************************************************************/

#ifndef _SPX_EVENT_RENDERER_WIN_H
#define _SPX_EVENT_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxRecordRenderer.h"
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
class SPxRecordRenderer;
class SPxRendererWin;
class SPxTimelineDatabase;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRecordRendererWin : public SPxRecordRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRecordRendererWin(SPxTimelineDatabase *timelineDB,
                         SPxStampDatabase *stampDB);
    virtual ~SPxRecordRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxRecordRendererWin */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* SPX_EVENT_RENDERER_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
