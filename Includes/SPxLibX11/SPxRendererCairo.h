/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxRendererCairo class which supports rendering
*   of graphics in a Linux application using Cairo.
*
* Revision Control:
*   09/08/21 v1.4    SP 	Remove GDK/GTK+ dependency.
*
* Previous Changes:
*   06/08/21 1.3    SP 	Revert last change.
*   06/08/21 1.2    SP 	Add SetDestination() for backward compatibility.
*   06/08/21 1.1    SP 	Initial version created from SPxRenderGtk.
*
**********************************************************************/

#ifndef _SPX_RENDERER_CAIRO_H
#define _SPX_RENDERER_CAIRO_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibUtils/SPxRenderer.h"

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
class SPxRenderer;
class SPxSoftButton;
struct _cairo;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxRendererCairo : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRendererCairo(void);
    virtual ~SPxRendererCairo(void);

    /* Set the Cairo context. */
    SPxErrorCode SetCairo(struct _cairo *cr);
    struct _cairo *GetCairo(void) { return m_cr; }

protected:
    /*
     * Protected functions.
     */

    /* The following rendering functions implement functions
     * defined in the base class.
     */
    virtual SPxErrorCode startRendering(void);
    virtual SPxErrorCode renderPoly(UINT32 colour,
                                    unsigned int numPoints,
                                    SPxRendererXY_t *points,
                                    unsigned int filled);
    virtual SPxErrorCode renderPolyGradient(UINT32 colour[2],
                                            unsigned int numPoints,
                                            SPxRendererXY_t *points);
    virtual SPxErrorCode renderLine(UINT32 colour,
                                    SPxRendererXY_t *start,
                                    SPxRendererXY_t *end);
    virtual SPxErrorCode renderArc(UINT32 colour,
                                   SPxRendererXY_t *centre,
                                   unsigned int radiusX,
                                   unsigned int radiusY,
                                   double startDegs,
                                   double sweepDegs,
                                   unsigned int filled=FALSE);
    virtual SPxErrorCode renderEllipse(UINT32 colour,
                                       SPxRendererXY_t *topLeft,
                                       SPxRendererXY_t *bottomRight);
    virtual SPxErrorCode renderPoint(UINT32 colour,
                                     SPxRendererXY_t *point);
    virtual SPxErrorCode renderText(UINT32 colour,
                                    SPxRendererXY_t *point,
                                    const char *text,
                                    int adjustFontSize = TRUE);
    virtual SPxErrorCode getTextExtents(const char *text,
                                        double *widthRtn,
                                        double *heightRtn);
    virtual SPxErrorCode renderImage(UINT32 colour,
                                     SPxImage *image,
                                     SPxRendererXY_t *topLeft,
                                     SPxRendererXY_t *bottomRight,
                                     double rotDegs=0.0);
    virtual SPxErrorCode renderSector(UINT32 colour, 
                                      SPxRendererXY_t *centre,
                                      unsigned int innerRadius,
                                      unsigned int outerRadius,
                                      double startDegs, 
                                      double sweepDegs,
                                      unsigned int filled);
    virtual SPxErrorCode renderSectorGradient(UINT32 colour[2], 
                                              SPxRendererXY_t *centre,
                                              unsigned int innerRadius,
                                              unsigned int outerRadius,
                                              double startDegs,
                                              double sweepDegs);
    virtual SPxErrorCode setClipRectangle(SPxRendererXY_t *topLeft,
                                          SPxRendererXY_t *bottomRight);
    virtual SPxErrorCode endRendering(void);

    /* Floating point versions of above functions. */
    virtual SPxErrorCode renderPoly(UINT32 colour,
                                    unsigned int numPoints,
                                    SPxRendererXYD_t *points,
                                    unsigned int filled);
    virtual SPxErrorCode renderLine(UINT32 colour,
                                    SPxRendererXYD_t *start,
                                    SPxRendererXYD_t *end);
    virtual SPxErrorCode renderArc(UINT32 colour,
                                   SPxRendererXYD_t *centre,
                                   double radiusX,
                                   double radiusY,
                                   double startDegs,
                                   double sweepDegs,
                                   unsigned int filled);
    virtual SPxErrorCode renderEllipse(UINT32 colour,
                                       SPxRendererXYD_t *topLeft,
                                       SPxRendererXYD_t *bottomRight);
    virtual SPxErrorCode renderPoint(UINT32 colour,
                                     SPxRendererXYD_t *point);
    virtual SPxErrorCode renderText(UINT32 colour,
                                    SPxRendererXYD_t *point,
                                    const char *text,
                                    int adjustFontSize = TRUE);
    virtual SPxErrorCode renderImage(UINT32 colour,
                                     SPxImage *image,
                                     SPxRendererXYD_t *topLeft,
                                     SPxRendererXYD_t *bottomRight,
                                     double rotDegs=0.0);
    virtual SPxErrorCode renderSector(UINT32 colour, 
                                      SPxRendererXYD_t *centre,
                                      double innerRadius,
                                      double outerRadius,
                                      double startDegs, 
                                      double sweepDegs,
                                      unsigned int filled);

    virtual SPxErrorCode createOffscreenBitmap(unsigned int index);
    virtual SPxErrorCode setOffscreenBitmapTarget(unsigned int index);
    virtual SPxErrorCode renderOffscreenBitmap(unsigned int index);
    virtual SPxErrorCode clearOffscreenBitmaps(void);

    virtual SPxErrorCode getMaxScreenSize(unsigned int *width,
                                          unsigned int *height) { return SPX_ERR_NOT_SUPPORTED; }

    struct _cairo *getRenderer(void) { return m_cr; }

protected:
    /*
     * Protected variables.
     */

    /* Fields used during rendering.  Use opaque handles where possible
     * so the header doesn't need to drag in cairo.h etc.
     */
    struct _cairo *m_cr;
    struct _cairo *m_crInUse;

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */
    virtual SPxSoftButton *newSoftButton(const char *bitmap,
                                         int width, int height,
                                         const char *moBitmap,
                                         int moWidth, int moHeight);
    void setColour(UINT32 argb);
    void setDash(void);
    static void setSoftButtonDest(SPxSoftButton *btn, void *userArg);
    template<typename T>
    SPxErrorCode renderPolyT(UINT32 colour,
                             unsigned int numPoints,
                             T *points,
                             unsigned int filled);

}; /* SPxRendererCairo */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_RENDERER_CAIRO_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
