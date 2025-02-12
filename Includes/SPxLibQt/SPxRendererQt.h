/*********************************************************************
*
* (c) Copyright 2015 - 2018, Cambridge Pixel Ltd.
*
*
* Purpose:
*   Header file for a SPxRendererQt class.
*
*
* Revision Control:
*   18/12/18 v1.9    BTB	Fix formatting and bug.
*
* Previous Changes:
*   17/12/18 1.8    BTB	Support adjusting text font size.
*   04/12/15 1.7    REW	Support mouseOverBitmap in SPxSoftButtons.
*   12/06/15 1.6    AGC	Support user specified soft button sizes.
*   24/04/15 1.5    JP	Add setDash declaration.
*   09/02/15 1.4    AGC	Support base class soft button management.
*   02/02/15 1.3    JP	Warning fixed.
*   16/01/15 1.2    JP	Add comments to the code.
*   07/01/15 1.1    JP	Initial version.
**********************************************************************/

#ifndef _SPX_RENDERER_QT_H
#define _SPX_RENDERER_QT_H

/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxRenderer.h"

/* Qt headers. */
#include <QPainter>
#include <QPaintDevice>

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxRenderer;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxRendererQt : virtual public SPxRenderer
{

public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRendererQt(void);
    virtual ~SPxRendererQt(void);

    /* Set/get destination to render to */
    virtual SPxErrorCode SetDestination(QPaintDevice *paintDevice);
    QPaintDevice* GetDestination(void);

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
				   unsigned int filled);
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

    /* Handler of the device context to use. */
    QPaintDevice *m_paintDevice;

    QPointF* m_points;
    unsigned int m_nPoints;

    virtual SPxSoftButton *newSoftButton(const char *bitmap,
						int width, int height,
						const char *moBitmap,
						int moWidth, int moHeight);
    static void setSoftButtonDest(SPxSoftButton *btn, void *userArg);

private:
    /* 
     * Private functions.
     */
    void setDash(QPen &pen);
};

#endif /* SPX_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
