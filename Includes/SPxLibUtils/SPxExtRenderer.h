/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Object for rendering using standard rendering interface with
*   an external renderer.
*
*
* Revision Control:
*   09/02/22 v1.21   AGC	Support destroying soft buttons.
*
* Previous Changes:
*   30/11/18 1.20   AGC	Allow access to window size.
*   03/05/18 1.19   BTB	Support mouse events a better way.
*   02/05/18 1.18   AGC	Support mouse events.
*   27/02/18 1.17   AGC	Add double version of RenderPoint().
*			Add double version of RenderImage().
*   31/08/17 1.16   AGC	Add double version of RenderArc().
*			Add double version of RenderSector().
*   01/08/17 1.15   AGC	Add double version of RenderPoly().
*			Add double version of RenderText().
*   22/06/17 1.14   AGC	Add double version of RenderLine().
*   19/10/16 1.13   AGC	Add double version of RenderRectangle().
*   29/09/16 1.12   REW	Wrap DPI scale functions.
*   20/04/16 1.11   AGC	Support retrieving paused state.
*   22/02/16 1.10   AGC	Support retrieving user time.
*   07/01/16 1.9    AGC	Add floating point overload for RenderEllipse().
*   22/12/15 1.8    REW	Add wrappers for display units functions.
*   12/06/15 1.7    AGC	Support user specified soft button sizes.
*   14/04/15 1.6    AGC	Add new perspective RenderImage() overload.
*   20/02/15 1.5    AGC	Remove public soft button functions.
*			Allow SPxAVRender to call private createSoftButton().
*   20/02/15 1.4    REW	Add int variant of CreateSoftButton().
*   09/02/15 1.3    AGC	Expose new renderer soft button functions.
*   04/11/13 1.2    AGC	Move SPxRendererObj to new header.
*   07/10/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_EXT_RENDERER_H
#define _SPX_EXT_RENDERER_H

/*
 * Other headers required.
 */

/* For SPxTime_t. */
#include "SPxLibUtils/SPxTime.h"

/* For base class. */
#include "SPxLibUtils/SPxRenderer.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxRendererObj;

/* External renderer. */
class SPxExtRenderer : virtual public SPxRenderer
{
public:
    /* Constructor and destructor */
    explicit SPxExtRenderer(SPxRendererObj *obj);
    virtual ~SPxExtRenderer(void);

