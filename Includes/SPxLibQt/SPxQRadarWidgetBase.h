/*********************************************************************
 *
 * (c) Copyright 2013, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxQRadarWidgetBase.h,v $
 * ID: $Id: SPxQRadarWidgetBase.h,v 1.4 2013/10/08 11:41:05 rew Exp $
 *
 * Purpose:
 *   SPxQRadarWidget base class header file.
 *
 * Revision Control:
 *   08/10/13 v1.4    AGC	Include SPxTypes.h for FALSE definition.
 *
 * Previous Changes:
 *   07/10/13 1.3    AGC    Simplify headers.
 *   05/09/13 1.2    AGC    Fix clang warning.
 *   06/06/13 1.1    SP     Initial version.
 *
 *********************************************************************/

/* Avoid multiple inclusion. */
#ifndef SPX_Q_RADAR_WIDGET_BASE_H
#define SPX_Q_RADAR_WIDGET_BASE_H

/* SPx headers. */
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

/* Forward declarations. */
class SPxScSourceLocal;
class SPxViewControl;

/* SPxQRadarWidgetBase class. */
class SPxQRadarWidgetBase
{
public:

    /*
     * Public functions.
     */

    /* Constructors and destructor. */
    SPxQRadarWidgetBase();
    virtual ~SPxQRadarWidgetBase(void);

    /* Add radar scan converter. */
    SPxErrorCode addRadar(SPxScSourceLocal *sc);

    /* Set underlay and overlay pixmaps. */
    virtual SPxErrorCode setUlayPixmap(QPixmap *pixmap);
    virtual SPxErrorCode setOlayPixmap(QPixmap *pixmap);

    /* Configure view. */
    virtual SPxErrorCode setView(double x, double y,
                                 double w, double h);

    virtual SPxErrorCode getView(double *x, double *y,
                                 double *w, double *h);

    virtual SPxErrorCode panView(double x, double c, int clearTrails=FALSE);

    /* Attach to view control. */
    virtual SPxErrorCode setViewControl(SPxViewControl *obj,
                                        bool enableMousePan=FALSE,
                                        bool enableMouseZoom=FALSE);

    /* Composite the display (pure virtual function). */
    virtual SPxErrorCode compositeDisplay(void)=0;

protected:

    /*
     * Protected functions.
     */

    /* Event handlers. */
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

    /*
     * Protected variables.
     */

    /* Underlay graphics. */
    QPixmap *m_ulayPixmap;

    /* Overlay graphics. */
    QPixmap *m_olayPixmap;

private:

    /*
     * Private types.
     */

    /* Use to hold scan converter info in an array. */
    typedef struct
    {
        SPxScSourceLocal *sc;

    } ScInfo_t;

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

    static int viewControlHandler(void *invokingObj,
                                  void *contextPtr,
                                  void *changeFlagsPtr);

    /*
     * Private variables.
     */

    /* List of scan converters. */
    std::vector<ScInfo_t> m_scList;

    /* Current window size. */
    unsigned int m_winWidth;
    unsigned int m_winHeight;

    /* View in metres. */
    double m_viewXMetres;
    double m_viewYMetres;
    double m_viewWMetres;
    double m_viewHMetres;

    /* Installed view control. */
    SPxViewControl *m_viewCtrl;
    bool m_isMousePanEnabled;
    bool m_isMouseZoomEnabled;

    /* Control variables for view panning. */
    bool m_isViewPanInProgress;
    QPoint m_viewPanAnchor;
};

#endif /* SPX_Q_RADAR_WIDGET_BASE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
