/*********************************************************************
*
* (c) Copyright 2019, 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxRendererWpf class which supports rendering
*   of graphics in a .NET WPF Windows application.
*
*
* Revision Control:
*   17/02/22 v1.5    AGC	Support renderPoly() overload taking angle.
*
* Previous Changes:
*   01/04/21 1.4    AGC	Support renderLine() overload taking length and angle.
*   31/10/19 1.3    AGC	Support rendering rotated text.
*   08/10/19 1.2    AGC	Add set active wrapper.
*   24/09/13 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxRenderer.h"

 /*********************************************************************
 *
 *   Constants
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
class SPxSoftButton;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRendererWpf : virtual public SPxRenderer
{
public:
    typedef void (*SetActiveFn)(int active);
    typedef SPxErrorCode (*StartRenderingFn)(void);
    typedef SPxErrorCode (*RenderPolyFn)(UINT32 colour,
			    unsigned int numPoints,
			    SPxRendererXY_t *points,
			    unsigned int filled);
    typedef SPxErrorCode (*RenderPolyDFn)(UINT32 colour,
			    unsigned int numPoints,
			    SPxRendererXYD_t *points,
			    unsigned int filled);
    typedef SPxErrorCode (*RenderPolyDAFn)(UINT32 colour,
			    unsigned int numPoints,
			    SPxRendererXYD_t *points,
			    unsigned int filled,
			    double angleDegs);
    typedef SPxErrorCode (*RenderPolyGradientFn)(UINT32 colour[2],
				    unsigned int numPoints,
				    SPxRendererXY_t *points);
    typedef SPxErrorCode (*RenderLineFn)(UINT32 colour,
			    SPxRendererXY_t *start,
			    SPxRendererXY_t *end);
    typedef SPxErrorCode (*RenderLineDFn)(UINT32 colour,
			    SPxRendererXYD_t *start,
			    SPxRendererXYD_t *end);
    typedef SPxErrorCode (*RenderLineDLAFn)(UINT32 colour,
			    SPxRendererXYD_t *start,
			    SPxRendererXYD_t *end,
			    double lengthPx,
			    double angleDegs);
    typedef SPxErrorCode (*RenderArcFn)(UINT32 colour,
			   SPxRendererXY_t *centre,
			   unsigned int radiusX,
			   unsigned int radiusY,
			   double startDegs,
			   double sweepDegs,
			   unsigned int filled);
    typedef SPxErrorCode (*RenderArcDFn)(UINT32 colour,
			   SPxRendererXYD_t *centre,
			   double radiusX,
			   double radiusY,
			   double startDegs,
			   double sweepDegs,
			   unsigned int filled);
    typedef SPxErrorCode (*RenderEllipseFn)(UINT32 colour,
			       SPxRendererXY_t *topLeft,
			       SPxRendererXY_t *bottomRight);
    typedef SPxErrorCode (*RenderEllipseDFn)(UINT32 colour,
			       SPxRendererXYD_t *topLeft,
			       SPxRendererXYD_t *bottomRight);
    typedef SPxErrorCode (*RenderPointFn)(UINT32 colour,
			     SPxRendererXY_t *point);
    typedef SPxErrorCode (*RenderPointDFn)(UINT32 colour,
			     SPxRendererXYD_t *point);
    typedef SPxErrorCode (*RenderTextFn)(UINT32 colour,
			    SPxRendererXY_t *point,
			    const char *text,
			    int adjustFontSize);
    typedef SPxErrorCode (*RenderTextDFn)(UINT32 colour,
			    SPxRendererXYD_t *point,
			    const char *text,		    
			    int adjustFontSize);
    typedef SPxErrorCode (*RenderTextRotFn)(UINT32 colour,
			    SPxRendererXY_t *point,
			    const char *text,
			    double rotationDegs,
			    SPxRendererXY_t *centre,
			    int adjustFontSize);
    typedef SPxErrorCode (*RenderTextRotDFn)(UINT32 colour,
			    SPxRendererXYD_t *point,
			    const char *text,
			    double rotationDegs,
			    SPxRendererXYD_t *centre,
			    int adjustFontSize);
    typedef SPxErrorCode (*GetTextExtentsFn)(const char *text,
				double *widthRtn,
				double *heightRtn);
    typedef SPxErrorCode (*RenderImageFn)(UINT32 colour,
			     SPxImage *image,
			     SPxRendererXY_t *topLeft,
			     SPxRendererXY_t *bottomRight,
			     double rotDegs);
    typedef SPxErrorCode (*RenderImageTransFn)(UINT32 colour,
			     SPxImage *image,
			     const SPxRendererXY_t *topLeft,
			     const SPxRendererXY_t *bottomRight,
			     const SPxRendererXYD_t bitmapPoints[4]);
    typedef SPxErrorCode (*RenderImageDFn)(UINT32 colour,
			     SPxImage *image,
			     SPxRendererXYD_t *topLeft,
			     SPxRendererXYD_t *bottomRight,
			     double rotDegs);
    typedef SPxErrorCode (*RenderImageTransDFn)(UINT32 colour,
			     SPxImage *image,
			     const SPxRendererXYD_t *topLeft,
			     const SPxRendererXYD_t *bottomRight,
			     const SPxRendererXYD_t bitmapPoints[4]);
    typedef SPxErrorCode (*RenderSectorFn)(UINT32 colour,
			      SPxRendererXY_t *centre,
			      unsigned int innerRadius,
			      unsigned int outerRadius,
			      double startDegs,
			      double sweepDegs,
			      unsigned int filled);
    typedef SPxErrorCode (*RenderSectorDFn)(UINT32 colour,
			      SPxRendererXYD_t *centre,
			      double innerRadius,
			      double outerRadius,
			      double startDegs,
			      double sweepDegs,
			      unsigned int filled);
    typedef SPxErrorCode (*RenderSectorGradientFn)(UINT32 colour[2],
				      SPxRendererXY_t *centre,
				      unsigned int innerRadius,
				      unsigned int outerRadius,
				      double startDegs,
				      double sweepDegs);
    typedef SPxErrorCode (*RenderSectorPartialGradientFn)(UINT32 colour[2],
					     SPxRendererXY_t *centre,
					     unsigned int innerRadius,
					     unsigned int outerRadius,
					     unsigned int fillRadius,
					     double startDegs,
					     double sweepDegs);
    typedef SPxErrorCode (*SetClipRectangleFn)(SPxRendererXY_t *topLeft,
				  SPxRendererXY_t *bottomRight);
    typedef SPxErrorCode (*EndRenderingFn)(void);
    typedef SPxErrorCode (*CreateOffscreenBitmapFn)(unsigned int index);
    typedef SPxErrorCode (*SetOffscreenBitmapTargetFn)(unsigned int index);
    typedef SPxErrorCode (*RenderOffscreenBitmapFn)(unsigned int index);
    typedef SPxErrorCode (*ClearOffscreenBitmapsFn)(void);
    typedef SPxSoftButton *(*NewSoftButtonFn)(const char *bitmap,
					 int width, int height,
					 const char *moBitmap,
					 int moWidth, int moHeight);

    /* Constructor and destructor. */
    SPxRendererWpf(void);
    virtual ~SPxRendererWpf(void);

    virtual void SetActive(int active=TRUE);

    void InstallSetActiveFn(SetActiveFn setActiveFn);
    void InstallStartRenderingFn(StartRenderingFn startRenderingFn);
    void InstallRenderPolyFn(RenderPolyFn renderPolyFn);
    void InstallRenderPolyDFn(RenderPolyDFn renderPolyDFn);
    void InstallRenderPolyDAFn(RenderPolyDAFn renderPolyDAFn);
    void InstallRenderPolyGradientFn(RenderPolyGradientFn renderPolyGradientFn);
    void InstallRenderLineFn(RenderLineFn renderLineFn);
    void InstallRenderLineDFn(RenderLineDFn renderLineDFn);
    void InstallRenderLineDLAFn(RenderLineDLAFn renderLineDLAFn);
    void InstallRenderArcFn(RenderArcFn renderArcFn);
    void InstallRenderArcDFn(RenderArcDFn renderArcDFn);
    void InstallRenderEllipseFn(RenderEllipseFn renderEllipseFn);
    void InstallRenderEllipseDFn(RenderEllipseDFn renderEllipseDFn);
    void InstallRenderPointFn(RenderPointFn renderPointFn);
    void InstallRenderPointDFn(RenderPointDFn renderPointDFn);
    void InstallRenderTextFn(RenderTextFn renderTextFn);
    void InstallRenderTextDFn(RenderTextDFn renderTextDFn);
    void InstallRenderTextRotFn(RenderTextRotFn renderTextRotFn);
    void InstallRenderTextRotDFn(RenderTextRotDFn renderTextRotDFn);
    void InstallGetTextExtentsFn(GetTextExtentsFn getTextExtentsFn);
    void InstallRenderImageFn(RenderImageFn renderImageFn);
    void InstallRenderImageTransFn(RenderImageTransFn renderImageTransFn);
    void InstallRenderImageDFn(RenderImageDFn renderImageDFn);
    void InstallRenderImageTransDFn(RenderImageTransDFn renderImageTransDFn);
    void InstallRenderSectorFn(RenderSectorFn renderSectorFn);
    void InstallRenderSectorDFn(RenderSectorDFn renderSectorDFn);
    void InstallRenderSectorGradientFn(RenderSectorGradientFn renderSectorGradientFn);
    void InstallRenderSectorPartialGradientFn(RenderSectorPartialGradientFn renderSectorPartialGradientFn);
    void InstallSetClipRectangleFn(SetClipRectangleFn setClipRectangleFn);
    void InstallEndRenderingFn(EndRenderingFn endRenderingFn);
    void InstallCreateOffscreenBitmapFn(CreateOffscreenBitmapFn createOffscreenBitmapFn);
    void InstallSetOffscreenBitmapTargetFn(SetOffscreenBitmapTargetFn setOffscreenBitmapTargetFn);
    void InstallRenderOffscreenBitmapFn(RenderOffscreenBitmapFn renderOffscreenBitmapFn);
    void InstallClearOffscreenBitmapsFn(ClearOffscreenBitmapsFn clearOffscreenBitmapsFn);
    void InstallNewSoftButtonFn(NewSoftButtonFn newSoftButtonFn);

