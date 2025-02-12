/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxHistogramX11.h,v $
* ID: $Id: SPxHistogramX11.h,v 1.6 2015/10/01 11:37:41 rew Exp $
*
* Purpose:
*   Header for histogram windows in X11.
*
* Revision Control:
*   01/10/15 v1.6    AGC	Move std::vector usage into source file.
*
* Previous Changes:
*   27/03/15 1.5   JP  Add on mouse leave event.
*   26/03/15 1.4    REW	Fix build issue in last version.
*   26/03/15 1.3    JP 	Add comments to the code.
*   17/03/15 1.2    JP  Changes during implementation.
*   26/02/15 1.1    JP 	Initial Version
**********************************************************************/
#ifndef _SPX_HISTOGRAMX11_H
#define _SPX_HISTOGRAMX11_H

/* Base class header. */
#include "SPxLibUtils/SPxHistogram.h"

/* Standard headers. */

/* Forward declarations. */
class SPxHistogramBar;

/* SPxHistogramX11 class declaration. */
class SPxHistogramX11 : public SPxHistogram
{

public:
    /* Constructor. */
    explicit  SPxHistogramX11(GtkWidget* histogramWidget, SPxRunProcess* proStats = NULL);

    /* Destructor. */
    virtual ~SPxHistogramX11(void);

    /*
     * Public functions.
     */

    /* Histogram drawing. */
    virtual void RequestRedraw(void) { DrawHistogram(); }
    virtual void DrawHistogram(void);

private:

    /*
     * Private functions.
     */
    /* Mouse motion event. */
    static gint OnHistogramDrawingAreaMotionNotifyEvent(GtkWidget *widget, 
                                                        GdkEventMotion *event,  
                                                        gpointer userData);

    /* Mouse leave event. */
    static gboolean OnHistogramDrawingAreaLeaveNotifyEvent(GtkWidget *widget,
                                                        GdkEventCrossing *event,
                                                        gpointer userData);

    /* Draw function. */
    virtual void DrawBackground(void) const;
    virtual void DrawBar(const SPxHistogram::Bar& bar) const;
    virtual void DrawGrid(void) const;
    virtual void DrawAxisCaption(double x, int index) const;
    virtual void DrawMean(double x0, double y0, double x1, double y1) const;
    virtual void DrawSd(double x0, double y0, double x1, double y1) const;
    virtual void DrawNormalCurve(double cx, double cy) const;
    virtual void DrawSamplePercentageCaption(const char *caption) const;
    virtual void DrawCursorLine(void) const;
    virtual void DrawCursorCaption(const char *captionPos,
				   const char *captionVal) const;

    virtual double GetX(void) const;
    virtual double GetY(void) const;
    virtual double GetWidth(void) const;
    virtual double GetHeight(void) const;
    virtual double GetCursorX(void) const;
    virtual double GetCursorY(void) const;

    /* Set cairo color. */
    void SetCairoColor(UINT32 colour) const;

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxHistogramX11 */

#endif /* _SPX_HISTOGRAMX11_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

