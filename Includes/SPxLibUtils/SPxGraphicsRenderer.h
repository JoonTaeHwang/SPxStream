/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxGraphicsRenderer base class which supports the display
*   of radar overlay or underlay graphics in an application, but only 
*   via a derived class.
*
* Revision Control:
*   30/06/22 v1.41   AGC	Support North or platform heading relative EBL.
*				Support show/hide of EBL/VRM text.
*
* Previous Changes:
*   13/05/21 1.40   BTB	Support drawing platform heading line.
*   15/09/20 1.39   AGC	Use SPxRendererXYD_t everywhere.
*   15/09/20 1.38   AGC	Fix N/C #793 - auto range rings with non-zero ref position.
*   09/01/20 1.37   BTB	Improve support for pairs of PI lines.
*   20/12/19 1.36   BTB	Support pairs of PI lines and clearing all lines.
*   11/11/19 1.35   BTB	Support colour configuration on a per EBL/VRM basis.
*   30/07/19 1.34   AGC	Support configuration radar position size.
*			Support end range labels.
*   30/11/18 1.33   AGC	Support configurable radar symbol.
*			Support radial lines.
*   18/10/18 1.32   REW	Support non-radar reference points.
*   17/10/18 1.31   AGC	Support radar fov lines.
*   11/10/18 1.30   REW	Support CmdRings for rings on/auto/off/more/less.
*   16/03/18 1.29   BTB	Create parameters for auto range ring spacing.
*   14/03/18 1.28   BTB	Add compass ring.
*   13/03/18 1.27   AGC	Support editable range rings.
*   12/03/18 1.26   BTB	Use separate member variables for
*			auto range rings.
*   05/03/18 1.25   BTB	Added support to set highlighting of every
*			nth range ring (calculated automatically
*			for auto range rings).
*   05/03/18 1.24   BTB	Add auto range rings.
*   07/02/18 1.23   SP 	Allow ruler items in menu to be disabled.
*   18/12/17 1.22   SP 	Support per-ruler colour.
*   31/01/17 1.21   SP 	Add SetGraticuleCrossEnabled().
*   30/01/17 1.20   REW	Support CPA as part of rulers.
*   14/07/16 1.19   AGC	Support multiple EBL/VRMs.
*   10/10/14 1.18   REW	Support multiple rulers.
*   02/06/14 1.17   AGC	Support radar name.
*   16/05/14 1.16   AGC	Add automatic context menu support.
*   01/04/14 1.15   REW	Add ruler support.
*   05/03/14 1.14   SP 	Add option to make parallel index 
*			lines relative to course (instead of heading).
*   13/12/13 1.13   AGC	Parallel index lines now relative to heading.
*			Parallel index lines always parallel to heading.
*   04/11/13 1.12   SP 	Add GetGraticuleMarginPixels().
*   07/10/13 1.11   AGC	Move rendering to renderAll().
*   05/09/13 1.10   SP 	Change default graticule cross colour.
*   01/08/13 1.9    SP 	Add graticule and end-of-range marker.
*   12/02/13 1.8    AGC	New form of parallel index line.
*   21/12/12 1.7    AGC	Add FindNearestParallelIndexLine().
*   12/12/12 1.6    AGC	Add Parallel Index Line support.
*   23/11/12 1.5    AGC	Add EBL/VRM support.
*			Implement SPxMouseInterface.
*   26/10/12 1.4    AGC	Make Get...Enabled() functions const.
*   30/04/12 1.3    SP 	Add RealToString().
*   08/02/12 1.2    SP 	Support configurable units.
*                       Add radar position marker.
*   26/04/11 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_GRAPHICS_RENDERER_H
#define _SPX_GRAPHICS_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxSymbol.h"
#include "SPxLibUtils/SPxRenderer.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Define the maximum number of rulers. */
#define SPX_GRAPHICS_RENDERER_MAX_RULERS	8


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

enum SPxGraphicsRendererRef_t
{
    SPX_GRAPHICS_RENDERER_REF_NORTH = 0,
    SPX_GRAPHICS_RENDERER_REF_PLATFORM_HEADING = 1
};

