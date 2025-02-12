/*********************************************************************
*
* (c) Copyright 2011 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxRendererWin class which supports rendering
*   of graphics in a MS Windows application.
*
*
* Revision Control:
*   05/03/21 v1.35   AGC	Replace offscreen bitmap composite option with alpha option.
*
* Previous Changes:
*   26/02/21 1.34   BTB	Support setting compositing mode on offscreen bitmaps.
*   24/09/20 1.33   AGC	Support enable/disable anti-aliasing.
*   13/12/18 1.32   BTB	Support adjusting text font size.
*   06/12/18 1.31   BTB	Add renderSectorPartialGradient().
*   30/11/18 1.30   AGC	Support offscreen bitmaps.
*   26/11/18 1.29   AGC	Support bold/italic text.
*   01/11/18 1.28   AGC	Support setting HWND for per-monitor high DPI.
*   04/12/15 1.27   REW	Support mouseOverBitmap in SPxSoftButtons.
*   12/06/15 1.26   AGC	Support user specified soft button sizes.
*   09/02/15 1.25   AGC	Support automatic soft button management.
*   24/11/14 1.24   SP 	Add setClipRectangle().
*   20/10/14 1.23   REW	Add getPointWidth().
*   18/07/14 1.22   AGC	Add GetDeviceScale() function.
*   02/06/14 1.21   AGC	Scale text according to DPI settings.
*   08/05/14 1.20   AGC	Reduce memory allocations.
*   06/12/13 1.19   AGC	Avoid memory allocations when rendering.
*   24/10/13 1.18   AGC	Add GetGraphics() function.
*   08/10/13 1.17   AGC	Remove createPopupMenu() function.
*   07/10/13 1.16   AGC	Support new forms for renderArc(),
*			renderSector() and renderSectorGradient().
*   04/10/13 1.15   AGC	Simplify headers.
*   23/08/13 1.14   SP 	Add renderSector() and renderSectorGradient().
*   14/08/13 1.13   AGC	Add renderImage().
*			Add renderEllipse().
*   24/05/13 1.12   AGC	Fix WINVER undefined warning.
*   25/01/13 1.11   AGC	Support polygons with gradients.
*   21/12/12 1.10   AGC	Add createPopupMenu().
*   12/12/12 1.9    AGC	Make SetDestination() virtual.
*   21/08/12 1.8    SP 	Add m_smoothingMode.
*   21/06/12 1.7    SP 	Make m_hdc protected.
*   08/02/12 1.6    AGC	Configurable dash style.
*   13/01/12 1.5    SP 	Add GetDestination().
*   20/12/11 1.4    SP 	Add renderPoint().
*   27/07/11 1.3    SP 	Add SetTextRenderingMode().
*   26/04/11 1.2    SP 	Read view size from base class.
*                       Implement new rendering functions.
*   06/04/11 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_RENDERER_WIN_H
#define _SPX_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxError.h"
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

/* Need to forward-declare other classes in case headers 
 * are in wrong order. 
*/
class SPxRenderer;

namespace Gdiplus
{
    class Graphics;
    class Pen;
    class PointF;
    class SolidBrush;
    class FontFamily;
    class Font;
    class Bitmap;
    class ImageAttributes;
};
namespace ATL
{
    class CImage;
};

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRendererWin : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRendererWin(void);
    virtual ~SPxRendererWin(void);

    /* Set/get destination to render to */
    virtual SPxErrorCode SetDestination(HDC hdc, HWND hwnd=NULL);
    HDC GetDestination(void) { return m_hdc; }

    /* Set text rendering mode */
    SPxErrorCode SetTextRenderingMode(SPxTextRenderingMode_t mode);

    /* Set anti-aliasing mode. */
    SPxErrorCode SetAntiAliasEnabled(int enable);

    /* Get graphics object. */
    Gdiplus::Graphics *GetGraphics(void) const { return m_gfx; }

    virtual float GetDeviceScale(void) const;

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
    virtual SPxErrorCode renderSectorPartialGradient(UINT32 colour[2], 
						     SPxRendererXY_t *centre,
						     unsigned int innerRadius,
						     unsigned int outerRadius,
						     unsigned int fillRadius,
						     double startDegs, 
						     double sweepDegs);
    virtual SPxErrorCode setClipRectangle(SPxRendererXY_t *topLeft,
                                          SPxRendererXY_t *bottomRight);
    virtual SPxErrorCode endRendering(void);

    virtual SPxErrorCode createOffscreenBitmap(unsigned int index);
    virtual SPxErrorCode setOffscreenBitmapTarget(unsigned int index);
    virtual SPxErrorCode renderOffscreenBitmap(unsigned int index);
    virtual SPxErrorCode clearOffscreenBitmaps(void);
    virtual SPxErrorCode setOffscreenBitmapAlpha(unsigned int index, double alpha);

    /*
     * Protected variables.
     */

    /* Handle of the device context to use */
    HDC m_hdc;

    /* Window handle for per-monitor DPI scaling. */
    HWND m_hwnd;

    /* GDI+ drawing contexts */
    Gdiplus::Graphics *m_gfx;
    Gdiplus::Graphics *m_currGfx;

private:

    /*
     * Private functions.
     */
    virtual SPxSoftButton *newSoftButton(const char *bitmap,
						int width, int height,
						const char *moBitmap,
						int moWidth, int moHeight);
    double getLineWidth(void) const;
    double getPointWidth(void) const;
    void setDash(Gdiplus::Pen *pen);
    Gdiplus::Pen *getPen(UINT32 colour);
    Gdiplus::SolidBrush *getBrush(UINT32 colour);
    Gdiplus::FontFamily *getFontFamily(const char *fontFamily);
    Gdiplus::Font *getFont(const char *fontFamily, unsigned int fontSize,
			   SPxRendererFontStyle_t fontStyle);
    Gdiplus::ImageAttributes *getImageAttrib(UINT32 colour);
    static void setSoftButtonDest(SPxSoftButton *btn, void *userArg);

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;
    int m_smoothingMode;
    unsigned int m_pointCacheSize;
    SPxAutoPtr<Gdiplus::PointF[]> m_pointCache;

}; /* SPxRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
