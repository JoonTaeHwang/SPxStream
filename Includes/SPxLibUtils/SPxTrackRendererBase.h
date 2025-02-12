/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxTrackRendererBase base class which supports the display
*   of radar tracks in an application, either from a local tracker,
*   or from a track database receiving remote track reports, using
*   SPxTrackRendererMHT and SPxTrackRenderer respectively.
*
* Revision Control:
*   30/03/22 v1.23   REW	Support configurable colour for flight plan tracks.
*
* Previous Changes:
*   15/06/21 1.22   SP 	Support configurable track flash colours.
*   20/05/21 1.21   AGC	Update ASTERIX colours support for Mode 5.
*   26/11/20 1.20   SP 	Fix various issues in previous changes.
*   25/11/20 1.19   SP 	Add trail point utility functions.
*			Preserve colour of trail points when using ASTERIX colours.
*   12/11/20 1.18   SP 	Move functionality here from SPxTrackRenderer.
*			Add GetSymbol().
*   27/08/20 1.17   AGC	Add IncludeLabelOffsetsInPositions().
*   26/08/20 1.16   AGC	Support IJMS/MIL2525D render configs.
*   20/06/19 1.15   DGJ	Display of video track updates
*   06/12/18 1.14   REW	Support fading of trail points.
*   20/04/18 1.13   REW	Support strobe rendering too.
*   13/04/18 1.12   REW	Support "plot" rendering too.
*   24/11/17 1.11   AGC	Support max label rows.
*   12/04/17 1.10   AGC	Support auto label positioning.
*   21/01/16 1.9    REW	Support SetTrailLengthSecsSelected() etc.
*   02/10/14 1.8    REW	Add label movement support.
*   18/06/14 1.7    REW	Add control over track interception selection.
*   10/04/14 1.6    AGC	Add double-click track selection support.
*   06/01/14 1.5    REW	Add track interception support.
*   14/08/13 1.4    AGC	Add symbol rotation option.
*   10/04/13 1.3    AGC	Add GetRenderConfig().
*   20/03/13 1.2    SP 	Make m_trackSymbol protected.
*   21/12/12 1.1    AGC	Initial version.
**********************************************************************/

#ifndef SPX_TRACK_RENDERER_BASE_H
#define SPX_TRACK_RENDERER_BASE_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, callbacks, errors etc. */

/* We need our base class. */
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxSymbol.h"
#include "SPxLibNet/SPxRadarTrack.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Track rendering configurations. */
enum SPxTrackRenderConfig
{
    SPX_TRACK_RENDER_CONFIG_SPX = 0,
    SPX_TRACK_RENDER_CONFIG_ECDIS = 1,
    SPX_TRACK_RENDER_CONFIG_IJMS = 2,
    SPX_TRACK_RENDER_CONFIG_MIL2525D = 3
};

/* Track symbol rendering modes. These values map to
 * the rendering modes supported by the SPxSymbol class.
 */
#define TRACK_MODE_ESTABLISHED_UPDATING      0
#define TRACK_MODE_ESTABLISHED_COASTING      1
#define TRACK_MODE_PROVISIONAL_UPDATING      2
#define TRACK_MODE_PROVISIONAL_COASTING      3
#define TRACK_MODE_LOST			     4
#define TRACK_MODE_ESTABLISHED_UPDATING_NPR  5
#define TRACK_MODE_ESTABLISHED_COASTING_NPR  6
#define TRACK_MODE_PROVISIONAL_UPDATING_NPR  7
#define TRACK_MODE_PROVISIONAL_COASTING_NPR  8
#define TRACK_MODE_VIDEO_UPDATE		     9

/* Special case used to indicate a mode we are not interested in */
#define TRACK_MODE_NO_RENDER (SPX_NUM_SYMBOL_MODES)

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
class SPxRadarTrack;

/* Need to forward-declare other structures we need.*/
struct _SPxRadarTrackTrailPoint;

