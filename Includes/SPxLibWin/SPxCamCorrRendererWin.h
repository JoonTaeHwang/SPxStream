/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
*
* Purpose:
*   Header for SPxCamCorrRendererWin class which supports the display
*   of symbols for points selected for camera correction
*   in a Windows-based application.
*
* Revision Control:
*   17/08/18 v1.1    BTB	Initial Version.
*
* Previous Changes:	
* 
**********************************************************************/

#ifndef _SPX_CAM_CORR_RENDERER_WIN_H
#define _SPX_CAM_CORR_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxCamCorrRenderer.h"
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
class SPxCamCorrRenderer;
class SPxRendererWin;
class SPxNavData;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxCamCorrRendererWin : public SPxCamCorrRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxCamCorrRendererWin(void);
    virtual ~SPxCamCorrRendererWin(void);

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

#endif /* SPX_CAM_CORR_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
