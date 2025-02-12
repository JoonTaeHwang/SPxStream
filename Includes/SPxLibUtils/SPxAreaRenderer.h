/*********************************************************************
*
* (c) Copyright 2012 - 2014, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxAreaRenderer base class which supports the display
*   of active areas in an application, but only via a derived class.
*
* Revision Control:
*   11/07/19 v1.7    AGC	Use more floating point positions.
*
* Previous Changes:
*   06/06/14 1.6    AGC	Support buildings points and labels.
*   08/05/14 1.5    AGC	Support buildings.
*   21/10/13 1.4    SP 	Allow display of disabled areas.
*   07/10/13 1.3    AGC	Use private class.
*			Move rendering to renderAll().
*   20/02/12 1.2    AGC	More explicit lat/long, metres specification.
*   08/02/12 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_AREA_RENDERER_H
#define _SPX_AREA_RENDERER_H

/*
 * Other headers required.
 */

/* We need SPxAutoPtr class. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* We need SPxError for error types. */
#include "SPxLibUtils/SPxError.h"

/* We need SPxRenderer for our base class. */
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

/* Need to forward-declare other classes in case headers are in wrong order.*/
class SPxRenderer;
class SPxBuildingDatabase;
class SPxSimBuilding;
class SPxSimBuildingShape;
class SPxActiveRegion;
class SPxPolyArea;
class SPxSegment;
struct SPxAreaRendererPriv;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxAreaRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAreaRenderer(void);
    virtual ~SPxAreaRenderer(void);

    /* Configuration functions */
    SPxErrorCode SetIncludeColour(UINT32 argb);
    UINT32 GetIncludeColour(void) { return m_includeColour; }
    SPxErrorCode SetExcludeColour(UINT32 argb);
    UINT32 GetExcludeColour(void) { return m_excludeColour; }
    SPxErrorCode SetMiscIncludeColour(UINT32 argb);
    UINT32 GetMiscIncludeColour(void) { return m_miscIncludeColour; }
    SPxErrorCode SetMiscExcludeColour(UINT32 argb);
    UINT32 GetMiscExcludeColour(void) { return m_miscExcludeColour; }
    SPxErrorCode SetPolyInProgressColour(UINT32 argb);
    UINT32 GetPolyInProgressColour(void) { return m_polyInProgressColour; }
    SPxErrorCode SetSegmentColour(UINT32 argb);
    UINT32 GetSegmentColour(void) { return m_segmentColour; }
    SPxErrorCode SetOutlineColour(UINT32 argb);
    UINT32 GetOutlineColour(void) { return m_outlineColour; }
    SPxErrorCode SetOutlineHighlightColour(UINT32 argb);
    UINT32 GetOutlineHighlightColour(void) { return m_outlineHighlightColour; }
    SPxErrorCode SetPointColour(UINT32 argb);
    UINT32 GetPointColour(void) { return m_pointColour; }
    SPxErrorCode SetPointHighlightColour(UINT32 argb);
    UINT32 GetPointHighlightColour(void) { return m_pointHighlightColour; }
    SPxErrorCode SetPointSize(UINT32 size);
    UINT32 GetPointSize(void) { return m_pointSize; }
    SPxErrorCode SetTextColour(UINT32 argb);
    UINT32 GetTextColour(void) { return m_textColour; }
    SPxErrorCode SetLabelFontSize(UINT32 size);
    UINT32 GetLabelFontSize(void) { return m_labelFontSize; }
    SPxErrorCode SetPointLabelFontSize(UINT32 size);
    UINT32 GetPointLabelFontSize(void) { return m_pointLabelFontSize; }
    SPxErrorCode SetShowPoints(int isVisible);
    int GetShowPoints(void) { return m_showPoints; }
    SPxErrorCode SetShowLabels(int isVisible);
    int GetShowLabels(void) { return m_showLabels; }
    SPxErrorCode SetShowPointLabels(int isVisible);
    int GetShowPointLabels(void) { return m_showPointLabels; }
    SPxErrorCode SetShowHighlight(int isVisible);
    int GetShowHighlight(void) { return m_showHighlight; }
    SPxErrorCode SetHighlightPoint(double x, double y);
    SPxErrorCode SetShowDisabled(int isVisible);
    int GetShowDisabled(void) { return m_showDisabled; }
    SPxErrorCode SetDrawOnlySelectedRegion(int isVisible);
    int GetDrawOnlySelectedRegion(void) { return m_drawOnlySelectedRegion; }
    SPxErrorCode SetDrawOnlySelectedPoly(int isVisible);
    int GetDrawOnlySelectedPoly(void) { return m_drawOnlySelectedPoly; }
    SPxErrorCode SetSelectedRegion(SPxActiveRegion *region);
    SPxErrorCode SetSelectedPoly(SPxPolyArea *poly);
    SPxErrorCode ResetPolyInProgess(void);
    SPxErrorCode AddPolyInProgressPoint(double x, double y);
    SPxErrorCode SetBuildingDatabase(SPxBuildingDatabase *bdb);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    SPxAutoPtr<SPxAreaRendererPriv> m_p;

    /* Area colours */
    UINT32 m_includeColour;	/* Tracker parameter include areas ARGB colour. */
    UINT32 m_excludeColour;	/* Tracker parameter exclude areas ARGB colour. */
    UINT32 m_miscIncludeColour; /* Area mask include areas ARGB colour. */
    UINT32 m_miscExcludeColour; /* Area mask include areas ARGB colour. */
    UINT32 m_polyInProgressColour;  /* Polygon in progress colour. */
    UINT32 m_segmentColour;	/* Segment ARGB colour. */
    UINT32 m_outlineColour;	/* Area outline ARGB colour. */
    UINT32 m_outlineHighlightColour;	/* Highlighted area outline ARGB colour. */
    UINT32 m_pointColour;	/* Point ARGB colour. */
    UINT32 m_pointHighlightColour;  /* Highlighted point ARGB colour. */
    UINT32 m_pointSize;		/* Size of points in pixels. */
    UINT32 m_textColour;	/* Text ARGB colour. */
    UINT32 m_labelFontSize;	/* Font size for labels. */
    UINT32 m_pointLabelFontSize; /* Font size for point labels. */
    int m_showPoints;		/* Show area points? */
    int m_showLabels;		/* Show area labels? */
    int m_showPointLabels;	/* Show point labels? */
    int m_showHighlight;	/* Highlight a point? */
    int m_showDisabled;         /* Show disabled poly/segements using muted colours? */
    double m_highlightX;	/* Coordinates of point to highlight. */
    double m_highlightY;
    int m_drawOnlySelectedRegion;	/* Draw only the selected region? */
    SPxActiveRegion *m_selectedRegion;	/* The currently selected active region. */
    int m_drawOnlySelectedPoly;		/* Draw only the selected polygon? */
    SPxPolyArea *m_selectedPoly;	/* The currently selected polygon. */
    
    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(void);
    SPxErrorCode RenderBuilding(SPxSimBuilding *building);
    SPxErrorCode RenderBuildingShape(SPxSimBuilding *building, SPxSimBuildingShape *shape);
    SPxErrorCode RenderRegion(SPxActiveRegion *region);
    SPxErrorCode RenderPolyArea(SPxActiveRegion *region, SPxPolyArea *poly);
    SPxErrorCode RenderPolyAreaPoint(int index, UINT32 pointColour,
	SPxRendererXYD_t *point, SPxRendererXYD_t *lastPoint, 
	int size, int drawCircle, int labelPoint);
    SPxErrorCode RenderSegment(SPxActiveRegion *region, SPxSegment *segment);
    SPxErrorCode RenderHighlightPoint(SPxRendererXYD_t *point);
    UINT32 GetDisabledColour(UINT32 argb, int isEnabled);

    /*
     * Private static functions.
     */

}; /* SPxAreaRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_AREA_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
