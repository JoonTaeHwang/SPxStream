/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Source for renderer base class which supports the display 
 *   of graphics in a range/altitude window, but only via a 
 *   derived class.
 *
 *   Note that this renderer cannot be used in a PPI window 
 *   and is not compatible with SPxViewControl.
 *
 * Revision Control:
 *   25/11/20 v1.6   SP 	Support configurable elevations.
 *
 * Previous Changes:
 *   24/11/20 1.5   SP 	Support lines of elevation.
 *   19/11/20 1.4   SP 	Support Earth curvature.
 *   13/11/20 1.3   SP 	Rename class.
 *   12/11/20 1.2   SP 	Make conversion functions public.
 *   27/10/20 1.1   SP 	Initial version.
 *
 **********************************************************************/

#ifndef _SPX_RHI_GRAPHICS_RENDERER_H
#define _SPX_RHI_GRAPHICS_RENDERER_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need our base class. */
#include "SPxLibUtils/SPxTrackRendererBase.h"

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

/* Need to forward-declare other classes we need.*/
class SPxRhi;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxRhiGraphicsRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRhiGraphicsRenderer(SPxRhi *view);
    virtual ~SPxRhiGraphicsRenderer(void);

    /* Conversion functions. */
    double RangeMetresToDu(double metres);
    double RangeDuToMetres(double metres);
    double AltitudeMetresToDu(double metres);
    double AltitudeDuToMetres(double metres);

    /* Features. */
    SPxErrorCode SetRangeGridEnabled(int isEnabled);
    int IsRangeGridEnabled(void) { return m_isRangeGridEnabled; }
    SPxErrorCode SetAltitudeGridEnabled(int isEnabled);
    int IsAltitudeGridEnabled(void) { return m_isAltGridEnabled; }
    SPxErrorCode SetElevationsEnabled(int isEnabled);
    int AreElevationsEnabled(void) { return m_areElevationsEnabled; }
    SPxErrorCode SetElevationsStr(const char *buf);
    SPxErrorCode GetElevationsStr(char *buf, unsigned int bufLen);

    /* Mouse handling. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseWheel(UINT32 flags, int x, int y, double delta);
    virtual int HandleMouseLeave(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

private:

    /*
     * Private types.
     */

    /* Label context. */
    struct Label_t
    {
        int isRendered;         /* Flag set once renderered. */
        UINT32 bgARGB;          /* Background colour. */
        UINT32 textARGB;        /* Text colour. */
        SPxRendererXYD_t tlPt;  /* Top-left point. */
        SPxRendererXYD_t brPt;  /* Bottom-right point. */
        char text[64];          /* Text to display. */
    };

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxCriticalSection m_mutex;
    SPxRhi *m_view;
    double m_winWPixels;
    double m_winHPixels;
    double m_earthRadiusXPx;
    double m_earthRadiusYPx;
    SPxRendererXYD_t m_earthCentrePt;
    int m_isRangeGridEnabled;
    int m_isAltGridEnabled;
    int m_areElevationsEnabled;

    /* Colours. */
    UINT32 m_backgroundARGB;
    UINT32 m_textARGB;
    UINT32 m_axesARGB;
    UINT32 m_priMarkerARGB;
    UINT32 m_secMarkerARGB;
    UINT32 m_elevationsARGB;

    /*
     * Private functions.
     */

    /* Rendering functions. */
    SPxErrorCode renderAll(void);
    void preRenderSetup(void);
    void renderRangeMarkers(void);
    void renderAltitudeMarkers(void);
    void renderElevations(void);
    void renderAxes(void);
    void renderLabels(void);

    /* General functions. */
    void getRangeLabel(double metres, char *buf, unsigned int bufLen);
    void getAltitudeLabel(double metres, char *buf, unsigned int bufLen);
    int isLabelOverlap(const Label_t *label1) const;

    /*
     * Private static functions.
     */

}; /* SPxRhiGraphicsRenderer */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_RHI_GRAPHICS_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
