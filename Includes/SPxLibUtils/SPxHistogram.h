/*********************************************************************
 *
 * (c) Copyright 2014, 2015, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxHistogram.h,v $
 * ID: $Id: SPxHistogram.h,v 1.4 2015/10/01 11:35:30 rew Exp $
 *
 * Purpose:
 *   Histogram control interface.
 *
 * Revision Control:
 *  01/10/15 v1.4    AGC	Move std::vector usage into source file.
 *
 * Previous Changes:
 *  16/10/14 1.3    JP	Added extra options to set counter data.
 *  14/10/14 1.2    REW	Use ifndef for multiple-inclusion protection.
 *  14/10/14 1.1    JP 	Initial version.
 **********************************************************************/

#ifndef _SPX_HISTOGRAM_H
#define _SPX_HISTOGRAM_H

/* Standard headers. */

/* Library headers. */
#include "SPxResource.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
class SPxRunProcess;

/* SPxHistogram class. */
class SPxHistogram : public SPxObj
{
public:

    /* destruction. */
    virtual ~SPxHistogram(void);

    // Set colors.
    void SetBarsColor(UINT32 color);
    void SetBackgroundColor(UINT32 color);
    void SetCaptionColor(UINT32 color);
    void SetGridColor(UINT32 color);
    void SetMeanColor(UINT32 color);
    void SetSdColor(UINT32 color);
    void SetNormalCurveColor(UINT32 color);
    void SetCursorLineColor(UINT32 color);
    void SetSamplePercentageColor(UINT32 color);
    void SetTickColor(UINT32 color);
    void SetHiddenDataIndicatorColor(UINT32 color);

    // Set histogram data.
    void SetCounter(const unsigned int *data, unsigned int numEl);
    void SetCounter();

    // Build and draw the histogram.
    bool BuildHistogram(int xAxisMin = 0, int xAxisMax = -1,
			bool meanFlag = false,
			bool sdFlag = false,
			bool normalCurveFlag = false);

protected:
    /* Protected types. */
    struct Bar
    {
	double x;	  // Top left x point of the bar.
	double y;	  // Top left y point of the bar.
	double width;     // Width of the bar.
	double height;    // Height of the bar.
    };

    /* Protected functions. */
    SPxHistogram(SPxRunProcess* proStats);
    void Draw(void) const;

    /* Protected variables. */
    UINT32 m_barFillARGB;      /* Control background colour. */
    UINT32 m_backgroundFillARGB;   /* Dialog background brush colour. */
    UINT32 m_captionARGB;   /* Caption colour. */
    UINT32 m_gridARGB;      /* Control background colour. */
    UINT32 m_meanARGB;      /* Mean line colour. */
    UINT32 m_sdARGB;      /* Standard deviation colour. */
    UINT32 m_normalCurveARGB; /* Normal curve colour. */
    UINT32 m_cursorLineARGB; /* Cursor line colour. */
    UINT32 m_samplePercentageARGB; /* Sample percentage colour. */
    UINT32 m_tickARGB;             /* Tick mark color */
    UINT32 m_hiddenDataIndicatorARGB; /* Hidden data indicator color */

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;
    
    /*
     * Private functions.
     */

    // Draw function.
    virtual void RequestRedraw(void) = 0;
    virtual void DrawBackground(void) const = 0;
    virtual void DrawBar(const Bar& bar) const = 0;
    virtual void DrawGrid(void) const = 0;
    virtual void DrawAxisCaption(double x, int index) const = 0;
    virtual void DrawMean(double x0, double y0, double x1, double y1) const = 0;
    virtual void DrawSd(double x0, double y0, double x1, double y1) const = 0;
    virtual void DrawNormalCurve(double cx, double cy) const = 0;
    virtual void DrawSamplePercentageCaption(const char *caption) const = 0;
    virtual void DrawCursorLine(void) const = 0;
    virtual void DrawCursorCaption(const char *captionPos,
				   const char *captionVal) const = 0;

    virtual double GetX(void) const = 0;
    virtual double GetY(void) const = 0;
    virtual double GetWidth(void) const = 0;
    virtual double GetHeight(void) const = 0;
    virtual double GetCursorX(void) const = 0;
    virtual double GetCursorY(void) const = 0;

}; /* Class SPxHistogram. */

#endif /* _SPX_HISTOGRAM_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