typedef SPxErrorCode (*SPxTrackRendererPreRenderFn_t)(SPxRadarTrack *track,
							void *userArg,
							SPxSymbol *symbol,
							UINT32 *colourPtr);

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxTrackRendererBase : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */
    enum TrackColours
    {
	TRACK_COLOURS_NORMAL = 0,
	TRACK_COLOURS_THREAT = 1,
	TRACK_COLOURS_ASTERIX = 2
    };

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxTrackRendererBase(SPxTrackRenderConfig renderConfig = SPX_TRACK_RENDER_CONFIG_SPX);
    virtual ~SPxTrackRendererBase(void);

    SPxErrorCode SetRenderConfig(SPxTrackRenderConfig renderConfig);
    SPxTrackRenderConfig GetRenderConfig(void) const { return m_renderConfig; }

    void ShowOnlySelected(int enable);
    int IsShowingOnlySelected(void) const { return m_showOnlySelected; }
    void SetSelectionEnabled(int state) { m_selectionEnabled = state; }
    int GetSelectionEnabled(void) const { return m_selectionEnabled; }
    void SetSelectWithDoubleClick(int state) { m_selectWithDoubleClick = state; }
    int GetSelectWithDoubleClick(void) const { return m_selectWithDoubleClick; }
    void SetInterceptEnabled(int state) { m_interceptEnabled = state; }
    int GetInterceptEnabled(void) const { return m_interceptEnabled; }
    void SetInterceptSelectionEnabled(int state)
				    { m_interceptSelectionEnabled = state; }
    int GetInterceptSelectionEnabled(void) const
				    { return m_interceptSelectionEnabled; }
    void SetLabelMoveEnabled(int state) { m_labelMoveEnabled = state; }
    int GetLabelMoveEnabled(void) const { return m_labelMoveEnabled; }

    /* Configuration functions */
    SPxSymbol *GetTrackSymbol(void) { return m_trackSymbol; }
    SPxSymbol *GetPlotSymbol(void) { return m_plotSymbol; }
    SPxErrorCode SetTrailLengthSecs(unsigned int secs);
    unsigned int GetTrailLengthSecs(void) const
					{ return m_trailLengthSecs; }
    SPxErrorCode SetTrailLengthSecsSelected(unsigned int secs);
    unsigned int GetTrailLengthSecsSelected(void) const
					{ return m_trailLengthSecsSelected; }
    SPxErrorCode SetFadeTrailPoints(int enable);
    int GetFadeTrailPoints(void) const { return m_fadeTrailPoints; }
    SPxErrorCode SetVectorLengthSecs(unsigned int secs);
    unsigned int GetVectorLengthSecs(void) const
					{ return m_vectorLengthSecs; }
    SPxErrorCode SetStrobeFadeSecs(unsigned int secs);
    unsigned int GetStrobeFadeSecs(void) const { return m_strobeFadeSecs; }
    SPxErrorCode SetUseRelativeVelocity(int useRelVelocity);
    int GetUseRelativeVelocity(void) const
					{ return m_useRelativeVelocity; }
    SPxErrorCode SetShowTracks(int isVisible);
    SPxErrorCode SetShowPlots(int isVisible);
    SPxErrorCode SetShowStrobes(int isVisible);
    SPxErrorCode SetShowLabels(int isVisible);
    int GetShowTracks(void) const	{ return m_showTracks; }
    int GetShowPlots(void) const	{ return m_showPlots; }
    int GetShowStrobes(void) const	{ return m_showStrobes; }
    int GetShowLabels(void) const	{ return m_showLabels; }
    SPxErrorCode SetLabelAutoOffset(int enable);
    int IsLabelAutoOffsetEnabled(void) const { return m_labelAutoOffset; }
    SPxErrorCode SetLabelOffsetPixels(int x, int y);
    int GetLabelXOffsetPixels(void) const { return m_labelXOffsetPixels; }
    int GetLabelYOffsetPixels(void) const { return m_labelYOffsetPixels; }
    SPxErrorCode SetLabelMaxRows(unsigned int maxRows);
    unsigned int GetLabelMaxRows(void) const { return m_labelMaxRows; }
    SPxErrorCode SetLabel(const char *txt);
    const char *GetLabel(void) { return m_label; }
    SPxErrorCode SetPlotLabel(const char *txt);
    const char *GetPlotLabel(void) { return m_plotLabel; }
    SPxErrorCode SetLabelColour(UINT32 argb);
    UINT32 GetLabelColour(void) const	{ return m_labelColour; }
    SPxErrorCode SetUseFilteredPos(int state);
    int GetUseFilteredPos(void) const	{ return m_useFilteredPos; }
    SPxErrorCode SetRotateSymbols(int state)
    {
	m_rotateSymbols = state;
	return SPX_NO_ERROR;
    }
    int GetRotateSymbols(void) const { return m_rotateSymbols; }
    virtual int IncludeLabelOffsetsInPositions(void) const;

    /* Colour of strobes. */
    SPxErrorCode SetStrobeColour(UINT32 argb);
    UINT32 GetStrobeColour(void) const;

    /* Fusion symbols. */
    SPxErrorCode SetUseFusionSymbols(int state);
    int GetUseFusionSymbols(void) const { return m_useFusionSymbols; }

    /* Excluded tracks. */
    SPxErrorCode SetShowExcludedTracks(int state);
    int GetShowExcludedTracks(void) const { return m_showExcludedTracks; }

    SPxErrorCode SetTrackColours(TrackColours trackColours);
    TrackColours GetTrackColours(void) const { return (m_trackColours); }

    /* Threat colours. */
    SPxErrorCode SetUseThreatColours(int state);
    int GetUseThreatColours(void) const { return(m_trackColours == TRACK_COLOURS_THREAT); }
    UINT32 GetThreatColour(void) const { return m_threatColour; }
    SPxErrorCode SetThreatColourUnclassified(UINT32 argb);
    SPxErrorCode SetThreatColourFriendly(UINT32 argb);
    SPxErrorCode SetThreatColourNeutral(UINT32 argb);
    SPxErrorCode SetThreatColourHostile(UINT32 argb);
    SPxErrorCode SetThreatColourCivilian(UINT32 argb);
    UINT32 GetThreatColourUnclassified(void) const;
    UINT32 GetThreatColourFriendly(void) const;
    UINT32 GetThreatColourNeutral(void) const;
    UINT32 GetThreatColourHostile(void) const;
    UINT32 GetThreatColourCivilian(void) const;
    SPxErrorCode SetThreatLostAlpha(UINT8 alpha);
    UINT8 GetThreatLostAlpha(void) const;

    /* Detection colours. */
    SPxErrorCode SetAsterixColourCoast(UINT32 argb);
    SPxErrorCode SetAsterixColourPsr(UINT32 argb);
    SPxErrorCode SetAsterixColourSsr(UINT32 argb);
    SPxErrorCode SetAsterixColourSsrComb(UINT32 argb);
    SPxErrorCode SetAsterixColourAllCall(UINT32 argb);
    SPxErrorCode SetAsterixColourRollCall(UINT32 argb);
    SPxErrorCode SetAsterixColourAllCallComb(UINT32 argb);
    SPxErrorCode SetAsterixColourRollCallComb(UINT32 argb);
    SPxErrorCode SetAsterixColourGhost(UINT32 argb);
    SPxErrorCode SetAsterixColourTest(UINT32 argb);
    SPxErrorCode SetAsterixColourSCF(UINT32 argb);
    UINT32 GetAsterixColourCoast(void) const;
    UINT32 GetAsterixColourPsr(void) const;
    UINT32 GetAsterixColourSsr(void) const;
    UINT32 GetAsterixColourSsrComb(void) const;
    UINT32 GetAsterixColourAllCall(void) const;
    UINT32 GetAsterixColourRollCall(void) const;
    UINT32 GetAsterixColourAllCallComb(void) const;
    UINT32 GetAsterixColourRollCallComb(void) const;
    UINT32 GetAsterixColourGhost(void) const;
    UINT32 GetAsterixColourTest(void) const;
    UINT32 GetAsterixColourSCF(void) const;

    /* Colour of selected target (if non-zero). */
    SPxErrorCode SetSelectedColour(UINT32 argb);
    UINT32 GetSelectedColour(void) const;

    /* Colour of targets with flight plans (if non-zero). */
    SPxErrorCode SetFlightPlanColour(UINT32 argb);
    UINT32 GetFlightPlanColour(void) const;

    /* Flashing of various targets. */
    SPxErrorCode SetEmergencyFlashHz(unsigned int hz);	/* 0 for no flash */
    unsigned int GetEmergencyFlashHz(void) const { return(m_emergencyFlashHz); }
    SPxErrorCode SetEmergencyFlashColour(UINT32 argb);	/* 0 for complementary colour */
    UINT32 GetEmergencyFlashColour(void) const { return(m_emergencyFlashColour); }
    SPxErrorCode SetSPIFlashHz(unsigned int hz);	/* 0 for no flash */
    unsigned int GetSPIFlashHz(void) const { return(m_spiFlashHz); }
    SPxErrorCode SetSPIFlashColour(UINT32 argb);	/* 0 for complementary colour */
    UINT32 GetSPIFlashColour(void) const { return(m_spiFlashColour); }
    SPxErrorCode SetHighlightFlashHz(unsigned int hz);	/* 0 for no flash */
    unsigned int GetHighlightFlashHz(void) const { return(m_highlightFlashHz); }
    SPxErrorCode SetHighlightFlashColour(UINT32 argb);	/* 0 for complementary colour */
    UINT32 GetHighlightFlashColour(void) const { return(m_highlightFlashColour); }

    /* Alpha of coasted fusion targets. */
    SPxErrorCode SetFuseCoastAlpha(UINT8 alpha);
    UINT8 GetFuseCoastAlpha(void) const;

    /* Altitude mode. */
    SPxErrorCode SetAltitudeMode(SPxHeightMode_t mode);
    SPxHeightMode_t GetAltitudeMode(void) const { return(m_altitudeMode); }

    /* Pre-render callback. */
    SPxErrorCode SetPreRenderCallback(SPxTrackRendererPreRenderFn_t fn,
					void *userArg);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */
    int GetShowCoastedVector(void) const { return m_showCoastedVector; }
    SPxRendererDash_t GetVectorDash(void) const { return m_vectorDash; }
    unsigned int GetTrackMode(UINT8 status, UINT8 numCoasts, int principal=TRUE, int isVideo=FALSE);
    SPxErrorCode RenderVector(unsigned int mode, 
                              double xMetres, double yMetres, 
                              double courseDegrees, double speedMps, 
                              UINT32 colour);
    SPxSymbol *GetSymbol(SPxRadarTrack *track, 
			 UINT32 *labelColRtn,
			 int *showAdditionsRtn,
			 int *showTrailsRtn);
    UINT32 GetTrailPointColour(const _SPxRadarTrackTrailPoint *trailPt);
    UINT32 FadeTrailPoint(UINT32 col, double ageFraction);
    UINT32 GetSecondaryTargetTypeColour(SPxRadarTrackSecTargetType_t stt);
    SPxErrorCode InvokePreRender(SPxRadarTrack *track, 
				 SPxSymbol *m_symbol, 
				 UINT32 *symbolCol);

    /*
     * Protected variables.
     */

    /* Track/plot symbols */
    SPxSymbol *m_trackSymbol;
    SPxSymbol *m_plotSymbol;
    SPxSymbol m_symbol;

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* Track render configuration. */
    SPxTrackRenderConfig m_renderConfig;

    /* Show tracks etc.? */
    int m_showTracks;
    int m_showPlots;
    int m_showStrobes;

    /* Show only selected tracks? */
    int m_showOnlySelected;

    /* Is track selection using the mouse enabled? */
    int m_selectionEnabled;

    /* Is track selection using double clicks instead of single clicks? */
    int m_selectWithDoubleClick;

    /* Is track interception enabled (also needs track selection). */
    int m_interceptEnabled;

    /* Is selection of interception track using the mouse enabled? */
    int m_interceptSelectionEnabled;

    /* Is label movement enabled? */
    int m_labelMoveEnabled;

    /* Use measured or filtered track position? */
    int m_useFilteredPos;

    /* Rotate symbols. */
    int m_rotateSymbols;

    /* Track trail length in seconds */
    unsigned int m_trailLengthSecs;

    /* Selected track trail length in seconds, if > m_trailLengthSecs. */
    unsigned int m_trailLengthSecsSelected;

    /* Do we fade trail points? */
    int m_fadeTrailPoints;

    /* Track vector line length in seconds */
    unsigned int m_vectorLengthSecs;

    /* Strobe display duration in seconds */
    unsigned int m_strobeFadeSecs;

    /* Colour for new strobes. */
    UINT32 m_strobeColour;

    /* Dash style for vector line */
    SPxRendererDash_t m_vectorDash;

    /* Use relative instead of true velocity for vector line. */
    int m_useRelativeVelocity;

    /* Show vectors for coasted tracks? */
    int m_showCoastedVector;

    /* Track labels */
    int m_labelAutoOffset;    /* Auto offset labels to avoid overlap? */
    int m_labelXOffsetPixels; /* X offset from track symbol in pixels */
    int m_labelYOffsetPixels; /* Y offset from track symbol in pixels */
    UINT32 m_labelColour;     /* Label ARGB colour */
    int m_showLabels;         /* Is label visible? */
    unsigned int m_labelMaxRows; /* Max label rows to show */
    const char *m_label;      /* Label string to render for tracks */
    const char *m_plotLabel;  /* Label string to render for plots */

    /* Source of colours. */
    TrackColours m_trackColours;

    /* Use threat colours? */
    UINT32 m_threatColour;
    UINT32 m_threatColourUnclassified;
    UINT32 m_threatColourFriendly;
    UINT32 m_threatColourNeutral;
    UINT32 m_threatColourHostile;
    UINT32 m_threatColourCivilian;

    /* Use Asterix colours? */
    UINT32 m_asterixColourCoast;
    UINT32 m_asterixColourPsr;
    UINT32 m_asterixColourSsr;
    UINT32 m_asterixColourSsrComb;
    UINT32 m_asterixColourAllCall;
    UINT32 m_asterixColourRollCall;
    UINT32 m_asterixColourAllCallComb;
    UINT32 m_asterixColourRollCallComb;
    UINT32 m_asterixColourGhost;
    UINT32 m_asterixColourTest;
    UINT32 m_asterixColourSCF;

    /* Use fusion symbols? */
    int m_useFusionSymbols;

    /* Show tracks excluded by filters? */
    int m_showExcludedTracks;

    /* Colour for selected targets, if non-zero. */
    UINT32 m_selectedColour;

    /* Colour for targets with flight plans, if non-zero. */
    UINT32 m_flightPlanColour;

    /* Target flash support. */
    unsigned int m_emergencyFlashHz;	/* Zero for no flash */
    UINT32 m_emergencyFlashColour;	/* Colour to flash to or 0 for complementary. */
    unsigned int m_spiFlashHz;		/* Zero for no flash */
    UINT32 m_spiFlashColour;		/* Colour to flash to or 0 for complementary. */
    unsigned int m_highlightFlashHz;	/* Zero for no flash */
    UINT32 m_highlightFlashColour;	/* Colour to flash to or 0 for complementary. */

    /* Alpha for coasted fusion targets, if non-zero. */
    UINT8 m_fuseCoastAlpha;

    /* Alpha for lost targets when using threat colours, if non-zero. */
    UINT8 m_threatLostAlpha;

    /* Source of altitude information. */
    SPxHeightMode_t m_altitudeMode;

    /* Pre-render callback function. */
    SPxTrackRendererPreRenderFn_t m_preRenderFn;
    void *m_preRenderArg;

    /*
     * Private functions.
     */

    void isFusedTrack(const SPxRadarTrack *track,
		      int *containsPrimaryRtn,
		      int *containsSecondaryRtn,
		      int *containsM5Rtn=NULL,
		      int *useFuseSymbolsRtn=NULL,
		      int plotNotTrack=FALSE,
		      int mode=TRACK_MODE_ESTABLISHED_UPDATING,
		      SPxSymbolShape_t shape=SPX_SYMBOL_SHAPE_NOT_SET);

    SPxErrorCode getIJMSProps(SPxRadarTrack *track,
			      SPxSymbolShape_t *shapeRtn,
			      UINT32 *colRtn);

    /*
     * Private static functions.
     */

}; /* SPxTrackRendererBase */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TRACK_RENDERER_BASE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
