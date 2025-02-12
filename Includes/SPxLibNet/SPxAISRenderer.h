/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAISRenderer abstract class which supports the display
*	of AIS messages in an application, but only via a derived class.
*
*
* Revision Control:
*   08/07/22 v1.20   AJH	Support clipping of tracks outside window.
*
* Previous Changes:
*   14/03/22 1.19   AGC	Support simple symbols for large views.
*   08/12/21 1.18   AJH	Support ShowAllTracks/ShowActivatedTracks.
*   07/09/21 1.17   REW	Support SetShowOwnShip().
*   18/09/20 1.16   AGC	Support showing excluded tracks.
*   09/10/19 1.15   BTB	Support track filtering on speed and size.
*   15/01/19 1.14   BTB	Support configuring label colour (separate to symbol colour).
*   15/01/19 1.13   REW	Support display of extrapolated positions.
*   20/11/17 1.12   AGC	Support flashing of targets.
*   13/07/17 1.11   SP 	Add GetDecoder().
*   07/10/13 1.10   AGC	Move rendering to renderAll().
*   04/10/13 1.9    AGC	Simplify headers.
*   13/02/13 1.8    AGC	Support AIS track sector filtering.
*   21/12/12 1.7    AGC	Support an info panel for a selected track.
*			Add convenience functions for track manipulation.
*			Support track filtering on range.
*   05/07/11 1.6    REW	Support history trails.
*   29/06/11 1.5    REW	Support multiple colours.
*   27/06/11 1.4    REW	Use standardised symbols etc.
*   26/04/11 1.3    SP 	Use base class view control.
*   06/04/11 1.2    SP 	Inherit from SPxRenderer base class.
*			Use SPxRendererXY_t not SPxAISRendererXY_t.
*   02/03/11 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_AIS_RENDERER_H
#define _SPX_AIS_RENDERER_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxRenderer.h"