    SPxErrorCode StartRendering(void);
    SPxErrorCode EndRendering(void);
    SPxErrorCode RenderPoly(UINT32 colour,
                            unsigned int numPoints,
                            SPxRendererXY_t *points,
                            unsigned int filled);
    SPxErrorCode RenderPoly(UINT32 colour,
                            unsigned int numPoints,
                            SPxRendererXYD_t *points,
                            unsigned int filled);
    SPxErrorCode RenderPolyGradient(UINT32 colour[2],
                                    unsigned int numPoints,
                                    SPxRendererXY_t *points);
    SPxErrorCode RenderLine(UINT32 colour,
                            SPxRendererXY_t *start,
                            SPxRendererXY_t *end);
    SPxErrorCode RenderLine(UINT32 colour,
                            SPxRendererXYD_t *start,
                            SPxRendererXYD_t *end);
    SPxErrorCode RenderArc(UINT32 colour,
                           SPxRendererXY_t *centre,
                           unsigned int radiusX,
                           unsigned int radiusY,
                           double startDegs,
                           double sweepDegs,
                           unsigned int filled=FALSE);
    SPxErrorCode RenderArc(UINT32 colour,
                           SPxRendererXYD_t *centre,
                           double radiusX,
                           double radiusY,
                           double startDegs,
                           double sweepDegs,
                           unsigned int filled = FALSE);
    SPxErrorCode RenderEllipse(UINT32 colour,
                               SPxRendererXY_t *topLeft,
                               SPxRendererXY_t *bottomRight);
    SPxErrorCode RenderEllipse(UINT32 colour,
                               SPxRendererXYD_t *topLeft,
                               SPxRendererXYD_t *bottomRight);
    SPxErrorCode RenderPoint(UINT32 colour,
                             SPxRendererXY_t *point);
    SPxErrorCode RenderPoint(UINT32 colour,
                             SPxRendererXYD_t *point);
    SPxErrorCode RenderText(UINT32 colour,
                            SPxRendererXY_t *point,
                            const char *text);
    SPxErrorCode RenderText(UINT32 colour,
                            SPxRendererXYD_t *point,
                            const char *text);
    SPxErrorCode GetTextExtents(const char *text, 
                                double *widthRtn,
                                double *heightRtn);
    SPxErrorCode RenderImage(UINT32 colour,
                             SPxImage *image,
                             SPxRendererXY_t *topLeft,
                             SPxRendererXY_t *bottomRight,
                             double rotDegs=0.0);
    SPxErrorCode RenderImage(UINT32 colour,
                             SPxImage *image,
                             const SPxRendererXY_t *topLeft,
                             const SPxRendererXY_t *bottomRight,
                             const SPxRendererXYD_t bitmapPoints[4]);
    SPxErrorCode RenderImage(UINT32 colour,
                             SPxImage *image,
                             SPxRendererXYD_t *topLeft,
                             SPxRendererXYD_t *bottomRight,
                             double rotDegs=0.0);
    SPxErrorCode RenderSector(UINT32 colour, 
                              SPxRendererXY_t *centre,
                              unsigned int innerRadius,
                              unsigned int outerRadius,
                              double startDegs, 
                              double sweepDegs,
                              unsigned int filled);
    SPxErrorCode RenderSector(UINT32 colour, 
                              SPxRendererXYD_t *centre,
                              double innerRadius,
                              double outerRadius,
                              double startDegs, 
                              double sweepDegs,
                              unsigned int filled);
    SPxErrorCode RenderSectorGradient(UINT32 colour[2], 
                                      SPxRendererXY_t *centre,
                                      unsigned int innerRadius,
                                      unsigned int outerRadius,
                                      double startDegs, 
                                      double sweepDegs);
    SPxErrorCode RenderRectangle(UINT32 colour,
                                 SPxRendererXY_t *topLeft,
                                 SPxRendererXY_t *bottomRight,
                                 unsigned int filled);
    SPxErrorCode RenderRectangle(UINT32 colour,
                                 SPxRendererXYD_t *topLeft,
                                 SPxRendererXYD_t *bottomRight,
                                 unsigned int filled);

    /* Display units: Provide access to protected functions in base class. */
    double MetresToDisplayUnits(double metres);
    double DisplayUnitsToMetres(double du);
    const char *GetDisplayUnitDistText(void);
    SPxErrorCode FormatBearingText(char *buf, int bufLen,
                                   double bearing, double resolution);
    double MpsToDisplayUnits(double mps);
    double DisplayUnitsToMps(double du);
    const char *GetDisplayUnitSpeedText(void);

    /* Set/get a user specified time. */
    SPxErrorCode SetUserTime(const SPxTime_t *time);
    SPxErrorCode GetUserTime(SPxTime_t *time) const;

    /* Utility functions for DPI scaling. */
    int GetDpiScaledSize(int size) const;
    unsigned int GetDpiScaledSize(unsigned int size) const;
    float GetDpiScaledSize(float size) const;
    double GetDpiScaledSize(double size) const;

    SPxErrorCode SetPaused(int paused);
    int IsPaused(void) const;

    SPxErrorCode SetWindowSizePixels(unsigned int winWidth,
				     unsigned int winHeight);
    unsigned int GetWindowWidthPixels(void) const { return m_winWidthPixels; }
    unsigned int GetWindowHeightPixels(void) const { return m_winHeightPixels; }

private:
    /* Private functions. */
    SPxSoftButton *createSoftButton(int bitmap, int width=-1, int height=-1);
    SPxSoftButton *createSoftButton(const char *bitmap, int width=-1, int height=-1);
    SPxErrorCode destroySoftButton(SPxSoftButton *btn);
    virtual SPxErrorCode renderAll(void);

    /* Private variables. */
    SPxRendererObj *m_obj;
    int m_isUserTimeValid;
    SPxTime_t m_userTime;
    int m_isPaused;
    int m_wasPaused;
    unsigned int m_winWidthPixels;
    unsigned int m_winHeightPixels;

    /* Allow SPxAVRender to call private createSoftButton(). */
    friend class SPxAVRender;

}; /* SPxExtRenderer */

#endif /* _SPX_EXT_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