/* Forward declarations. */
class SPxNavData;
struct SPxParallelIndexLine_t;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxGraphicsRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxGraphicsRenderer(SPxNavData *navData=NULL);
    virtual ~SPxGraphicsRenderer(void);

    /* Range ring configuration functions. */
    SPxErrorCode SetRangeRingsEnabled(int state);
    int GetRangeRingsEnabled(void) const { return m_rangeRingsEnabled; }
    SPxErrorCode SetRangeRingsEditEnabled(int state);
    int GetRangeRingsEditEnabled(void) const { return m_rangeRingsEditEnabled; }
    SPxErrorCode SetRangeRings(int count, 
                               double startMetres,
                               double spacingMetres);
    SPxErrorCode SetRangeRingsList(const char *rangeList);
    SPxErrorCode GetRangeRingsList(char *buf, unsigned int bufLen) const;
    unsigned int GetRangeRingsCount(void) { return m_rangeRingsCount; }
    double GetRangeRingsStartMetres(void) { return m_rangeRingsStartMetres; }
    double GetRangeRingsSpacingMetres(void) {return m_rangeRingsSpacingMetres;}
    SPxErrorCode SetRangeRingsColour(UINT32 col);
    UINT32 GetRangeRingsColour(void) { return m_rangeRingsColour; }
    SPxErrorCode SetRangeRingsLabelsEnable(int state);
    int GetRangeRingsLabelsEnable(void) { return m_rangeRingsLabelsEnabled; }
    SPxErrorCode SetHighlightNthRangeRing(int state);
    int GetHighlightNthRangeRing(void) const { return m_highlightNthRangeRing; }
    SPxErrorCode SetNthRangeRingColour(UINT32 col);
    UINT32 GetNthRangeRingColour(void) const { return m_nthRangeRingColour; }
    SPxErrorCode HandleCmdRings(const char *cmd);

    /* Auto range ring configuration functions. */
    SPxErrorCode SetRangeRingsAuto(int state);
    int GetRangeRingsAuto(void) const { return m_rangeRingsAutoMode; }
    SPxErrorCode SetAutoRingsDisplayed(UINT32 val);
    UINT32 GetAutoRingsDisplayed(void) const { return m_autoRingsDisplayed; }
    SPxErrorCode SetAutoMinRingSpacing(UINT32 val);
    UINT32 GetAutoMinRingSpacing(void) const { return m_autoMinRingSpacingPixels; }

    /* Compass ring configuration functions. */
    SPxErrorCode SetCompassRingEnabled(int state);
    int GetCompassRingEnabled(void) const { return m_compassRingEnabled; }
    SPxErrorCode SetCompassRingColour(UINT32 col);
    UINT32 GetCompassRingColour(void) { return m_compassRingColour; }

    /* Radar position configuration functions. */
    SPxErrorCode SetRadarPositionEnabled(int state);
    int GetRadarPositionEnabled(void) const { return m_radarPositionEnabled; }
    SPxErrorCode SetRadarPositionColour(UINT32 col);
    UINT32 GetRadarPositionColour(void) const { return m_radarPositionColour; }
    SPxErrorCode SetRadarPositionShape(SPxSymbolShape_t shape);
    SPxSymbolShape_t GetRadarPositionShape(void) const { return m_radarPositionShape; }
    SPxErrorCode SetRadarPositionSize(double size);
    double GetRadarPositionSize(void) const { return m_radarPositionSize; }
    SPxErrorCode SetRadarName(const char *radarName);


    /* Electronic bearing line/variable range marker functions. */
    SPxErrorCode SetEBLEnabled(int state);
    int GetEBLEnabled(void) const;
    int AddEBL(void);
    SPxErrorCode RemoveEBL(int idx);
    SPxErrorCode SelectEBL(int idx);
    SPxErrorCode SetEBLColour(UINT32 col, int idx=0);
    UINT32 GetEBLColour(int idx=0);
    SPxErrorCode SetEBLOffset(double rangeMetres, double bearingDegs, int idx=0);
    SPxErrorCode GetEBLOffset(double *rangeMetres, double *bearingDegs, int idx=0) const;
    SPxErrorCode SetEBLDash(SPxRendererDash_t dash);
    SPxRendererDash_t GetEBLDash(void) const { return m_eblDash; }
    SPxErrorCode SetEBLFixed(int fixed);
    int GetEBLFixed(void) const;
    SPxErrorCode SetEBLReference(SPxGraphicsRendererRef_t ref);
    SPxGraphicsRendererRef_t GetEBLReference(void) const;
    SPxErrorCode SetEBLShowText(int showText);
    int GetEBLShowText(void) const;
    SPxErrorCode SetEBLTargetPos(double rangeMetres, double bearingDegs, int idx = 0);
    SPxErrorCode GetEBLTargetPos(double *rangeMetres, double *bearingDegs, int idx = 0) const;
    int FindNearestEBL(int x, int y,
		       double *distMetresRtn) const;

    SPxErrorCode SetVRMEnabled(int state);
    int GetVRMEnabled(void) const;
    int AddVRM(void);
    SPxErrorCode RemoveVRM(int idx);
    SPxErrorCode SelectVRM(int idx);
    SPxErrorCode SetVRMColour(UINT32 col, int idx=0);
    UINT32 GetVRMColour(int idx=0);
    SPxErrorCode SetVRMDash(SPxRendererDash_t dash);
    SPxRendererDash_t GetVRMDash(void) const { return m_vrmDash; }
    SPxErrorCode SetVRMFixed(int fixed);
    int GetVRMFixed(void) const;
    SPxErrorCode SetVRMShowText(int showText);
    int GetVRMShowText(void) const;
    SPxErrorCode SetVRMTargetPos(double rangeMetres, double bearingDegs, int idx = 0);
    SPxErrorCode GetVRMTargetPos(double *rangeMetres, double *bearingDegs, int idx = 0) const;
    int FindNearestVRM(int x, int y,
		       double *distMetresRtn) const;

    /* Convenience functions to set EBL and VRM together. */
    SPxErrorCode SetEBLVRMFixed(int fixed);
    SPxErrorCode SetTargetPos(double rangeMetres, double bearingDegs, int idx=0);

    /* Parallel index lines functions. */
    SPxErrorCode SetParallelIndexEnabled(int state);
    int GetParallelIndexEnabled(void) const { return m_pilEnabled; }
    SPxErrorCode SetParallelIndexHeadingAligned(int state);
    int GetParallelIndexHeadingAligned(void) const { return m_pilHeadingAligned; }
    SPxErrorCode SetParallelIndexColour(UINT32 col);
    UINT32 GetParallelIndexColour(void) const { return m_pilColour; }
    SPxErrorCode SetParallelIndexDash(SPxRendererDash_t dash);
    SPxRendererDash_t GetParallelIndexDash(void) const { return m_pilDash; }
    SPxParallelIndexLine_t *AddParallelIndexLine(double distMetres, int addMirroredLine = FALSE);
    SPxParallelIndexLine_t *AddParallelIndexLine(SPxRendererXY_t *pos, int addMirroredLine = FALSE);
    SPxErrorCode RemoveParallelIndexLine(SPxParallelIndexLine_t *line, int removeMirroredLine = FALSE);
    SPxParallelIndexLine_t *FindNearestParallelIndexLine(int x, int y,
							 double *distMetresRtn) const;
    void ClearParallelIndexLines(void);

    /* Graticule functions. */
    SPxErrorCode SetGraticuleEnabled(int state);
    int GetGraticuleEnabled(void) const { return m_gratEnabled; }
    SPxErrorCode SetGraticuleCrossEnabled(int state);
    int GetGraticuleCrossEnabled(void) { return m_gratCrossEnabled; }
    SPxErrorCode SetGraticuleRulerColour(UINT32 col);
    UINT32 GetGraticuleRulerColour(void) { return m_gratRulerColour; }
    SPxErrorCode SetGraticuleCrossColour(UINT32 col);
    UINT32 GetGraticuleCrossColour(void) { return m_gratCrossColour; }
    SPxErrorCode SetGraticuleBgColour(UINT32 col);
    UINT32 GetGraticuleBgColour(void) { return m_gratBgColour; }
    int GetGraticuleMarginPixels(void) { return m_gratMarginPixels; }

    /* End of range marker functions. */
    SPxErrorCode SetEndRangeEnabled(int state);
    int GetEndRangeEnabled(void) const { return m_endRangeEnabled; }
    SPxErrorCode SetEndRangeLabelsEnabled(int state);
    int GetEndRangeLabelsEnabled(void) const { return m_endRangeLabelsEnabled; }
    SPxErrorCode SetEndRangeColour(UINT32 col);
    UINT32 GetEndRangeColour(void) { return m_endRangeColour; }
    SPxErrorCode SetEndRangeMetres(double metres);

    /* Ruler functions. */
    SPxErrorCode SetRulerColour(UINT32 col, int idx=-1);
    UINT32 GetRulerColour(int idx=0) const;
    SPxErrorCode SetRulerEnabled(int state, int idx=-1);
    int GetRulerEnabled(int idx=0) const;
    SPxErrorCode SetRulerCPAEnabled(int state, int idx=-1);
    int GetRulerCPAEnabled(int idx=0) const;
    SPxErrorCode SetRulerDefaultCPAState(int state);
    int GetRulerDefaultCPAState(void) const { return(m_rulerDefaultCPAState);};
    SPxErrorCode SetRulerStartPoint(double latDegs, double longDegs,
					double altMetres=0.0,
					int idx=0,
					double speedMps=0.0,	/* For CPA */
					double courseDegs=0.0);
    SPxErrorCode GetRulerStartPoint(double *latDegsPtr, double *longDegsPtr,
					double *altMetresPtr,
					int idx=0,
					double *speedMpsPtr = NULL,
					double *courseDegsPtr = NULL) const;
    SPxErrorCode SetRulerEndPoint(double latDegs, double longDegs,
					double altMetres=0.0,
					int idx=0,
					double speedMps=0.0,	/* For CPA */
					double courseDegs=0.0);
    SPxErrorCode GetRulerEndPoint(double *latDegsPtr, double *longDegsPtr,
					double *altMetresPtr,
					int idx=0,
					double *speedMpsPtr = NULL,
					double *courseDegsPtr = NULL) const;
    void SetRulerMenuEnabled(int isEnabled) { m_rulerMenuEnabled = isEnabled; }

    /* Coverage indicators. */
    SPxErrorCode SetCoverageEnabled(int state);
    int GetCoverageEnabled(void) const;
    SPxErrorCode SetCoverageColour(UINT32 col);
    UINT32 GetCoverageColour(void) const;
    SPxErrorCode SetCoverageBearingDegs(double bearingDegs);
    double GetCoverageBearingDegs(void) const;
    SPxErrorCode SetCoverageFOVDegs(double fovDegs);
    double GetCoverageFOVDegs(void) const;

    /* Radial lines. */
    SPxErrorCode SetRadialLinesEnabled(int state);
    int GetRadialLinesEnabled(void) const;
    SPxErrorCode SetRadialLinesColour(UINT32 col);
    UINT32 GetRadialLinesColour(void) const;
    SPxErrorCode SetNumRadialLines(unsigned int numLines);
    unsigned int GetNumRadialLines(void) const;

    /* Heading line. */
    SPxErrorCode SetHeadingLineEnabled(int state);
    int GetHeadingLineEnabled(void) const;
    SPxErrorCode SetHeadingLineColour(UINT32 col);
    UINT32 GetHeadingLineColour(void) const;

    /* Functions for handling mouse interaction. */
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);

