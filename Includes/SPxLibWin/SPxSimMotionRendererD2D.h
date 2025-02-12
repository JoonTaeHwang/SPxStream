/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSimMotionRendererD2D.h,v $
* ID: $Id: SPxSimMotionRendererD2D.h,v 1.2 2014/06/26 14:51:39 rew Exp $
*
* Purpose:
*   Header for SPxSimMotionRendererD2D class which supports the display
*   of simulated motion objects from a radar simulator.
*
* Revision Control:
*   26/06/14 v1.2    AGC	Support use with single SPxSimMotion object.
*
* Previous Changes:
*   27/09/13 1.1    AGC	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_SIM_MOTION_RENDERER_D2D_H
#define _SPX_SIM_MOTION_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibData/SPxSimMotionRenderer.h"
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
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxSimMotionRenderer;
class SPxRendererD2D;
class SPxRadarSimulator;
class SPxSimMotion;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxSimMotionRendererD2D : public SPxSimMotionRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxSimMotionRendererD2D(SPxRadarSimulator *radarSim);
    explicit SPxSimMotionRendererD2D(SPxSimMotion *motion);
    virtual ~SPxSimMotionRendererD2D(void);

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

#endif /* _SPX_SIM_MOTION_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