/* We need the SPxAISDecoder class. */
#include "SPxLibNet/SPxAISDecoder.h"


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

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxAISDecoder;
class SPxAISTrack;
class SPxRenderer;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxAISRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAISRenderer(SPxAISDecoder *decoder);
    virtual ~SPxAISRenderer(void);

    /* Get decoder. */
    SPxAISDecoder *GetDecoder(void) { return m_decoder; }

    /* Track rendering. */
    SPxErrorCode RenderTracks(void);

    /* Track filtering. */
    void SetMinRangeMetres(double minRangeMetres) {m_minRangeMetres = minRangeMetres; }
    double GetMinRangeMetres(void)		  {return(m_minRangeMetres);}
    void SetMaxRangeMetres(double maxRangeMetres) {m_maxRangeMetres = maxRangeMetres; }
    double GetMaxRangeMetres(void)		  {return(m_maxRangeMetres);}
    void SetMinAziDegs(double minAziDegs);
    double GetMinAziDegs(void)			  {return(m_minAziDegs);}
    void SetMaxAziDegs(double maxAziDegs);
    double GetMaxAziDegs(void)			  {return(m_maxAziDegs);}
    void SetMinLengthMetres(unsigned int minLengthMetres) {m_minLengthMetres = minLengthMetres;}
    unsigned int GetMinLengthMetres(void)	  {return(m_minLengthMetres);}
    void SetMaxLengthMetres(unsigned int maxLengthMetres) {m_maxLengthMetres = maxLengthMetres;}
    unsigned int GetMaxLengthMetres(void)	  {return(m_maxLengthMetres);}
    void SetMinWidthMetres(unsigned int minWidthMetres)   {m_minWidthMetres = minWidthMetres;}
    unsigned int GetMinWidthMetres(void)	  {return(m_minWidthMetres);}
    void SetMaxWidthMetres(unsigned int maxWidthMetres)   {m_maxWidthMetres = maxWidthMetres;}
    unsigned int GetMaxWidthMetres(void)	  {return(m_maxWidthMetres);}
    void SetMinSpeedMps(double minSpeedMps)	  {m_minSpeedMps = minSpeedMps;}
    double GetMinSpeedMps(void)			  {return(m_minSpeedMps);}
    void SetMaxSpeedMps(double maxSpeedMps)	  {m_maxSpeedMps = maxSpeedMps;}
    double GetMaxSpeedMps(void)			  {return(m_maxSpeedMps);}

    /* Convenience functions. */
    SPxAISTrack *FindNearestTrack(int x, int y,
                                  double *distMetresRtn=NULL);
    SPxErrorCode SelectTrack(SPxAISTrack *track);
    SPxAISTrack *GetSelectedTrack(void);
    SPxErrorCode ActivateAllTracks(int active);
    SPxErrorCode ClearHistory(void);

    /* Symbol control. */
    void SetSymbolLength(UINT32 pixels)		{ m_symbolLength = pixels; }
    UINT32 GetSymbolLength(void)		{ return(m_symbolLength); }

    /* Colour control. */
    void SetSymbolColour(UINT32 argb)		{ m_symbolColour = argb; }
    UINT32 GetSymbolColour(void)		{ return(m_symbolColour); }
    void SetSymbolColourFill(UINT32 argb)	{ m_symbolColourFill = argb; }
    UINT32 GetSymbolColourFill(void)		{ return(m_symbolColourFill); }
    void SetLabelColour(UINT32 argb)		{ m_labelColour = argb; }
    UINT32 GetLabelColour(void)			{ return(m_labelColour); }

    /* Multi-colour control, for passengers, cargos, tankers, HSC and tugs. */
    void SetUseMultiColour(int use)		{ m_useMultiColour = use; }
    int GetUseMultiColour(void)			{ return(m_useMultiColour); }
    void SetSymbolColourPass(UINT32 argb) { m_symbolColourPass = argb; }
    UINT32 GetSymbolColourPass(void) { return(m_symbolColourPass); }
    void SetSymbolColourPassFill(UINT32 argb) { m_symbolColourPassFill = argb;}
    UINT32 GetSymbolColourPassFill(void) { return(m_symbolColourPassFill); }
    void SetSymbolColourCargo(UINT32 argb) { m_symbolColourCargo = argb; }
    UINT32 GetSymbolColourCargo(void) { return(m_symbolColourCargo); }
    void SetSymbolColourCargoFill(UINT32 argb) {m_symbolColourCargoFill=argb;}
    UINT32 GetSymbolColourCargoFill(void) { return(m_symbolColourCargoFill); }
    void SetSymbolColourTanker(UINT32 argb) { m_symbolColourTanker = argb; }
    UINT32 GetSymbolColourTanker(void) { return(m_symbolColourTanker); }
    void SetSymbolColourTankerFill(UINT32 argb) {m_symbolColourTankerFill=argb;}
    UINT32 GetSymbolColourTankerFill(void) { return(m_symbolColourTankerFill); }
    void SetSymbolColourHSC(UINT32 argb) { m_symbolColourHSC = argb; }
    UINT32 GetSymbolColourHSC(void) { return(m_symbolColourHSC); }
    void SetSymbolColourHSCFill(UINT32 argb) { m_symbolColourHSCFill = argb;}
    UINT32 GetSymbolColourHSCFill(void) { return(m_symbolColourHSCFill); }
    void SetSymbolColourTugs(UINT32 argb) { m_symbolColourTugs = argb; }
    UINT32 GetSymbolColourTugs(void) { return(m_symbolColourTugs); }
    void SetSymbolColourTugsFill(UINT32 argb) { m_symbolColourTugsFill = argb;}
    UINT32 GetSymbolColourTugsFill(void) { return(m_symbolColourTugsFill); }

    /* Vector control. */
    void SetVectorSeconds(UINT32 secs)		{ m_vectorSeconds = secs; }
    UINT32 GetVectorSeconds(void) const		{ return(m_vectorSeconds); }

    /* Track display control. */
    void SetShowAllTracks(int show) { m_showAllTracks = show; }
    int GetShowAllTracks(void) const { return(m_showAllTracks); }
    void SetShowActivatedTracks(int show) { m_showActivatedTracks = show; }
    int GetShowActivatedTracks(void) const { return(m_showActivatedTracks); }

    /* Simple symbol control. */
    void SetSimpleViewWidthMetres(double simpleViewWidthMetres)
        { m_simpleViewWidthMetres = simpleViewWidthMetres; }
    double GetSimpleViewWidthMetres(void) const {return(m_simpleViewWidthMetres);}

    /* Remove tracks outside window. */
    SPxErrorCode SetClipTracksToWindow(int clipTracksToWindow) { m_clipTracksToWindow = clipTracksToWindow; return SPX_NO_ERROR; }
    int GetClipTracksToWindow(void) const { return(m_clipTracksToWindow); }

    /* Label control. */
    void SetShowAllLabels(int show)		{m_showAllLabels = show;}
    int GetShowAllLabels(void) const		{return(m_showAllLabels);}
    void SetShowActivatedLabels(int show)	{m_showActivatedLabels = show;}
    int GetShowActivatedLabels(void) const	{return(m_showActivatedLabels);}
    void SetShowSelectedLabels(int show)	{m_showSelectedLabels = show;}
    int GetShowSelectedLabels(void) const	{return(m_showSelectedLabels);}

    /* Extrapolated positions. */
    void SetShowExtrapolations(int show)	{m_showExtrapolations = show;}
    int GetShowExtrapolations(void) const	{return(m_showExtrapolations);}

    /* History trail control. */
    void SetHistorySeconds(UINT32 secs)		{ m_historySeconds = secs; }
    UINT32 GetHistorySeconds(void) const	{ return(m_historySeconds); }
    void SetShowAllTrails(int show)		{m_showAllTrails = show;}
    int GetShowAllTrails(void) const		{return(m_showAllTrails);}
    void SetShowActivatedTrails(int show)	{m_showActivatedTrails = show;}
    int GetShowActivatedTrails(void) const	{return(m_showActivatedTrails);}
    void SetShowSelectedTrails(int show)	{m_showSelectedTrails = show;}
    int GetShowSelectedTrails(void) const	{return(m_showSelectedTrails);}

    /* Info panel control. */
    void SetShowInfoPanel(int show)		{m_showInfoPanel = show;}
    int GetShowInfoPanel(void) const		{return(m_showInfoPanel);}
    void SetInfoPanelPos(int x, int y)		{m_infoPanelX = x;m_infoPanelY = y;}
    void GetInfoPanelPos(int *x, int *y);
    void SetShowCpa(int show)			{m_showCpa = show;}
    int GetShowCpa(void) const			{return(m_showCpa);}

    /* Flashing tracks. */
    SPxErrorCode SetHighlightFlashHz(unsigned int hz);	/* 0 for no flash */
    unsigned int GetHighlightFlashHz(void) const { return(m_highlightFlashHz); }

    /* Excluded tracks. */
    SPxErrorCode SetShowExcludedTracks(int state);
    int GetShowExcludedTracks(void) const	{return m_showExcludedTracks;}

    /* Own ship. */
    SPxErrorCode SetShowOwnShip(int state);
    int GetShowOwnShip(void) const		{return(m_showOwnShip);}

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
    /* Handle of associated decoder object. */
    SPxAISDecoder *m_decoder;		/* Decoder */

    /* Filtering options. */
    double m_minRangeMetres;		/* Minimum range for targets to be displayed */
    double m_maxRangeMetres;		/* Maximum range for targets to be displayed */
    double m_minAziDegs;		/* Minimum azimuth for targets to be displayed */
    double m_maxAziDegs;		/* Maximum azimuth for targets to be displayed */
    unsigned int m_minLengthMetres;	/* Minimum length for targets to be displayed */
    unsigned int m_maxLengthMetres;	/* Maxmimum length for targets to be displayed */
    unsigned int m_minWidthMetres;	/* Minimum width for targets to be displayed */
    unsigned int m_maxWidthMetres;	/* Maxmimum width for targets to be displayed */
    double m_minSpeedMps;		/* Minimum speed for targets to be displayed */
    double m_maxSpeedMps;		/* Minimum speed for targets to be displayed */

    /* Rendering options. */
    UINT32 m_symbolLength;		/* Pixels */
    UINT32 m_vectorSeconds;		/* Length of vector */
    UINT32 m_historySeconds;		/* Length of trail history */
    int m_showAllTracks;		/* Show all tracks */
    int m_showActivatedTracks;		/* Show activated tracks */
    int m_showAllLabels;		/* Show all vessel names */
    int m_showActivatedLabels;		/* Show activated vessel names */
    int m_showSelectedLabels;		/* Show selected vessel names */
    int m_showExtrapolations;		/* Show extrapolated positions */
    int m_showAllTrails;		/* Show all history trails */
    int m_showActivatedTrails;		/* Show activated history trails */
    int m_showSelectedTrails;		/* Show selected history trails */
    int m_showInfoPanel;		/* Show info panel for selected track */
    int m_showOwnShip;			/* Show own ship tracks or not */
    double m_simpleViewWidthMetres;	/* View width threshold for simplified symbols */

    /* Rendering colours */
    UINT32 m_symbolColour;		/* Colour, 0xAARRGGBB */
    UINT32 m_symbolColourFill;		/* 0 for outline only */
    int m_useMultiColour;		/* Flag */
    UINT32 m_symbolColourPass;		/* Colour for passenger craft */
    UINT32 m_symbolColourPassFill;	/* Fill colour, or 0 for outline */
    UINT32 m_symbolColourCargo;		/* Colour for cargo ships */
    UINT32 m_symbolColourCargoFill;	/* Fill colour, or 0 for outline */
    UINT32 m_symbolColourTanker;	/* Colour for tankers */
    UINT32 m_symbolColourTankerFill;	/* Fill colour, or 0 for outline */
    UINT32 m_symbolColourHSC;		/* Colour for high speed craft */
    UINT32 m_symbolColourHSCFill;	/* Fill colour, or 0 for outline */
    UINT32 m_symbolColourTugs;		/* Colour for tugs/pilots etc */
    UINT32 m_symbolColourTugsFill;	/* Fill colour, or 0 for outline */
    UINT32 m_labelColour;		/* Colour for vessel names */
    UINT32 m_panelColourBg;		/* Background colour for info panel */
    UINT32 m_panelColourBorder;		/* Border colour for info panel */
    UINT32 m_panelColourText;		/* Text colour for info panel */

    /* Info panel. */
    int m_infoPanelDrawn;		/* Has an info panel been drawn? */
    int m_infoPanelX;			/* X Position for info panel */
    int m_infoPanelY;			/* Y Position for info panel */
    int m_showCpa;			/* Display Closest Point of Approach? */

    /* Target flash support. */
    int m_highlightFlashHz;		/* Zero for no flash */

    /* Excluded targets. */
    int m_showExcludedTracks;

    /* Remove tracks outside window. */
    int m_clipTracksToWindow;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(void);
    SPxErrorCode renderTrack(SPxAISTrack *track);
    static int staticRenderTrack(void *decoder, void *arg, void *track);

}; /* SPxAISRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_AIS_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
