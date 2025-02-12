/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
*
* Purpose:
*   Header for SPxCamCorrRendererWin class which supports the display
*   of symbols for points selected for camera correction
*   in a Direct2D-based application.
*
* Revision Control:
*   19/06/19 v1.1    AGC	Initial Version.
*
* Previous Changes:	
* 
**********************************************************************/

#ifndef _SPX_CAM_CORR_RENDERER_D2D_H
#define _SPX_CAM_CORR_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxCamCorrRenderer.h"
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
class SPxCamCorrRenderer;
class SPxRendererD2D;
class SPxNavData;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxCamCorrRendererD2D : public SPxCamCorrRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxCamCorrRendererD2D(void);
    virtual ~SPxCamCorrRendererD2D(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxCamCorrRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_CAM_CORR_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