protected:
    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    mutable SPxCriticalSection m_mutex;
    SPxNavData *m_navData;

    /* Position we reference things to when rendering. */
    double m_refXMetres;
    double m_refYMetres;
    double m_refXPixels;
    double m_refYPixels;

    /* Range ring settings */
    int m_rangeRingsEnabled;
    int m_rangeRingsEditEnabled;
    unsigned int m_rangeRingsCount;
    double m_rangeRingsStartMetres;
    double m_rangeRingsSpacingMetres;
    SPxAutoPtr<double[]> m_rangeRingsListMetres;
    unsigned int m_rangeRingsListCount;
    int m_rangeRingsMoveIndex;
    int m_rangeRingsLabelsEnabled;
    UINT32 m_rangeRingsColour;
    int m_highlightNthRangeRing;
    UINT32 m_nthRangeRingColour;
    unsigned int m_firstRangeRingNum;

    /* Auto range ring settings */
    int m_rangeRingsAutoMode;
    unsigned int m_autoRingsCount;
    double m_autoRingsStartMetres;
    double m_autoRingsSpacingMetres;
    int m_autoHighlighNthRing;
    UINT32 m_autoRingsDisplayed;
    UINT32 m_autoMinRingSpacingPixels;

    /* Compass ring settings. */
    int m_compassRingEnabled;
    UINT32 m_compassRingColour;

    /* Radar position settings. */
    int m_radarPositionEnabled;
    UINT32 m_radarPositionColour;
    SPxSymbol m_radarPositionSymbol;
    SPxSymbolShape_t m_radarPositionShape;
    double m_radarPositionSize;
    SPxAutoPtr<char[]> m_radarName;

    /* Electronic bearing line settings. */
    int m_eblEnabled;
    int m_eblFixed;
    SPxGraphicsRendererRef_t m_eblRef;
    SPxRendererDash_t m_eblDash;
    int m_eblShowText;
    int m_vrmEnabled;
    int m_vrmFixed;
    SPxRendererDash_t m_vrmDash;
    int m_vrmShowText;
    int m_mouseInWindow;
    SPxRendererXYD_t m_mousePos;
    int m_selectedEBL;
    int m_selectedVRM;

    /* Parallel index line settings. */
    int m_pilEnabled;
    UINT32 m_pilColour;
    SPxRendererDash_t m_pilDash;
    SPxParallelIndexLine_t *m_pilListHead;
    int m_pilHeadingAligned;

    /* Graticule. */
    int m_gratEnabled;
    int m_gratCrossEnabled;
    UINT32 m_gratRulerColour;
    UINT32 m_gratCrossColour;
    UINT32 m_gratBgColour;
    int m_gratMarginPixels;

    /* End of range marker. */
    int m_endRangeEnabled;
    int m_endRangeLabelsEnabled;
    UINT32 m_endRangeColour;
    double m_endRangeMetres;

    /* Ruler. */
    int m_rulerDefaultCPAState;
    UINT32 m_rulerColour[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    int m_rulerEnabled[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    int m_rulerCPAEnabled[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerStartLatDegs[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerStartLongDegs[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerStartSpeedMps[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerStartCourseDegs[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerStartAltMetres[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerEndLatDegs[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerEndLongDegs[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerEndAltMetres[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerEndSpeedMps[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    double m_rulerEndCourseDegs[SPX_GRAPHICS_RENDERER_MAX_RULERS];
    int m_rulerHideID;		/* Context menu handle */
    int m_rulerMenuEnabled;

    /* Coverage. */
    int m_coverageEnabled;
    UINT32 m_coverageColour;
    double m_coverageBearingDegs;
    double m_coverageFOVDegs;

    /* Radial lines. */
    int m_radialLinesEnabled;
    UINT32 m_radialLinesColour;
    unsigned int m_numRadialLines;

    /* Heading line. */
    int m_headingLineEnabled;
    UINT32 m_headingLineColour;

    /*
     * Private functions.
     */

    virtual SPxErrorCode renderAll(void);
    SPxErrorCode renderRangeRings(void);
    SPxErrorCode renderCompassRing(void);
    SPxErrorCode renderRadarPos(void);
    SPxErrorCode renderEbl(void);
    SPxErrorCode renderVrm(void);
    SPxErrorCode renderPil(void);
    SPxErrorCode renderGraticule(void);
    SPxErrorCode renderEndRange(void);
    SPxErrorCode renderRadialLines(void);
    SPxErrorCode renderHeadingLine(void);
    SPxErrorCode renderRulers(void);
    SPxErrorCode renderCoverage(void);
    void RealToString(char *buf, unsigned int bufLen,
                      double val, int numDp);
    static void contextMenuWrapper(SPxPopupMenu *menu, int id, void *userArg);
    void contextMenu(int id);
    void recalculateRangeRings(void);
    int getClosestRangeRing(int x, int y) const;

}; /* SPxGraphicsRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_OVERLAY_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
