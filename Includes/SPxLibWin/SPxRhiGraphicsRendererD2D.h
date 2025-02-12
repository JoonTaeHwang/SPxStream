/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for renderer base class which supports
 *   the display of graphics in a range/altitude window
 *   using Direct2D.
 *
 * Revision Control:
 *   13/11/20 v1.2   SP 	Rename class.
 *
 * Previous Changes:
 *   27/10/20 1.1   SP 	Initial version.
 *
 **********************************************************************/

#ifndef _SPX_RHI_GRAPHICS_RENDERER_D2D_H
#define _SPX_RHI_GRAPHICS_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxRhiGraphicsRenderer.h"
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
class SPxRhiGraphicsRenderer;
class SPxRendererD2D;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRhiGraphicsRendererD2D : public SPxRhiGraphicsRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRhiGraphicsRendererD2D(SPxRhi *view);
    virtual ~SPxRhiGraphicsRendererD2D(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxRhiGraphicsRendererD2D */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_RHI_GRAPHICS_RENDERER_D2D_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
