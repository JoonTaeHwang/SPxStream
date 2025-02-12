/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAISRendererD2D class which supports the display
*	of AIS messages in a Direct2D-based application.
*
* Revision Control:
*   30/11/18 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_AIS_RENDERER_D2D_H
#define _SPX_AIS_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxAISRenderer.h"
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
class SPxAISRenderer;
class SPxRendererD2D;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxAISRendererD2D : public SPxAISRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAISRendererD2D(SPxAISDecoder *decoder);
    virtual ~SPxAISRendererD2D(void);

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

}; /* SPxAISRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_AIS_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
