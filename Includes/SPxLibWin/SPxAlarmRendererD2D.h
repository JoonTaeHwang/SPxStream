/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmRendererD2D class which supports
 *   the display of alarm graphics in a Direct2D-based application.
 *
 * Revision Control:
 *   01/07/19 v1.1    AGC	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_ALARM_RENDERER_D2D_H
#define _SPX_ALARM_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxAlarmRenderer.h"
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
 *********************************************************************/


/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxAlarmRenderer;
class SPxRendererD2D;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxAlarmRendererD2D : public SPxAlarmRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAlarmRendererD2D(SPxAlarmDatabase *database);
    virtual ~SPxAlarmRendererD2D(void);

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

#endif /* SPX_ALARM_RENDERER_D2D_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
