/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for SPxAISRendererQt class which supports the display
*	of AIS messages in a Qt-based application.
*
* Revision Control:
*   13/04/15 v1.1    JP 	Initial Version.
*
* Previous Changes:
*
**********************************************************************/

#ifndef _SPX_AIS_RENDERER_QT_H
#define _SPX_AIS_RENDERER_QT_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxAISRenderer.h"
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
class SPxAISRenderer;
class SPxRendererQt;

/*
 * Define our class, derived from the base AIS renderer object
 * and also the Qt renderer to provide the rendering functions.
 */
class SPxAISRendererQt :  public SPxAISRenderer, public SPxRendererQt
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAISRendererQt(SPxAISDecoder *decoder);
    virtual ~SPxAISRendererQt(void);

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

}; /* SPxAISRendererQt */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_AIS_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
