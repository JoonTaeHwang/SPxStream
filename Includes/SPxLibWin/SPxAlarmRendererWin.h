/*********************************************************************
 *
 * (c) Copyright 2017, 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmRendererWin class which supports
 *   the display of alarm graphics in a Windows-based application.
 *
 * Revision Control:
 *   01/07/19 v1.2    AGC	Remove unused headers etc.
 *
 * Previous Changes:
 *   18/10/17 1.1    SP     Initial version.
 *
 *********************************************************************/

#ifndef _SPX_ALARM_RENDERER_WIN_H
#define _SPX_ALARM_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxAlarmRenderer.h"
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
 *********************************************************************/


/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxAlarmRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxAlarmRendererWin : public SPxAlarmRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAlarmRendererWin(SPxAlarmDatabase *database);
    virtual ~SPxAlarmRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxAlarmRendererWin */


/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_ALARM_RENDERER_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
