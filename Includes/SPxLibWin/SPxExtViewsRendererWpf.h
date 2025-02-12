/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxExtViewsRenderer base class which supports
*	displaying outlines of other view areas
*	in a WPF-based application.
*
* Revision Control:
*   25/11/19 v1.1    BTB	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_EXT_VIEWS_RENDERER_WPF_H
#define _SPX_EXT_VIEWS_RENDERER_WPF_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxExtViewsRenderer.h"
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
class SPxExtViewsRenderer;
class SPxRendererWpf;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxExtViewsRendererWpf : public SPxExtViewsRenderer, public SPxRendererWpf
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxExtViewsRendererWpf(void);
    virtual ~SPxExtViewsRendererWpf(void);

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

}; /* SPxExtViewsRendererWpf */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_EXT_VIEWS_RENDERER_WPF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
