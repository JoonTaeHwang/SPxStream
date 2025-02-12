/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for renderer base class which supports
 *   the display of graphics in a range/altitude window
 *   using GDI+.
 *
 * Revision Control:
 *   13/11/20 v1.2   SP 	Rename class.
 *
 * Previous Changes:
 *   27/10/20 1.1   SP 	Initial version.
 *
 **********************************************************************/

#ifndef _SPX_RHI_GRAPHICS_RENDERER_WIN_H
#define _SPX_RHI_GRAPHICS_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxRhiGraphicsRenderer.h"
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
class SPxRhiGraphicsRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRhiGraphicsRendererWin : public SPxRhiGraphicsRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRhiGraphicsRendererWin(SPxRhi *view);
    virtual ~SPxRhiGraphicsRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxRhiGraphicsRendererWin */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_RHI_GRAPHICS_RENDERER_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
