/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Range/altitude display platform independent base class.
 *
 * Revision Control:
 *   19/11/20 v1.5   SP 	Support Earth curvature.
 *
 * Previous Changes:
 *   13/11/20 1.4   SP 	Rename class.
 *   27/10/20 1.3   SP 	Rework to support view control.
 *   26/10/20 1.2   SP 	Further development.
 *   23/10/20 1.1   SP 	Initial version.
 *
 **********************************************************************/

#ifndef _SPX_RHI_H
#define _SPX_RHI_H

/* Library headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxMouseControl.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxRefPoint.h"
#include "SPxLibUtils/SPxViewControl.h" /* For SPxLatLong_t */

/* Forward declare any classes required. */
class SPxUniTrackDatabase;
class SPxRenderer;

/* SPxRhi dialog */
class SPxRhi : public SPxObj, public SPxMouseInterface
{
public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxRhi(unsigned int maxWPixels,
           unsigned int maxHPixels);
    virtual ~SPxRhi(void);

    /* Access. */
    int GetMaxWinWPixels(void) const { return m_maxWinWPixels; }
    int GetMaxWinHPixels(void) const { return m_maxWinHPixels; }
    int GetWinWPixels(void) const { return m_winWPixels; }
    int GetWinHPixels(void) const { return m_winHPixels; }

    /* Reference position. */
    int IsRefPointSet(void) { return m_isRefPointSet; }
    SPxErrorCode SetRefPosition(const SPxLatLong_t *ll, double altMetres);
    SPxErrorCode GetRefLatLong(SPxLatLong_t *llRtn) const;
    SPxErrorCode GetRefAltitude(double *altMetresRtn) const;

    /* View options. */
    SPxErrorCode SetEarthCurvatureEnabled(int isEnabled);
    int IsEarthCurvatureEnabled(void) { return m_earthCurvatureEnabled; }

    /* View control for range. */
    SPxErrorCode SetViewRangeCentre(double metres);
    double GetViewRangeCentre(void) const { return m_rangeCentreMetres; }
    SPxErrorCode SetViewRangeExtent(double metres);
    double GetViewRangeExtent(void) const { return m_rangeExtentMetres; }
    double GetViewRangeStart(void) const { return m_rangeStartMetres; }
    double GetViewRangeEnd(void) const { return m_rangeEndMetres; }
    double GetViewRangePpm(void) const { return m_rangePpm; } 
    SPxErrorCode ZoomViewRange(double factor);

    /* View control for altitude. */
    SPxErrorCode SetViewAltitudeCentre(double metres);
    double GetViewAltitudeCentre(void) const { return m_altCentreMetres; }
    SPxErrorCode SetViewAltitudeExtent(double metres);
    double GetViewAltitudeExtent(void) const { return m_altExtentMetres; }
    double GetViewAltitudeStart(void) const { return m_altStartMetres; }
    double GetViewAltitudeEnd(void) const { return m_altEndMetres; }
    double GetViewAltitudePpm(void) const { return m_altPpm; } 
    SPxErrorCode ZoomViewAltitude(double factor);

    /* Common view control. */
    SPxErrorCode ZoomView(double factor, UINT32 flags);
    SPxErrorCode ZoomViewAtPixel(double factor, int xPixels, int yPixels, UINT32 flags);
    SPxErrorCode PanViewPixels(int deltaXPixels, int deltaYPixels);

    /* Range conversion functions. */
    double RangeMetresToPixels(double metres) const;
    double RangePixelsToMetres(double xPixels) const;

    /* Altitude conversion functions. */
    double AltitudeMetresToPixels(double rangeMetres, double altMetres) const;
    double AltitudePixelsToMetres(double xPixels, double yPixels) const;

    /* Mouse handling. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseWheel(UINT32 flags, int x, int y, double delta);
    virtual int HandleMouseLeave(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    SPxErrorCode addRenderer(SPxRenderer *rdr);
    void redraw(int wPixels, int hPixels);

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    unsigned int m_maxWinWPixels;       /* Max window width. */
    unsigned int m_maxWinHPixels;       /* Max window height. */
    int m_winWPixels;                   /* Current window width. */
    int m_winHPixels;                   /* Current window height. */
    SPxRefPoint m_refPoint;             /* Reference point. */
    int m_isRefPointSet;                /* Reference point set? */

    /* View. */
    int m_earthCurvatureEnabled;
    double m_rangeCentreMetres;
    double m_rangeExtentMetres;
    double m_rangeStartMetres;
    double m_rangeEndMetres;
    double m_rangePpm;
    double m_rangeMinMetres;
    double m_altCentreMetres;
    double m_altExtentMetres;
    double m_altStartMetres;
    double m_altEndMetres;
    double m_altPpm;
    double m_altMinMetres;
    double m_isViewPanInProgress;
    int m_panXAnchorPixels;
    int m_panYAnchorPixels;

    /*
     * Private functions.
     */

    SPxErrorCode recalculateView(void);

    /*
     * Private static functions.
     */

}; /* Class SPxRhi. */

#endif /* SPX_RHI_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
