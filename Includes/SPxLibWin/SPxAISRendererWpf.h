/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAISRendererWpf class which supports the display
*	of AIS messages in a WPF-based application.
*
* Revision Control:
*   24/09/19 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_AIS_RENDERER_WPF_H
#define _SPX_AIS_RENDERER_WPF_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxAISRenderer.h"
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
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxAISRenderer;
class SPxRendererWpf;
class SPxAISDecoder;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxAISRendererWpf : public SPxAISRenderer, public SPxRendererWpf
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxAISRendererWpf(SPxAISDecoder *decoder);
    virtual ~SPxAISRendererWpf(void);

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

}; /* SPxAISRendererWpf */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_AIS_RENDERER_WPF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
