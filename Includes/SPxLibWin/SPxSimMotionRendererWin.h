/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSimMotionRendererWin.h,v $
* ID: $Id: SPxSimMotionRendererWin.h,v 1.2 2014/06/26 14:51:39 rew Exp $
*
* Purpose:
*   Header for SPxSimMotionRendererWin class which supports the display
*   of simulated motion objects from a radar simulator.
*
* Revision Control:
*   26/06/14 v1.2    AGC	Support use with single SPxSimMotion object.
*
* Previous Changes:
*   26/10/12 1.1    AGC	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_SIM_MOTION_RENDERER_WIN_H
#define _SPX_SIM_MOTION_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibData/SPxSimMotionRenderer.h"
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
class SPxSimMotionRenderer;
class SPxRendererWin;
class SPxRadarSimulator;
class SPxSimMotion;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxSimMotionRendererWin : public SPxSimMotionRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxSimMotionRendererWin(SPxRadarSimulator *radarSim);
    explicit SPxSimMotionRendererWin(SPxSimMotion *motion);
    virtual ~SPxSimMotionRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxSimMotionRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SIM_MOTION_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
