/*********************************************************************
*
* (c) Copyright 2011 - 2015, 2018, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxRendererGtk class which supports rendering
*   of graphics in a GTK+-based application.
*
* Revision Control:
*   10/08/21 v1.26   SP 	Rename m_cr to avoid clash with base class.
*
* Previous Changes:
*   09/08/21 1.25   SP 	Only destroy Cairo context if created by class.
*   06/08/21 1.24   SP 	Add SetDestination() for backward compatibility.
*   06/08/21 1.23   SP 	Derive from SPxRendererCairo.
*   13/12/18 1.22   BTB	Support adjusting text font size.
*   30/11/18 1.21   AGC	Support offscreen bitmaps.
*   27/02/18 1.20   AGC	Support double versions of render functions.
*   04/12/15 1.19   REW	Support mouseOverBitmap in SPxSoftButtons.
*   12/06/15 1.18   AGC	Support user specified soft button sizes.
*   09/02/15 1.17   AGC	Support automatic soft button management.
*   24/11/14 1.16   SP 	Add setClipRectangle().
*   04/11/13 1.15   AGC	Add cairo form of SetDestination().
*   08/10/13 1.14   AGC	Remove createPopupMenu() function.
*   07/10/13 1.13   AGC	Support new forms for renderArc(),
*			renderSector() and renderSectorGradient().
*   04/10/13 1.12   AGC	Simplify headers.
*   23/08/13 1.11   SP 	Add renderSector() and 
*                       renderSectorGradient().
*   14/08/13 1.10   AGC	Add renderImage().
*			Add renderEllips().
*   25/01/13 1.9    AGC	Support polygons with gradients.
*   21/12/12 1.8    AGC	Add createPopupMenu().
*   09/11/12 1.7    AGC	Add GetDestination().
*   02/08/12 1.6    SP 	Add getBestInterpType().
*   06/06/12 1.5    SP 	Add a GdkGC.
*			Make some fields protected.
*   08/02/12 1.4    AGC	Support alpha in colours.
*			Configurable dash style.
*   20/12/11 1.3    SP 	Add renderPoint().
*   26/04/11 1.2    SP 	Read view size from base class.
*                       Implement new rendering functions.
*   06/04/11 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_RENDERER_GTK_H
#define _SPX_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibX11/SPxRendererCairo.h"

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
class SPxRendererCairo;
class SPxSoftButton;
struct _GdkDrawable;
struct _GdkGC;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxRendererGtk : virtual public SPxRendererCairo
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRendererGtk(void);
    virtual ~SPxRendererGtk(void);

    /* Set the destination drawable. */
    SPxErrorCode SetDestination(struct _GdkDrawable *drawable);
    struct _GdkDrawable *GetDestination(void) { return m_gdkDrawable; }

    /* For backward compatibility. */
    SPxErrorCode SetDestination(struct _cairo *cr) { return SetCairo(cr); }

protected:
    /*
     * Protected functions.
     */

    virtual SPxErrorCode getMaxScreenSize(unsigned int *width,
                                          unsigned int *height);

    virtual int getBestInterpType(double xScale, double yScale=1.0);

protected:
    /*
     * Protected variables.
     */

    /* Handle of the drawable we're rendering into.  Use the opaque one
     * so that this header doesn't need to drag in gdk.h.
     */
    struct _GdkDrawable *m_gdkDrawable;

    /* A GC that may be used to render directly to m_gdkDrawable. */
    struct _GdkGC *m_gc;

    /* Cairo context created by this class. */
    struct _cairo *m_localCr;

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */

}; /* SPxRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