protected:

    virtual SPxErrorCode startRendering(void);
    virtual SPxErrorCode renderPoly(UINT32 colour,
				    unsigned int numPoints,
				    SPxRendererXY_t *points,
				    unsigned int filled);
    virtual SPxErrorCode renderPoly(UINT32 colour,
				    unsigned int numPoints,
				    SPxRendererXYD_t *points,
				    unsigned int filled);
    virtual SPxErrorCode renderPoly(UINT32 colour,
				    unsigned int numPoints,
				    SPxRendererXYD_t *points,
				    unsigned int filled,
				    double angleDegs);
    virtual SPxErrorCode renderPolyGradient(UINT32 colour[2],
					    unsigned int numPoints,
					    SPxRendererXY_t *points);
    virtual SPxErrorCode renderLine(UINT32 colour,
				    SPxRendererXY_t *start,
				    SPxRendererXY_t *end);
    virtual SPxErrorCode renderLine(UINT32 colour,
				    SPxRendererXYD_t *start,
				    SPxRendererXYD_t *end);
    virtual SPxErrorCode renderLine(UINT32 colour,
				    SPxRendererXYD_t *start,
				    SPxRendererXYD_t *end,
				    double lengthPx,
				    double angleDegs);
    virtual SPxErrorCode renderArc(UINT32 colour,
				   SPxRendererXY_t *centre,
				   unsigned int radiusX,
				   unsigned int radiusY,
				   double startDegs,
				   double sweepDegs,
				   unsigned int filled = FALSE);
    virtual SPxErrorCode renderArc(UINT32 colour,
				   SPxRendererXYD_t *centre,
				   double radiusX,
				   double radiusY,
				   double startDegs,
				   double sweepDegs,
				   unsigned int filled = FALSE);
    virtual SPxErrorCode renderEllipse(UINT32 colour,
				       SPxRendererXY_t *topLeft,
				       SPxRendererXY_t *bottomRight);
    virtual SPxErrorCode renderEllipse(UINT32 colour,
				       SPxRendererXYD_t *topLeft,
				       SPxRendererXYD_t *bottomRight);
    virtual SPxErrorCode renderPoint(UINT32 colour,
				     SPxRendererXY_t *point);
    virtual SPxErrorCode renderPoint(UINT32 colour,
				     SPxRendererXYD_t *point);
    virtual SPxErrorCode renderText(UINT32 colour,
				    SPxRendererXY_t *point,
				    const char *text,
				    int adjustFontSize = TRUE);
    virtual SPxErrorCode renderText(UINT32 colour,
				    SPxRendererXYD_t *point,
				    const char *text,
				    int adjustFontSize = TRUE);
    virtual SPxErrorCode renderText(UINT32 colour,
				    SPxRendererXY_t *point,
				    const char *text,
				    double rotationDegs,
				    SPxRendererXY_t *centre,
				    int adjustFontSize = TRUE);
    virtual SPxErrorCode renderText(UINT32 colour,
				    SPxRendererXYD_t *point,
				    const char *text,
				    double rotationDegs,
				    SPxRendererXYD_t *centre,
				    int adjustFontSize = TRUE);
    virtual SPxErrorCode getTextExtents(const char *text,
					double *widthRtn,
					double *heightRtn);
    virtual SPxErrorCode renderImage(UINT32 colour,
				     SPxImage *image,
				     SPxRendererXY_t *topLeft,
				     SPxRendererXY_t *bottomRight,
				     double rotDegs);
    virtual SPxErrorCode renderImage(UINT32 colour,
				     SPxImage *image,
				     const SPxRendererXY_t *topLeft,
				     const SPxRendererXY_t *bottomRight,
				     const SPxRendererXYD_t bitmapPoints[4]);
    virtual SPxErrorCode renderImage(UINT32 colour,
				     SPxImage *image,
				     SPxRendererXYD_t *topLeft,
				     SPxRendererXYD_t *bottomRight,
				     double rotDegs);
    virtual SPxErrorCode renderImage(UINT32 colour,
				     SPxImage *image,
				     const SPxRendererXYD_t *topLeft,
				     const SPxRendererXYD_t *bottomRight,
				     const SPxRendererXYD_t bitmapPoints[4]);
    virtual SPxErrorCode renderSector(UINT32 colour,
				      SPxRendererXY_t *centre,
				      unsigned int innerRadius,
				      unsigned int outerRadius,
				      double startDegs,
				      double sweepDegs,
				      unsigned int filled);
    virtual SPxErrorCode renderSector(UINT32 colour,
				      SPxRendererXYD_t *centre,
				      double innerRadius,
				      double outerRadius,
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

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */
    virtual SPxSoftButton *newSoftButton(const char *bitmap,
					 int width, int height,
					 const char *moBitmap,
					 int moWidth, int moHeight);

}; /* SPxRendererWpf */

/*********************************************************************
*
* End of file
*
**********************************************************************/
