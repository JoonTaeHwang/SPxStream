/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmRendererWin class which supports
 *   the display of alarm graphics in a WPF-based application.
 *
 * Revision Control:
 *   19/12/19 v1.1    BTB	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_ALARM_RENDERER_WPF_H
#define _SPX_ALARM_RENDERER_WPF_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxAlarmRenderer.h"
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
class SPxAlarmRendererWpf : public SPxAlarmRenderer, public SPxRendererWpf
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAlarmRendererWpf(SPxAlarmDatabase *database);
    virtual ~SPxAlarmRendererWpf(void);

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

#endif /* _SPX_ALARM_RENDERER_WPF_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
