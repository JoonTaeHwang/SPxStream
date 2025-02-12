/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxGraphicsRendererQt.h,v $
* ID: $Id: SPxGraphicsRendererQt.h,v 1.1 2015/01/07 16:16:47 rew Exp $
*
* Purpose:
*	Header for SPxGraphicsRendererQt class which supports the display
*	of overlay or underlay in a Qt-based application.
*
* Revision Control:
*   07/01/15 v1.1    JP 	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_GRAPHICS_RENDERER_QT_H
#define _SPX_GRAPHICS_RENDERER_QT_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxGraphicsRenderer.h"
#include "SPxLibQt/SPxRendererQt.h"


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
class SPxGraphicsRenderer;
class SPxRendererQt;
class SPxNavData;

/*
 * Define our class, derived from the base graphics renderer object
 * and also the Qt renderer to provide the rendering functions.
 */
class SPxGraphicsRendererQt :  public SPxGraphicsRenderer, public SPxRendererQt
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxGraphicsRendererQt(SPxNavData *navData=NULL);
    virtual ~SPxGraphicsRendererQt(void);

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

}; /* SPxGraphicsRendererQt */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_GRAPHICS_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
