/*********************************************************************
*
* (c) Copyright 2015, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotRenderer base class which supports the display
*	of plots.
*
* Revision Control:
*   30/11/18 v1.2    AGC	Support configurable colours.
*				Support labels/nq-plots/shape boxes.
*				Support use of trails bitmap.
*
* Previous Changes:
*   13/11/15 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_PLOT_RENDERER_H
#define _SPX_PLOT_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxError.h"

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

/* Forward declare other classes required. */
class SPxTargetDB;
struct SPxBlob_tag;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxPlotRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxPlotRenderer(SPxTargetDB *plotDatabase,
		    unsigned int maxTrailLenSecs=3600);
    virtual ~SPxPlotRenderer(void);

    /* Configuration functions. */
    SPxErrorCode SetColour(UINT32 argb);
    UINT32 GetColour(void) const { return m_colour; }
    SPxErrorCode SetColourCross(UINT32 argb);
    UINT32 GetColourCross(void) const { return m_colourCross; }
    SPxErrorCode SetColourMerged(UINT32 argb);
    UINT32 GetColourMerged(void) const { return m_colourMerged; }
    SPxErrorCode SetColourNQ(UINT32 argb);
    UINT32 GetColourNQ(void) const { return m_colourNQ; }
    SPxErrorCode SetColourNQCross(UINT32 argb);
    UINT32 GetColourNQCross(void) const { return m_colourNQCross; }
    SPxErrorCode SetColourShapeBox(UINT32 argb);
    UINT32 GetColourShapeBox(void) const { return m_colourShapeBox; }
    SPxErrorCode SetColourLabel(UINT32 argb);
    UINT32 GetColourLabel(void) const { return m_colourLabel; }

    /* What are we drawing? */
    SPxErrorCode SetDrawLabels(int state);
    int GetDrawLabels(void) const { return(m_drawLabels); }
    SPxErrorCode SetDrawBBoxes(int state);
    int GetDrawBBoxes(void) const { return(m_drawBBoxes); }
    SPxErrorCode SetDrawShapeBoxes(int state);
    int GetDrawShapeBoxes(void) const { return(m_drawShapeBoxes); }
    SPxErrorCode SetDrawNQPlots(int state);
    int GetDrawNQPlots(void) const { return(m_drawNQPlots); }

    /* Trail length. */
    SPxErrorCode SetTrailLengthSecs(unsigned int secs);
    unsigned int GetTrailLengthSecs(void) const { return m_trailLengthSecs; }
    SPxErrorCode SetUseTrailsBitmap(int useTrailsBitmap);
    int IsUsingTrailsBitmap(void) const { return m_useTrailsBitmap; }
    SPxErrorCode ClearHistory(void);

protected:
    /*
     * Protected functions.
     */
    /* Implement the function in the base class. */
    virtual SPxErrorCode renderAll(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, 
                             char *parameterValue);

    virtual int GetParameter(char *parameterName, 
                             char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Database of plots to draw. */
    SPxTargetDB * const m_plotDatabase;	/* Database of SPxBlob_t reports */

    /* Maximum trail length. */
    unsigned int m_maxTrailLenSecs;
    int m_useTrailsBitmap;

    /* Appearance. */
    UINT32 m_colour;			/* Colour to use. */
    UINT32 m_colourCross;		/* Colour to use for cross. */
    UINT32 m_colourMerged;		/* Colour to use for merged plots. */
    UINT32 m_colourNQ;			/* Colour for non-qualifying plots. */
    UINT32 m_colourNQCross;		/* Colour for non-qualifying plot crosses. */
    UINT32 m_colourShapeBox;		/* Colour for shape box. */
    UINT32 m_colourLabel;		/* Colour for labels. */

    /* Options. */
    int m_drawLabels;			/* Draw labels. */
    int m_drawBBoxes;			/* Draw bounding boxes? */
    int m_drawShapeBoxes;		/* Draw shape boxes? */
    int m_drawNQPlots;			/* Draw non-qualifying plots? */

    /* Trail length. */
    unsigned int m_trailLengthSecs;

    /*
     * Private functions.
     */
    SPxErrorCode clearOldTrailPoints(void);
    SPxErrorCode updateOffscreenBitmaps(void);
    SPxErrorCode renderSymbolTrails(int useTrailsBitmap);
    SPxErrorCode renderPlot(SPxBlob_tag *plot, int saveTrail);

}; /* SPxPlotRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_PLOT_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
